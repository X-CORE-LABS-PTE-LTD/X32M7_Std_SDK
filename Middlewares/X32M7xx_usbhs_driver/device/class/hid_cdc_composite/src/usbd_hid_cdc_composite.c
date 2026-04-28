/**
 ******************************************************************************
 * @file      usbd_hid_cdc_composite.c
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

#include "usbd_customhid_core.h"
#include "usbd_cdc_core.h"
#include "usbd_hid_cdc_composite.h"
#include "usbd_desc.h"
#include "usbd_req.h"


/* CDC Device library callbacks */
extern uint8_t  USBD_CDC_Init        (void  *pdev, uint8_t cfgidx);
extern uint8_t  USBD_CDC_DeInit      (void  *pdev, uint8_t cfgidx);
extern uint8_t  USBD_CDC_Setup       (void  *pdev, USB_SETUP_REQ *req);
extern uint8_t  USBD_CDC_EP0_RxReady (void *pdev);
extern uint8_t  USBD_CDC_DataIn      (void *pdev, uint8_t epnum);
extern uint8_t  USBD_CDC_DataOut     (void *pdev, uint8_t epnum);
extern uint8_t  USBD_CDC_SOF         (void *pdev);

/* HID Device library callbacks */
extern uint8_t  USBD_CUSTOM_HID_Init (void  *pdev, uint8_t cfgidx);
extern uint8_t  USBD_CUSTOM_HID_DeInit (void  *pdev, uint8_t cfgidx);
extern uint8_t  USBD_CUSTOM_HID_Setup (void  *pdev, USB_SETUP_REQ *req);
extern uint8_t  USBD_CUSTOM_HID_DataIn (void  *pdev, uint8_t epnum);
extern uint8_t  USBD_CUSTOM_HID_DataOut (void  *pdev, uint8_t epnum);
extern uint8_t  USBD_CUSTOM_HID_EP0_RxReady (void *pdev);


static uint8_t  USBD_HID_CDC_Init         (void *pdev , uint8_t cfgidx);
static uint8_t  USBD_HID_CDC_DeInit       (void *pdev , uint8_t cfgidx);

/* Control Endpoints*/
static uint8_t  USBD_HID_CDC_Setup        (void *pdev , USB_SETUP_REQ  *req);  
static uint8_t  USBD_HID_CDC_EP0_RxReady  (void *pdev );

/* Class Specific Endpoints*/
static uint8_t  USBD_HID_CDC_DataIn       (void *pdev , uint8_t epnum);   
static uint8_t  USBD_HID_CDC_DataOut      (void *pdev , uint8_t epnum); 
static uint8_t  USBD_HID_CDC_SOF          (void *pdev); 
static uint8_t* USBD_HID_CDC_GetConfigDescriptor(uint8_t speed , uint16_t *length); 

#define USB_HID_CDC_CONFIG_DESC_SIZ  (USB_CUSTOM_HID_CONFIG_DESC_SIZ -9 + USB_CDC_CONFIG_DESC_SIZ  + 8)

#define HID_INTERFACE       0x00
#define CDC_COM_INTERFACE   0x01


USBD_Class_cb_TypeDef  USBD_HID_CDC_cb = 
{
    USBD_HID_CDC_Init,
    USBD_HID_CDC_DeInit,
    USBD_HID_CDC_Setup,
    NULL, 
    USBD_HID_CDC_EP0_RxReady, 
    USBD_HID_CDC_DataIn, 
    USBD_HID_CDC_DataOut, 
    USBD_HID_CDC_SOF, 
    NULL,
    NULL,
    USBD_HID_CDC_GetConfigDescriptor,
    USBD_HID_CDC_GetConfigDescriptor,
};


#ifdef USB_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_INTERNAL_DMA_ENABLED */ 
/* USB MSC/CDC device Configuration Descriptor */
__ALIGN_BEGIN static uint8_t USBD_HID_CDC_CfgDesc[USB_HID_CDC_CONFIG_DESC_SIZ] __ALIGN_END =
{
    0x09, /* bLength: Configuration Descriptor size */
    USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */
    USB_HID_CDC_CONFIG_DESC_SIZ,
    /* wTotalLength: Bytes returned */
    0x00,   
    0x03,         /*bNumInterfaces: 3 interfaces (2 for CDC, 1 for HID)*/
    0x01,         /*bConfigurationValue: Configuration value*/
    0x00,         /*iConfiguration: Index of string descriptor describing
    the configuration*/
    0xC0,         /*bmAttributes: bus powered and Support Remote Wake-up */
    0x32,         /*MaxPower 100 mA: this current is used for detecting Vbus*/

    /************** Descriptor of Custom HID interface ****************/
    /* 09 */
    0x09,         /*bLength: Interface Descriptor size*/
    USB_INTERFACE_DESCRIPTOR_TYPE,/*bDescriptorType: Interface descriptor type*/
    HID_INTERFACE,/*bInterfaceNumber: Number of Interface*/
    0x00,         /*bAlternateSetting: Alternate setting*/
    0x02,         /*bNumEndpoints*/
    0x03,         /*bInterfaceClass: HID*/
    0x01,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
    0x01,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
    0x00,         /*iInterface: Index of string descriptor*/
    /******************** Descriptor of HID ********************/
    /* 18 */
    0x09,         /*bLength: HID Descriptor size*/
    CUSTOM_HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
    0x10,         /*bcdHID: HID Class Spec release number*/
    0x01,
    0x00,         /*bCountryCode: Hardware target country*/
    0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
    0x22,         /*bDescriptorType*/
    USBD_CUSTOM_HID_REPORT_DESC_SIZE,/*wItemLength: Total length of Report descriptor*/
    0x00,
    /******************** Descriptor of Custom HID endpoint ********************/
    /* 27 */
    0x07,          /*bLength: Endpoint Descriptor size*/
    USB_ENDPOINT_DESCRIPTOR_TYPE, /*bDescriptorType:*/

    HID_IN_EP,     /*bEndpointAddress: Endpoint Address (IN)*/
    0x03,          /*bmAttributes: Interrupt endpoint*/
    HID_IN_PACKET, /*wMaxPacketSize: 4 Byte max */
    0x00,
    0x20,          /*bInterval: Polling Interval (10 ms)*/
    /* 34 */
    
    0x07,          /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: */
    /*    Endpoint descriptor type */
    HID_OUT_EP,   /* bEndpointAddress: */
    /*    Endpoint Address (OUT) */
    0x03, /* bmAttributes: Interrupt endpoint */
    HID_OUT_PACKET, /* wMaxPacketSize */
    0x00,
    0x20, /* bInterval: Polling Interval (10 ms) */

    /******** /IAD should be positioned just before the CDC interfaces ******
                IAD to associate the two CDC interfaces */
    0x08, /* bLength */
    0x0B, /* bDescriptorType */
    0x01, /* bFirstInterface */
    0x02, /* bInterfaceCount */
    0x02, /* bFunctionClass */
    0x02, /* bFunctionSubClass */
    0x01, /* bFunctionProtocol */
    0x00, /* iFunction (Index of string descriptor describing this function) */

    /*Interface Descriptor */
    0x09,   /* bLength: Interface Descriptor size */
    USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: Interface */
    /* Interface descriptor type */
    CDC_COM_INTERFACE,   /* bInterfaceNumber: Number of Interface */
    0x00,   /* bAlternateSetting: Alternate setting */
    0x01,   /* bNumEndpoints: One endpoints used */
    0x02,   /* bInterfaceClass: Communication Interface Class */
    0x02,   /* bInterfaceSubClass: Abstract Control Model */
    0x01,   /* bInterfaceProtocol: Common AT commands */
    0x01,   /* iInterface: */

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
    0x02,   /* bDataInterface: 2 */

    /*ACM Functional Descriptor*/
    0x04,   /* bFunctionLength */
    0x24,   /* bDescriptorType: CS_INTERFACE */
    0x02,   /* bDescriptorSubtype: Abstract Control Management desc */
    0x02,   /* bmCapabilities */

    /*Union Functional Descriptor*/
    0x05,   /* bFunctionLength */
    0x24,   /* bDescriptorType: CS_INTERFACE */
    0x06,   /* bDescriptorSubtype: Union func desc */
    0x01,   /* bMasterInterface: Communication class interface */
    0x02,   /* bSlaveInterface0: Data Class Interface */

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
    0x02,   /* bInterfaceNumber: Number of Interface */
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
    LOBYTE(CDC_DATA_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
    HIBYTE(CDC_DATA_MAX_PACKET_SIZE),
    0x00,                              /* bInterval: ignore for Bulk transfer */

    /*Endpoint IN Descriptor*/
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,     /* bDescriptorType: Endpoint */
    CDC_IN_EP,                        /* bEndpointAddress */
    0x02,                             /* bmAttributes: Bulk */
    LOBYTE(CDC_DATA_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
    HIBYTE(CDC_DATA_MAX_PACKET_SIZE),
    0x00,                              /* bInterval */
} ;

/**
  * @brief  USBD_MSC_CDC_Init
  *         Initialize the MSC & CDC interfaces
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t USBD_HID_CDC_Init (void *pdev, uint8_t cfgidx)
{
    /* HID initialization */
    USBD_CUSTOM_HID_Init(pdev, cfgidx);

    /* CDC initialization */
    USBD_CDC_Init(pdev, cfgidx);

    return USBD_OK;
}

/**
  * @brief  USBD_MSC_CDC_Init
  *         DeInitialize the MSC/CDC interfaces
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t USBD_HID_CDC_DeInit(void *pdev, uint8_t cfgidx)
{
    /* HID De-initialization */
    USBD_CUSTOM_HID_DeInit(pdev,cfgidx);

    /* CDC De-initialization */
    USBD_CDC_DeInit(pdev,cfgidx);

    return USBD_OK;
}

/**
  * @brief  USBD_MSC_CDC_Setup
  *         Handle the MSC/CDC specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
static uint8_t USBD_HID_CDC_Setup(void *pdev, USB_SETUP_REQ *req)
{
    switch (req->bmRequest & USB_REQ_RECIPIENT_MASK)
    {
        case USB_REQ_RECIPIENT_INTERFACE:
            if (req->wIndex == HID_INTERFACE)
            {
                return (USBD_CUSTOM_HID_Setup(pdev, req));
            }
            else
            {
                return (USBD_CDC_Setup(pdev, req));
            }

        case USB_REQ_RECIPIENT_ENDPOINT:
            if (req->wIndex == HID_IN_EP)
            {
                return (USBD_CUSTOM_HID_Setup(pdev, req));   
            }
            else
            {
              return (USBD_CDC_Setup(pdev, req));
            }
            
        default:
            break;
    }   
    return USBD_OK;
}

/**
  * @brief  USBD_MSC_CDC_GetCfgDesc 
  *         return configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t *USBD_HID_CDC_GetConfigDescriptor(uint8_t speed, uint16_t *length)
{
    *length = sizeof (USBD_HID_CDC_CfgDesc);
    return USBD_HID_CDC_CfgDesc;
}

/**
  * @brief  USBD_MSC_CDC_DataIn
  *         handle data IN Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t USBD_HID_CDC_DataIn (void *pdev, uint8_t epnum)
{
    /*DataIN can be for CDC or MSC */
    if (epnum == (CDC_IN_EP&0x0F))
    {
        return (USBD_CDC_DataIn(pdev, epnum));
    }
    else
    {
        return (USBD_CUSTOM_HID_DataIn(pdev, epnum));
    }
}


static uint8_t USBD_HID_CDC_DataOut(void *pdev , uint8_t epnum)
{
    /*DataOut can be for CDC */
    if(epnum == (CDC_OUT_EP&0x0F))
    {
        return (USBD_CDC_DataOut(pdev, epnum));
    }
    else
    {
        return (USBD_CUSTOM_HID_DataOut(pdev, epnum));
    }  
}


static uint8_t USBD_HID_CDC_SOF (void *pdev)
{
    /*SOF processing needed for CDC */
    return (USBD_CDC_SOF(pdev));
}


static uint8_t  USBD_HID_CDC_EP0_RxReady(void *pdev)
{
    /*RxReady processing needed for CDC only*/
    return (USBD_CDC_EP0_RxReady(pdev));
}
