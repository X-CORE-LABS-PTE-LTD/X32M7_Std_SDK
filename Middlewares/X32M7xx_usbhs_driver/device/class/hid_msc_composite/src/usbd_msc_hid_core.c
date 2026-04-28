/**
 ******************************************************************************
 * @file      usbd_msc_hid_core.c
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

#include "usbd_msc_hid_core.h"
#include "usbd_msc_core.h"
#include "usbd_customhid_core.h"
#include "usbd_desc.h"
#include "usbd_req.h"


static uint8_t  USBD_MSC_HID_Init (void  *pdev, 
                               uint8_t cfgidx);

static uint8_t  USBD_MSC_HID_DeInit (void  *pdev, 
                                 uint8_t cfgidx);

static uint8_t  USBD_MSC_HID_Setup (void  *pdev, 
                                USB_SETUP_REQ *req);

static uint8_t  *USBD_MSC_HID_GetCfgDesc (uint8_t speed, uint16_t *length);

static uint8_t  USBD_MSC_HID_DataIn (void  *pdev, uint8_t epnum);
static uint8_t  USBD_MSC_HID_DataOut(void *pdev , uint8_t epnum);

#define HID_INTERFACE 0x0
#define MSC_INTERFACE 0x1


#define USB_MSC_HID_CONFIG_DESC_SIZ  (USB_CUSTOM_HID_CONFIG_DESC_SIZ -9 + USB_MSC_CONFIG_DESC_SIZ)

USBD_Class_cb_TypeDef  USBD_MSC_HID_cb = 
{
    USBD_MSC_HID_Init,
    USBD_MSC_HID_DeInit,
    USBD_MSC_HID_Setup,
    NULL, /*EP0_TxSent*/  
    NULL, /*EP0_RxReady*/
    USBD_MSC_HID_DataIn, /*DataIn*/
    USBD_MSC_HID_DataOut, /*DataOut*/
    NULL, /*SOF */
    NULL,
    NULL,      
    USBD_MSC_HID_GetCfgDesc,
    USBD_MSC_HID_GetCfgDesc, /* use same config as per FS */
};

#ifdef USB_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_INTERNAL_DMA_ENABLED */ 
/* USB HID device Configuration Descriptor */
__ALIGN_BEGIN static uint8_t USBD_MSC_HID_CfgDesc[USB_MSC_HID_CONFIG_DESC_SIZ] __ALIGN_END =
{
    0x09, /* bLength: Configuration Descriptor size */
    USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */
    USB_MSC_HID_CONFIG_DESC_SIZ,
    /* wTotalLength: Bytes returned */
    0x00,
    0x02,         /*bNumInterfaces: 2 interface*/
    0x01,         /*bConfigurationValue: Configuration value*/
    0x00,         /*iConfiguration: Index of string descriptor describing
    the configuration*/
    0xC0,         /*bmAttributes: bus powered and Support Remote Wake-up */
    0x32,         /*MaxPower 100 mA: this current is used for detecting Vbus*/

    /************** Descriptor of HID interface ****************/
    /* 09 */
    0x09,         /*bLength: Interface Descriptor size*/
    USB_INTERFACE_DESCRIPTOR_TYPE,/*bDescriptorType: Interface descriptor type*/
    HID_INTERFACE,         /*bInterfaceNumber: Number of Interface*/
    0x00,         /*bAlternateSetting: Alternate setting*/
    0x02,         /*bNumEndpoints*/
    0x03,         /*bInterfaceClass: HID*/
    0x01,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
    0x01,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
    0,            /*iInterface: Index of string descriptor*/
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
    /******************** Descriptor of HID endpoint ********************/
    /* 27 */
    0x07,          /*bLength: Endpoint Descriptor size*/
    USB_ENDPOINT_DESCRIPTOR_TYPE, /*bDescriptorType:*/

    HID_IN_EP,     /*bEndpointAddress: Endpoint Address (IN)*/
    0x03,          /*bmAttributes: Interrupt endpoint*/
    HID_IN_PACKET, /*wMaxPacketSize: 4 Byte max */
    0x00,
    0x0A,          /*bInterval: Polling Interval (10 ms)*/
    /* 34 */
    
    0x07,          /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,	/* bDescriptorType: */
    /*    Endpoint descriptor type */
    HID_OUT_EP,   /* bEndpointAddress: */
    /*    Endpoint Address (OUT) */
    0x03, /* bmAttributes: Interrupt endpoint */
    HID_OUT_PACKET, /* wMaxPacketSize */
    0x00,
    0x20, /* bInterval: Polling Interval (10 ms) */
    /* 41 */
    /********************  Mass Storage interface ********************/
    0x09,   /* bLength: Interface Descriptor size */
    0x04,   /* bDescriptorType: */
    MSC_INTERFACE,   /* bInterfaceNumber: Number of Interface */
    0x00,   /* bAlternateSetting: Alternate setting */
    0x02,   /* bNumEndpoints*/
    0x08,   /* bInterfaceClass: MSC Class */
    0x06,   /* bInterfaceSubClass : SCSI transparent*/
    0x50,   /* nInterfaceProtocol */
    0x05,   /* iInterface: */
    /********************  Mass Storage Endpoints ********************/
    0x07,   /*Endpoint descriptor length = 7*/
    0x05,   /*Endpoint descriptor type */
    MSC_IN_EP,   /*Endpoint address (IN, address 1) */
    0x02,   /*Bulk endpoint type */
    LOBYTE(MSC_MAX_PACKET),
    HIBYTE(MSC_MAX_PACKET),
    0x00,   /*Polling interval in milliseconds */

    0x07,   /*Endpoint descriptor length = 7 */
    0x05,   /*Endpoint descriptor type */
    MSC_OUT_EP,   /*Endpoint address (OUT, address 1) */
    0x02,   /*Bulk endpoint type */
    LOBYTE(MSC_MAX_PACKET),
    HIBYTE(MSC_MAX_PACKET),
    0x00     /*Polling interval in milliseconds*/
} ;

/* Private function prototypes */

/*********************************************
   MSC Device library callbacks
*********************************************/
extern uint8_t  USBD_MSC_Init (void  *pdev, uint8_t cfgidx);
extern uint8_t  USBD_MSC_DeInit (void  *pdev, uint8_t cfgidx);
extern uint8_t  USBD_MSC_Setup (void  *pdev, USB_SETUP_REQ *req);
extern uint8_t  USBD_MSC_DataIn (void  *pdev, uint8_t epnum);
extern uint8_t  USBD_MSC_DataOut (void  *pdev,  uint8_t epnum);
extern uint8_t  *USBD_MSC_GetCfgDesc (uint8_t speed, uint16_t *length);
extern uint8_t  USBD_MSC_CfgDesc[USB_MSC_CONFIG_DESC_SIZ];

/*********************************************
   HID Device library callbacks
*********************************************/
extern uint8_t  USBD_CUSTOM_HID_Init (void  *pdev, uint8_t cfgidx);
extern uint8_t  USBD_CUSTOM_HID_DeInit (void  *pdev, uint8_t cfgidx);
extern uint8_t  USBD_CUSTOM_HID_Setup (void  *pdev, USB_SETUP_REQ *req);
extern uint8_t  *USBD_CUSTOM_HID_GetCfgDesc (uint8_t speed, uint16_t *length);
extern uint8_t  USBD_CUSTOM_HID_DataIn (void  *pdev, uint8_t epnum);
extern uint8_t  USBD_CUSTOM_HID_DataOut (void  *pdev, uint8_t epnum);
extern uint8_t  USBD_CUSTOM_HID_EP0_RxReady (void  *pdev);

/**
  * @brief  USBD_MSC_HID_Init
  *         Initialize the MSC-HID interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_MSC_HID_Init (void  *pdev, 
                                   uint8_t cfgidx)
{
  /* HID initialization */
  USBD_CUSTOM_HID_Init (pdev,cfgidx);
  
  /* MSC initialization */
  USBD_MSC_Init (pdev,cfgidx);
  
  return USBD_OK;
}

/**
  * @brief  USBD_MSC_HID_DeInit
  *         DeInitialize the MSC_HID layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_MSC_HID_DeInit (void  *pdev, 
                                     uint8_t cfgidx)
{
  /* HID De-initialization */
  USBD_CUSTOM_HID_DeInit (pdev,cfgidx);
  
  /* MSC De-initialization */
  USBD_MSC_DeInit (pdev,cfgidx);
  
  return USBD_OK;
}

/**
  * @brief  USBD_MSC_HID_Setup
  *         Handle the MSC_HID specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
static uint8_t  USBD_MSC_HID_Setup (void  *pdev, 
                                    USB_SETUP_REQ *req)
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
      return (USBD_MSC_Setup(pdev, req));
    }
    
  case USB_REQ_RECIPIENT_ENDPOINT:
    if (req->wIndex == HID_IN_EP)
    {
      return (USBD_CUSTOM_HID_Setup(pdev, req));   
    }
    else
    {
      return (USBD_MSC_Setup(pdev, req));
    }
  }   
  return USBD_OK;
}

/**
  * @brief  USBD_MSC_HID_GetCfgDesc 
  *         return configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t  *USBD_MSC_HID_GetCfgDesc (uint8_t speed, uint16_t *length)
{
  *length = sizeof (USBD_MSC_HID_CfgDesc);
  return USBD_MSC_HID_CfgDesc;
}

/**
  * @brief  USBD_MSC_HID_DataIn
  *         handle data IN Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_MSC_HID_DataIn (void  *pdev, 
                              uint8_t epnum)
{
    /*DataIN can be for MSC or HID */
    if (epnum == (MSC_IN_EP&~0x80) )
    {
        return (USBD_MSC_DataIn(pdev, epnum));
    }
    else
    {
        return (USBD_CUSTOM_HID_DataIn(pdev, epnum));
    }
}

/**
  * @brief  USBD_MSC_HID_DataOut
  *         handle data OUT Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_MSC_HID_DataOut(void *pdev , uint8_t epnum)
{
    /*DataOut can be for MSC*/
    if (epnum == (MSC_OUT_EP&~0x80) )
    {
        return (USBD_MSC_DataOut(pdev, epnum));
    }
    else
    {
        return (USBD_CUSTOM_HID_DataOut(pdev, epnum));
    }
}
