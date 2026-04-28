/**
 ******************************************************************************
 * @file      usbd_cdc_core.c
 * @version   v1.0.0
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 X-CORE LABS PTE. LTD.
 * All rights reserved.
 *
 * Usage, modification, and distribution of this software, whether in source
 * code or compiled binary format, are authorized provided that the following
 * criteria are strictly met:
 *
 * 1. Any distribution of the source code must leave this copyright header,
 * the current list of conditions, and the enclosed disclaimer intact.
 * 2. When distributing in binary format, the corresponding documentation
 * and/or materials must clearly replicate this copyright notice, the
 * conditions, and the disclaimer.
 * 3. The name of the organization or its contributors cannot be utilized to
 * promote or endorse any derivative products built upon this software
 * without obtaining prior explicit written consent.
 *
 * THIS SOFTWARE PACKAGE IS DELIVERED ON AN "AS-IS" BASIS. THE COPYRIGHT
 * HOLDERS AND AUTHORS EXPRESSLY DISCLAIM ALL WARRANTIES, WHETHER EXPLICIT
 * OR IMPLIED, WHICH INCLUDES BUT IS NOT LIMITED TO ANY IMPLIED ASSURANCES
 * OF COMMERCIAL UTILITY OR FITNESS FOR A SPECIFIC REQUIREMENT. UNDER NO
 * CIRCUMSTANCES SHALL THE CREATORS OR CONTRIBUTORS BE HELD ACCOUNTABLE FOR
 * ANY DIRECT, INDIRECT, ACCIDENTAL, SPECIAL, PUNITIVE, OR SUBSEQUENT DAMAGES
 * (SUCH AS, BUT NOT LIMITED TO, PROCUREMENT OF ALTERNATIVE GOODS, LOSS OF
 * DATA, LOSS OF REVENUE, OR OPERATIONAL INTERRUPTIONS). THIS LIMITATION OF
 * LIABILITY APPLIES REGARDLESS OF THE LEGAL THEORY STATED - WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE) - ARISING FROM
 * ANY USE OF THIS CODE, EVEN IF PREVIOUSLY ADVISED OF SUCH POTENTIAL RISKS.
 ******************************************************************************
 */

#include "usbd_cdc_core.h"
#include "usbd_desc.h"
#include "usbd_req.h"
#include "usbhs_bsp.h"

#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#define USB_CDC_IDLE         0
#define USB_CDC_BUSY         1
#define USB_CDC_ZLP          2

/* CDC Device library callbacks */
uint8_t  USBD_CDC_Init        (void  *pdev, uint8_t cfgidx);
uint8_t  USBD_CDC_DeInit      (void  *pdev, uint8_t cfgidx);
uint8_t  USBD_CDC_Setup       (void  *pdev, USB_SETUP_REQ *req);
uint8_t  USBD_CDC_EP0_RxReady (void *pdev);
uint8_t  USBD_CDC_DataIn      (void *pdev, uint8_t epnum);
uint8_t  USBD_CDC_DataOut     (void *pdev, uint8_t epnum);
uint8_t  USBD_CDC_SOF         (void *pdev);

/* CDC specific management functions */
static void Handle_USBAsynchXfer  (void *pdev);
static uint8_t *USBD_CDC_GetCfgDesc (uint8_t speed, uint16_t *length);
static uint8_t *USBD_CDC_GetOtherCfgDesc (uint8_t speed, uint16_t *length);


extern CDC_IF_Prop_TypeDef  APP_FOPS;
extern uint8_t USBD_DeviceDesc   [USB_SIZ_DEVICE_DESC];

#ifdef USB_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN uint8_t USBD_CDC_CfgDesc  [USB_CDC_CONFIG_DESC_SIZ] __ALIGN_END ;

#ifdef USB_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN uint8_t USBD_CDC_OtherCfgDesc  [USB_CDC_CONFIG_DESC_SIZ] __ALIGN_END ;

#ifdef USB_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN static __IO uint32_t  USBD_CDC_AltSet  __ALIGN_END = 0;

#ifdef USB_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN uint8_t USB_Rx_Buffer   [CDC_DATA_MAX_PACKET_SIZE] __ALIGN_END ;

#ifdef USB_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN uint8_t APP_Rx_Buffer   [APP_RX_DATA_SIZE] __ALIGN_END ;

#ifdef USB_INTERNAL_DMA_ENABLED
/*
 * DMA-aligned IN bounce buffer.
 *
 * X32 USB internal DMA requires the source address of USBDEV_EP_Tx to be
 * 4-byte aligned. APP_Rx_Buffer's base is aligned, but &APP_Rx_Buffer[ptr_out]
 * is not guaranteed aligned because APP_Rx_ptr_out advances by arbitrary
 * byte counts (payloads of 9, 11, 17 ... are typical for MSP responses).
 *
 * Before each IN submission we memcpy the payload into this aligned buffer,
 * so the DMA engine always sees a 4-byte-aligned start address.
 *
 * Size matches CDC_DATA_IN_PACKET_SIZE (usually 64) — the maximum length of
 * a single IN transfer. Only compiled when USB_INTERNAL_DMA_ENABLED is
 * defined, so non-DMA builds pay zero RAM cost.
 */
  #if defined ( __ICCARM__ ) /*IAR Compiler */
    #pragma data_alignment=4
  #endif
__ALIGN_BEGIN static uint8_t USB_Tx_Buffer[CDC_DATA_IN_PACKET_SIZE] __ALIGN_END;
#endif /* USB_INTERNAL_DMA_ENABLED */


#ifdef USB_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN uint8_t CmdBuff[CDC_CMD_PACKET_SIZE] __ALIGN_END ;

/*
 * Fast word-copy from potentially-unaligned source to aligned destination.
 *
 * Designed for the USB IN bounce path where:
 *   - dst (USB_Tx_Buffer) is always 4-byte aligned (__ALIGN_BEGIN)
 *   - src (&APP_Rx_Buffer[ptr_out]) may have ANY byte alignment
 *   - n is at most CDC_DATA_IN_PACKET_SIZE (64 FS / 512 HS)
 *
 * On Cortex-M7, unaligned 32-bit LDR is handled in hardware with at most
 * one extra cycle when crossing a word boundary — no fault, no fixup trap.
 * LDRD (64-bit) would fault on unaligned addresses, so we stick to 32-bit.
 *
 * Cycle budget (M7 @ 400 MHz, D-cache hit):
 *   FS  64B  = 16 words → ~18 cycles  ≈  45 ns
 *   HS 512B  = 128 words → ~130 cycles ≈ 325 ns
 *
 * Zero external dependency — no libc, no __builtin, fully deterministic.
 */
static inline void cdc_copy_to_aligned(uint8_t *dst, const uint8_t *src, uint16_t n)
{
    /* Bulk: 4 bytes per iteration via word load/store.
     * dst is guaranteed 4-byte aligned; src may not be, but M7
     * handles unaligned word reads in hardware transparently. */
    uint32_t       *d = (uint32_t *)dst;
    const uint32_t *s = (const uint32_t *)src;
    uint16_t words = n >> 2;
    while (words--) {
        *d++ = *s++;
    }

    /* Tail: 0–3 remaining bytes via narrower accesses */
    uint8_t       *d8 = (uint8_t *)d;
    const uint8_t *s8 = (const uint8_t *)s;
    if (n & 2u) {
        *((uint16_t *)d8) = *((const uint16_t *)s8);
        d8 += 2; s8 += 2;
    }
    if (n & 1u) {
        *d8 = *s8;
    }
}

/*
 * Submit one IN packet to the host with guaranteed 4-byte-aligned source address.
 *
 * With USB_INTERNAL_DMA_ENABLED:
 *   Copy APP_Rx_Buffer[ptrOut .. ptrOut+length) into the aligned USB_Tx_Buffer,
 *   then hand USB_Tx_Buffer to the IN endpoint. The source range is always
 *   contiguous — Handle_USBAsynchXfer caps APP_Rx_length at
 *   (APP_RX_DATA_SIZE - ptr_out) when wrapping, so no mid-transfer ring wrap.
 *
 * Without USB_INTERNAL_DMA_ENABLED:
 *   USBDEV_EP_Tx copies into the EP FIFO by CPU, alignment is irrelevant —
 *   pass &APP_Rx_Buffer[ptrOut] directly and save the copy.
 */
static inline void cdc_submit_in_packet(void *pdev, uint32_t ptrOut, uint16_t length)
{
#ifdef USB_INTERNAL_DMA_ENABLED
    cdc_copy_to_aligned(USB_Tx_Buffer, &APP_Rx_Buffer[ptrOut], length);
    USBDEV_EP_Tx(pdev, CDC_IN_EP, USB_Tx_Buffer, length);
#else
    USBDEV_EP_Tx(pdev, CDC_IN_EP, (uint8_t *)&APP_Rx_Buffer[ptrOut], length);
#endif
}

volatile uint32_t APP_Rx_ptr_in  = 0;
volatile uint32_t APP_Rx_ptr_out = 0;
uint32_t APP_Rx_length  = 0;

volatile uint8_t  USB_Tx_State = USB_CDC_IDLE;

static uint32_t cdcCmd = 0xFF;
static uint32_t cdcLen = 0;

/* CDC interface class callbacks structure */
USBD_Class_cb_TypeDef  USBD_CDC_cb = 
{
    USBD_CDC_Init,
    USBD_CDC_DeInit,
    USBD_CDC_Setup,
    NULL,                 /* EP0_TxSent, */
    USBD_CDC_EP0_RxReady,
    USBD_CDC_DataIn,
    USBD_CDC_DataOut,
    USBD_CDC_SOF,
    NULL,
    NULL,     
    USBD_CDC_GetCfgDesc,
    USBD_CDC_GetOtherCfgDesc, /* use same cobfig as per FS */
};

#ifdef USB_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_INTERNAL_DMA_ENABLED */
/* USB CDC device Configuration Descriptor */
__ALIGN_BEGIN uint8_t USBD_CDC_CfgDesc[USB_CDC_CONFIG_DESC_SIZ]  __ALIGN_END =
{
    /* Configuration Descriptor */
    0x09,                                       /* bLength: Configuration Descriptor size */
    USB_DESC_TYPE_CONFIGURATION,                /* bDescriptorType: Configuration */
    USB_CDC_CONFIG_DESC_SIZ,                    /* wTotalLength */
    0x00,
    0x02,                                       /* bNumInterfaces: 2 interfaces */
    0x01,                                       /* bConfigurationValue: Configuration value */
    0x00,                                       /* iConfiguration: Index of string descriptor */
    0xC0,                                       /* bmAttributes: self powered */
    0x32,                                       /* MaxPower 100 mA */

    /* Interface Descriptor */
    0x09,                                       /* bLength: Interface Descriptor size */
    USB_DESC_TYPE_INTERFACE,                    /* bDescriptorType: Interface */
    /* Interface descriptor type */
    0x00,                                       /* bInterfaceNumber: Number of Interface */
    0x00,                                       /* bAlternateSetting: Alternate setting */
    0x01,                                       /* bNumEndpoints: One endpoint used */
    0x02,                                       /* bInterfaceClass: Communication Interface Class */
    0x02,                                       /* bInterfaceSubClass: Abstract Control Model */
    0x01,                                       /* bInterfaceProtocol: Common AT commands */
    0x00,                                       /* iInterface */

    /* Header Functional Descriptor */
    0x05,                                       /* bLength: Endpoint Descriptor size */
    0x24,                                       /* bDescriptorType: CS_INTERFACE */
    0x00,                                       /* bDescriptorSubtype: Header Func Desc */
    0x10,                                       /* bcdCDC: spec release number */
    0x01,

    /* Call Management Functional Descriptor */
    0x05,                                       /* bFunctionLength */
    0x24,                                       /* bDescriptorType: CS_INTERFACE */
    0x01,                                       /* bDescriptorSubtype: Call Management Func Desc */
    0x00,                                       /* bmCapabilities: D0+D1 */
    0x01,                                       /* bDataInterface */

    /* ACM Functional Descriptor */
    0x04,                                       /* bFunctionLength */
    0x24,                                       /* bDescriptorType: CS_INTERFACE */
    0x02,                                       /* bDescriptorSubtype: Abstract Control Management desc */
    0x02,                                       /* bmCapabilities */

    /* Union Functional Descriptor */
    0x05,                                       /* bFunctionLength */
    0x24,                                       /* bDescriptorType: CS_INTERFACE */
    0x06,                                       /* bDescriptorSubtype: Union func desc */
    0x00,                                       /* bMasterInterface: Communication class interface */
    0x01,                                       /* bSlaveInterface0: Data Class Interface */

    /* Endpoint 2 Descriptor */
    0x07,                                       /* bLength: Endpoint Descriptor size */
    USB_DESC_TYPE_ENDPOINT,                     /* bDescriptorType: Endpoint */
    CDC_CMD_EP,                                 /* bEndpointAddress */
    0x03,                                       /* bmAttributes: Interrupt */
    LOBYTE(CDC_CMD_PACKET_SIZE),                /* wMaxPacketSize */
    HIBYTE(CDC_CMD_PACKET_SIZE),
    0x10,                                       /* bInterval */
    /*---------------------------------------------------------------------------*/

    /* Data class interface descriptor */
    0x09,                                       /* bLength: Endpoint Descriptor size */
    USB_DESC_TYPE_INTERFACE,                    /* bDescriptorType: */
    0x01,                                       /* bInterfaceNumber: Number of Interface */
    0x00,                                       /* bAlternateSetting: Alternate setting */
    0x02,                                       /* bNumEndpoints: Two endpoints used */
    0x0A,                                       /* bInterfaceClass: CDC */
    0x00,                                       /* bInterfaceSubClass */
    0x00,                                       /* bInterfaceProtocol */
    0x00,                                       /* iInterface */

    /* Endpoint OUT Descriptor */
    0x07,                                       /* bLength: Endpoint Descriptor size */
    USB_DESC_TYPE_ENDPOINT,                     /* bDescriptorType: Endpoint */
    CDC_OUT_EP,                                 /* bEndpointAddress */
    0x02,                                       /* bmAttributes: Bulk */
    LOBYTE(CDC_DATA_MAX_PACKET_SIZE),           /* wMaxPacketSize: */
    HIBYTE(CDC_DATA_MAX_PACKET_SIZE),
    0x00,                                       /* bInterval */

    /* Endpoint IN Descriptor */
    0x07,                                       /* bLength: Endpoint Descriptor size */
    USB_DESC_TYPE_ENDPOINT,                     /* bDescriptorType: Endpoint */
    CDC_IN_EP,                                  /* bEndpointAddress */
    0x02,                                       /* bmAttributes: Bulk */
    LOBYTE(CDC_DATA_MAX_PACKET_SIZE),        /* wMaxPacketSize: */
    HIBYTE(CDC_DATA_MAX_PACKET_SIZE),
    0x00                                        /* bInterval */
};

#ifdef USB_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_INTERNAL_DMA_ENABLED */ 
__ALIGN_BEGIN uint8_t USBD_CDC_OtherCfgDesc[USB_CDC_CONFIG_DESC_SIZ]  __ALIGN_END =
{ 
    0x09,   /* bLength: Configuration Descriptor size */
    USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION,   
    USB_CDC_CONFIG_DESC_SIZ,
    0x00,
    0x02,   /* bNumInterfaces: 2 interfaces */
    0x01,   /* bConfigurationValue: */
    0x04,   /* iConfiguration: */
    0xC0,   /* bmAttributes: */
    0x32,   /* MaxPower 100 mA */  

    /*Interface Descriptor */
    0x09,   /* bLength: Interface Descriptor size */
    USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: Interface */
    /* Interface descriptor type */
    0x00,   /* bInterfaceNumber: Number of Interface */
    0x00,   /* bAlternateSetting: Alternate setting */
    0x01,   /* bNumEndpoints: One endpoints used */
    0x02,   /* bInterfaceClass: Communication Interface Class */
    0x02,   /* bInterfaceSubClass: Abstract Control Model */
    0x01,   /* bInterfaceProtocol: Common AT commands */
    0x00,   /* iInterface: */

    /*Header Functional Descriptor*/
    0x05,   /* bLength: Endpoint Descriptor size */
    0x24,   /* bDescriptorType: CS_INTERFACE */
    0x00,   /* bDescriptorSubtype: Header Func Desc */
    0x10,   /* bcdCDC: spec release number */
    0x01,

    /*Call Management Functional Descriptor*/
    0x05,   /* bFunctionLength */
    0x24,   /* bDescriptorType: CS_INTERFACE */
    0x01,   /* bDescriptorSubtype: Call Management Func Desc */
    0x00,   /* bmCapabilities: D0+D1 */
    0x01,   /* bDataInterface: 1 */

    /*ACM Functional Descriptor*/
    0x04,   /* bFunctionLength */
    0x24,   /* bDescriptorType: CS_INTERFACE */
    0x02,   /* bDescriptorSubtype: Abstract Control Management desc */
    0x02,   /* bmCapabilities */

    /*Union Functional Descriptor*/
    0x05,   /* bFunctionLength */
    0x24,   /* bDescriptorType: CS_INTERFACE */
    0x06,   /* bDescriptorSubtype: Union func desc */
    0x00,   /* bMasterInterface: Communication class interface */
    0x01,   /* bSlaveInterface0: Data Class Interface */

    /*Endpoint 2 Descriptor*/
    0x07,                           /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */
    CDC_CMD_EP,                     /* bEndpointAddress */
    0x03,                           /* bmAttributes: Interrupt */
    LOBYTE(CDC_CMD_PACKET_SIZE),    /* wMaxPacketSize: */
    HIBYTE(CDC_CMD_PACKET_SIZE),
    0xFF,                           /* bInterval: */

    /*---------------------------------------------------------------------------*/

    /*Data class interface descriptor*/
    0x09,   /* bLength: Endpoint Descriptor size */
    USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: */
    0x01,   /* bInterfaceNumber: Number of Interface */
    0x00,   /* bAlternateSetting: Alternate setting */
    0x02,   /* bNumEndpoints: Two endpoints used */
    0x0A,   /* bInterfaceClass: CDC */
    0x00,   /* bInterfaceSubClass: */
    0x00,   /* bInterfaceProtocol: */
    0x00,   /* iInterface: */

    /*Endpoint OUT Descriptor*/
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType: Endpoint */
    CDC_OUT_EP,                        /* bEndpointAddress */
    0x02,                              /* bmAttributes: Bulk */
    0x40,                              /* wMaxPacketSize: */
    0x00,
    0x00,                              /* bInterval: ignore for Bulk transfer */

    /*Endpoint IN Descriptor*/
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,     /* bDescriptorType: Endpoint */
    CDC_IN_EP,                        /* bEndpointAddress */
    0x02,                             /* bmAttributes: Bulk */
    0x40,                             /* wMaxPacketSize: */
    0x00,
    0x00                              /* bInterval */
};

/**
*\*\name    USBD_CDC_Init.
*\*\fun     Initialize the CDC interface.
*\*\param   pdev: device instance.
*\*\param   cfgidx: Configuration index
*\*\return  USBD_OK.
*\*\
**/
uint8_t USBD_CDC_Init (void  *pdev, uint8_t cfgidx)
{
    /* Open EP IN */
    USBDEV_EP_Open(pdev, CDC_IN_EP, CDC_DATA_IN_PACKET_SIZE, USB_EP_BULK);

    /* Open EP OUT */
    USBDEV_EP_Open(pdev, CDC_OUT_EP, CDC_DATA_OUT_PACKET_SIZE, USB_EP_BULK);

    /* Open Command IN EP */
    USBDEV_EP_Open(pdev, CDC_CMD_EP, CDC_CMD_PACKET_SIZE, USB_EP_INT);

    /* Initialize the Interface physical components */
    APP_FOPS.pIf_Init();

    /* Prepare Out endpoint to receive next packet */
    USBDEV_EP_PrepareRx(pdev, CDC_OUT_EP, (uint8_t*)(USB_Rx_Buffer), CDC_DATA_OUT_PACKET_SIZE);
  
    return USBD_OK;
}

/**
*\*\name    USBD_CDC_DeInit.
*\*\fun     DeInitialize the CDC layer.
*\*\param   pdev: device instance.
*\*\param   cfgidx: Configuration index
*\*\return  USBD_OK.
*\*\
**/
uint8_t  USBD_CDC_DeInit (void  *pdev, uint8_t cfgidx)
{
    /* Open EP IN */
    USBDEV_EP_Close(pdev, CDC_IN_EP);

    /* Open EP OUT */
    USBDEV_EP_Close(pdev, CDC_OUT_EP);

    /* Open Command IN EP */
    USBDEV_EP_Close(pdev, CDC_CMD_EP);

    /* Restore default state of the Interface physical components */
    APP_FOPS.pIf_DeInit();

    return USBD_OK;
}

/**
*\*\name    USBD_CDC_Setup
*\*\fun     Handle the CDC specific requests.
*\*\param   pdev: device instance.
*\*\param   req: usb requests
*\*\return  USBD_OK.
**/
uint8_t USBD_CDC_Setup (void  *pdev, USB_SETUP_REQ *req)
{
    switch (req->bmRequest & USB_REQ_TYPE_MASK)
    {
        /* CDC Class Requests -------------------------------*/
        case USB_REQ_TYPE_CLASS :
            /* Check if the request is a data setup packet */
            if (req->wLength)
            {
                /* Check if the request is Device-to-Host */
                if (req->bmRequest & 0x80)
                {
                    /* Get the data to be sent to Host from interface layer */
                    APP_FOPS.pIf_Ctrl(req->bRequest, CmdBuff, req->wLength);
                  
                    /* Send the data to the host */
                    USBD_CtrlSendData(pdev, CmdBuff, req->wLength);
                }
                else /* Host-to-Device request */
                {
                    /* Set the value of the current command to be processed */
                    cdcCmd = req->bRequest;
                    cdcLen = req->wLength;
                  
                    /* Prepare the reception of the buffer over EP0
                    Next step: the received data will be managed in usbd_cdc_EP0_TxSent() 
                    function. */
                    USBD_CtrlPrepareRx (pdev, CmdBuff, req->wLength);
                }
            }
            else /* No Data request */
            {
                /* Transfer the command to the interface layer */
                APP_FOPS.pIf_Ctrl(req->bRequest, NULL, 0);
            }
            break;

        /* Standard Requests -------------------------------*/
        case USB_REQ_TYPE_STANDARD:
            switch (req->bRequest)
            {
                case USB_REQ_GET_DESCRIPTOR: 
                    USBD_CtrlError (pdev, req);
                    return USBD_FAIL;

                case USB_REQ_GET_INTERFACE :
                    USBD_CtrlSendData (pdev, (uint8_t *)&USBD_CDC_AltSet, 1);
                    break;

                case USB_REQ_SET_INTERFACE :
                    if ((uint8_t)(req->wValue) < USBD_ITF_MAX_NUM)
                    {
                        USBD_CDC_AltSet = (uint8_t)(req->wValue);
                    }
                    else
                    {
                        /* Call the error management function (command will be nacked */
                        USBD_CtrlError (pdev, req);
                    }
                    break;
                    
                default:
                    break;
            }
            break;
            
        default:
            USBD_CtrlError (pdev, req);
            return USBD_FAIL;
    }
    return USBD_OK;
}

/**
*\*\name    USBD_CDC_EP0_RxReady
*\*\fun     Data received on control endpoint.
*\*\param   pdev: device instance.
*\*\return  USBD_OK.
**/
uint8_t  USBD_CDC_EP0_RxReady (void  *pdev)
{ 
    if (cdcCmd != NO_CMD)
    {
        /* Process the data */
        APP_FOPS.pIf_Ctrl(cdcCmd, CmdBuff, cdcLen);
        /* Reset the command variable to default value */
        cdcCmd = NO_CMD;
    }

    return USBD_OK;
}


/**
*\*\name    USBD_CDC_DataIn.
*\*\fun     handle data IN Stage.
*\*\param   pdev: device instance.
*\*\param   epnum: endpoint index.
*\*\return  USBD_OK.
*\*\
**/
uint8_t  USBD_CDC_DataIn (void *pdev, uint8_t epnum)
{
    uint16_t USB_Tx_length;

    if (USB_Tx_State == USB_CDC_BUSY)
    {
        if (APP_Rx_length == 0)
        {
            USB_Tx_State = USB_CDC_IDLE;
        }
        else
        {
            if (APP_Rx_length > CDC_DATA_IN_PACKET_SIZE)
            {
                USB_Tx_length = CDC_DATA_IN_PACKET_SIZE;
            }
            else
            {
                USB_Tx_length = APP_Rx_length;

                if(USB_Tx_length == CDC_DATA_IN_PACKET_SIZE)
                {
                    USB_Tx_State = USB_CDC_ZLP;
                }
            }

            /* Prepare the available data buffer to be sent on IN endpoint.
             * Goes through cdc_submit_in_packet so DMA builds get a 4-byte
             * aligned source address via the USB_Tx_Buffer bounce. */
            cdc_submit_in_packet(pdev, APP_Rx_ptr_out, USB_Tx_length);

            /* Advance out pointer with modulo wrapping */
            APP_Rx_ptr_out = (APP_Rx_ptr_out + USB_Tx_length) % APP_RX_DATA_SIZE;
            APP_Rx_length -= USB_Tx_length;

            return USBD_OK;
        }
    }

    /* Avoid any asynchronous transfer during ZLP */
    if (USB_Tx_State == USB_CDC_ZLP)
    {
        /*Send ZLP to indicate the end of the current transfer */
        USBDEV_EP_Tx (pdev, CDC_IN_EP, NULL, 0);

        USB_Tx_State = USB_CDC_IDLE;
    }
    return USBD_OK;
}

/**
*\*\name    USBD_CDC_DataOut.
*\*\fun     handle data OUT Stage.
*\*\param   pdev: device instance.
*\*\param   epnum: endpoint index.
*\*\return  USBD_OK.
*\*\
**/
uint8_t  USBD_CDC_DataOut (void *pdev, uint8_t epnum)
{      
    uint16_t USB_Rx_Cnt;

    /* Get the received data buffer and update the counter */
    USB_Rx_Cnt = ((USB_CORE_MODULE*)pdev)->dev.out_ep[epnum].xfer_count;

    /* USB data will be immediately processed, this allow next USB traffic being 
    NAKed till the end of the application Xfer */
    APP_FOPS.pIf_DataRx(USB_Rx_Buffer, USB_Rx_Cnt);

    /* Prepare Out endpoint to receive next packet */
    USBDEV_EP_PrepareRx(pdev, CDC_OUT_EP, (uint8_t*)(USB_Rx_Buffer), CDC_DATA_OUT_PACKET_SIZE);

    return USBD_OK;
}

/**
*\*\name    USBD_CDC_SOF.
*\*\fun     Start Of Frame event management.
*\*\param   pdev: device instance.
*\*\return  USBD_OK.
*\*\
**/
uint8_t  USBD_CDC_SOF (void *pdev)
{      
    static uint32_t FrameCount = 0;

    if (FrameCount++ == CDC_IN_FRAME_INTERVAL)
    {
        /* Reset the frame counter */
        FrameCount = 0;

        /* Check the data to be sent through IN pipe */
        Handle_USBAsynchXfer(pdev);
    }

    return USBD_OK;
}

/**
*\*\name    Handle_USBAsynchXfer.
*\*\fun     process the data received from usart and send through USB to host.
*\*\param   pdev: device instance.
*\*\return  USBD_OK.
*\*\
**/
static void Handle_USBAsynchXfer (void *pdev)
{
    uint16_t USB_Tx_length;

    if(USB_Tx_State == USB_CDC_IDLE)
    {
        if(APP_Rx_ptr_out == APP_Rx_ptr_in)
        {
            return;
        }

        if(APP_Rx_ptr_out > APP_Rx_ptr_in) /* rollback */
        {
            APP_Rx_length = APP_RX_DATA_SIZE - APP_Rx_ptr_out;
        }
        else
        {
            APP_Rx_length = APP_Rx_ptr_in - APP_Rx_ptr_out;
        }


        if (APP_Rx_length > CDC_DATA_IN_PACKET_SIZE)
        {
            USB_Tx_length = CDC_DATA_IN_PACKET_SIZE;

            USB_Tx_State = USB_CDC_BUSY;
        }
        else
        {
            USB_Tx_length = APP_Rx_length;

            if(USB_Tx_length == CDC_DATA_IN_PACKET_SIZE)
            {
                USB_Tx_State = USB_CDC_ZLP;
            }
            else
            {
                USB_Tx_State = USB_CDC_BUSY;
            }
        }

        cdc_submit_in_packet(pdev, APP_Rx_ptr_out, USB_Tx_length);

        APP_Rx_ptr_out = (APP_Rx_ptr_out + USB_Tx_length) % APP_RX_DATA_SIZE;
        APP_Rx_length -= USB_Tx_length;
    }
}

/**
*\*\name    USBD_CDC_GetCfgDesc.
*\*\fun     Return configuration descriptor.
*\*\param   speed : current device speed
*\*\param   length : pointer data length
*\*\return  pointer to descriptor buffer.
*\*\
**/
static uint8_t *USBD_CDC_GetCfgDesc (uint8_t speed, uint16_t *length)
{
    *length = sizeof (USBD_CDC_CfgDesc);
    return USBD_CDC_CfgDesc;
}

/**
*\*\name    USBD_CDC_GetOtherCfgDesc.
*\*\fun     Return configuration descriptor.
*\*\param   speed : current device speed
*\*\param   length : pointer data length
*\*\return  pointer to descriptor buffer.
*\*\
**/
static uint8_t *USBD_CDC_GetOtherCfgDesc (uint8_t speed, uint16_t *length)
{
  *length = sizeof (USBD_CDC_OtherCfgDesc);
  return USBD_CDC_OtherCfgDesc;
}

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
