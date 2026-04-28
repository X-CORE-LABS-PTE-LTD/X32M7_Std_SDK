/**
 ******************************************************************************
 * @file      usbd_customhid_core.c
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
#include "usbd_desc.h"
#include "usbd_req.h"
#include "x32m7xx_gpio.h"
#include "usbhs_bsp.h"

uint8_t  USBD_CUSTOM_HID_Init (void  *pdev, uint8_t cfgidx);
uint8_t  USBD_CUSTOM_HID_DeInit (void  *pdev, uint8_t cfgidx);
uint8_t  USBD_CUSTOM_HID_Setup (void  *pdev, USB_SETUP_REQ *req);
uint8_t  *USBD_CUSTOM_HID_GetCfgDesc (uint8_t speed, uint16_t *length);
uint8_t  USBD_CUSTOM_HID_DataIn (void  *pdev, uint8_t epnum);
uint8_t  USBD_CUSTOM_HID_DataOut (void  *pdev, uint8_t epnum);
uint8_t  USBD_CUSTOM_HID_EP0_RxReady (void  *pdev);


USBD_Class_cb_TypeDef  USBD_CUSTOMHID_cb = 
{
    USBD_CUSTOM_HID_Init,
    USBD_CUSTOM_HID_DeInit,
    USBD_CUSTOM_HID_Setup,
    NULL, /*EP0_TxSent*/  
    USBD_CUSTOM_HID_EP0_RxReady, /*EP0_RxReady*/ /* STATUS STAGE IN */
    USBD_CUSTOM_HID_DataIn, /*DataIn*/
    USBD_CUSTOM_HID_DataOut,
    NULL, /*SOF */
    NULL, /*IsoINIncomplete*/
    NULL, /*IsoOUTIncomplete)*/
    USBD_CUSTOM_HID_GetCfgDesc,
    USBD_CUSTOM_HID_GetCfgDesc,
};

uint8_t Report_buf[4];
uint8_t USBD_HID_Report_ID=0;
__IO uint32_t IsReportAvailable = 0;
extern uint8_t PrevXferDone;

#ifdef USB_INTERNAL_DMA_ENABLED
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4 
#endif
#endif /* USB_INTERNAL_DMA_ENABLED */ 
__ALIGN_BEGIN uint8_t Report_buf[4] __ALIGN_END;

#ifdef USB_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_INTERNAL_DMA_ENABLED */        
__ALIGN_BEGIN static uint32_t  USBD_HID_AltSet  __ALIGN_END = 0;

#ifdef USB_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_INTERNAL_DMA_ENABLED */      
__ALIGN_BEGIN static uint32_t  USBD_HID_Protocol  __ALIGN_END = 0;

#ifdef USB_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_INTERNAL_DMA_ENABLED */  
__ALIGN_BEGIN static uint32_t  USBD_HID_IdleState __ALIGN_END = 0;

#ifdef USB_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_INTERNAL_DMA_ENABLED */ 
/* USB HID device Configuration Descriptor */
__ALIGN_BEGIN static uint8_t USBD_CUSTOM_HID_CfgDesc[USB_CUSTOM_HID_CONFIG_DESC_SIZ] __ALIGN_END =
{
    0x09, /* bLength: Configuration Descriptor size */
    USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */
    USB_CUSTOM_HID_CONFIG_DESC_SIZ,
    /* wTotalLength: Bytes returned */
    0x00,
    0x01,         /*bNumInterfaces: 1 interface*/
    0x01,         /*bConfigurationValue: Configuration value*/
    0x00,         /*iConfiguration: Index of string descriptor describing
    the configuration*/
    0xC0,         /*bmAttributes: bus powered and Support Remote Wake-up */
    0x32,         /*MaxPower 100 mA: this current is used for detecting Vbus*/

    /************** Descriptor of Custom HID interface ****************/
    /* 09 */
    0x09,         /*bLength: Interface Descriptor size*/
    USB_INTERFACE_DESCRIPTOR_TYPE,/*bDescriptorType: Interface descriptor type*/
    0x00,         /*bInterfaceNumber: Number of Interface*/
    0x00,         /*bAlternateSetting: Alternate setting*/
    0x02,         /*bNumEndpoints*/
    0x03,         /*bInterfaceClass: HID*/
    0x00,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
    0x00,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
    0x00,         /*iInterface: Index of string descriptor*/
    /******************** Descriptor of Custom HID ********************/
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
    /******************** Descriptor of Custom HID endpoints ***********/
    /* 27 */
    0x08,          /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: */

    HID_IN_EP,     /* bEndpointAddress: Endpoint Address (IN) */
    0x03,          /* bmAttributes: Interrupt endpoint */
    LOBYTE(HID_IN_PACKET),    /* wMaxPacketSize: */
    HIBYTE(HID_IN_PACKET),
    0x00,
    0x01,          /* bInterval: Polling Interval (1 ms) */
    /* 34 */

    0x08,          /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,	/* bDescriptorType: */
    /*    Endpoint descriptor type */
    HID_OUT_EP,   /* bEndpointAddress: */
    /*    Endpoint Address (OUT) */
    0x03, /* bmAttributes: Interrupt endpoint */
    LOBYTE(HID_OUT_PACKET),    /* wMaxPacketSize: */
    HIBYTE(HID_OUT_PACKET),
    0x00,
    0x10, /* bInterval: Polling Interval (16 ms) */
    /* 41 */
} ;

#ifdef USB_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif 

__ALIGN_BEGIN static uint8_t CustomHID_ReportDesc[USBD_CUSTOM_HID_REPORT_DESC_SIZE] __ALIGN_END =
{
    0x06, 0xFF, 0x00,      /* USAGE_PAGE (Vendor Page: 0xFF00) */
    0x09, 0x01,            /* USAGE (Demo Kit)               */
    0xa1, 0x01,            /* COLLECTION (Application)       */
    /* 6 */
    
    /* Led 1 */        
    0x85, 0x01,            /*     REPORT_ID (1)              */
    0x09, 0x01,            /*     USAGE (LED 1)              */
    0x15, 0x00,            /*     LOGICAL_MINIMUM (0)        */
    0x25, 0x01,            /*     LOGICAL_MAXIMUM (1)        */
    0x75, 0x08,            /*     REPORT_SIZE (8)            */
    0x96, 0x00, 0x0C,            /*     REPORT_COUNT (1)           */
    0xB1, 0x82,             /*    FEATURE (Data,Var,Abs,Vol) */

    0x85, 0x01,            /*     REPORT_ID (1)              */
    0x09, 0x01,            /*     USAGE (LED 1)              */
    0x91, 0x82,            /*     OUTPUT (Data,Var,Abs,Vol)  */
    /* 26 */
    
    /* Led 2 */
    0x85, 0x02,            /*     REPORT_ID 2               a */
    0x09, 0x02,            /*     USAGE (LED 2)              */
    0x15, 0x00,            /*     LOGICAL_MINIMUM (0)        */
    0x25, 0x01,            /*     LOGICAL_MAXIMUM (1)        */
    0x75, 0x08,            /*     REPORT_SIZE (8)            */
    0x96, 0x00, 0x0C,            /*     REPORT_COUNT (1)           */
    0xB1, 0x82,             /*    FEATURE (Data,Var,Abs,Vol) */

    0x85, 0x02,            /*     REPORT_ID (2)              */
    0x09, 0x02,            /*     USAGE (LED 2)              */
    0x91, 0x82,            /*     OUTPUT (Data,Var,Abs,Vol)  */
    /* 46 */
    
    /* Led 3 */        
    0x85, 0x03,            /*     REPORT_ID (3)              */
    0x09, 0x03,            /*     USAGE (LED 3)              */
    0x15, 0x00,            /*     LOGICAL_MINIMUM (0)        */
    0x25, 0x01,            /*     LOGICAL_MAXIMUM (1)        */
    0x75, 0x08,            /*     REPORT_SIZE (8)            */
    0x96, 0x00, 0x0C,            /*     REPORT_COUNT (1)           */
    0xB1, 0x82,             /*    FEATURE (Data,Var,Abs,Vol) */

    0x85, 0x03,            /*     REPORT_ID (3)              */
    0x09, 0x03,            /*     USAGE (LED 3)              */
    0x91, 0x82,            /*     OUTPUT (Data,Var,Abs,Vol)  */
    /* 66 */
    
    /* Led 4 */
    0x85, 0x04,            /*     REPORT_ID (4)               */
    0x09, 0x04,            /*     USAGE (LED 4)              */
    0x15, 0x00,            /*     LOGICAL_MINIMUM (0)        */
    0x25, 0x01,            /*     LOGICAL_MAXIMUM (1)        */
    0x75, 0x08,            /*     REPORT_SIZE (8)            */
    0x96, 0x00, 0x0C,            /*     REPORT_COUNT (1)           */
    0xB1, 0x82,            /*     FEATURE (Data,Var,Abs,Vol) */

    0x85, 0x04,            /*     REPORT_ID (4)              */
    0x09, 0x04,            /*     USAGE (LED 4)              */
    0x91, 0x82,            /*     OUTPUT (Data,Var,Abs,Vol)  */
    /* 86 */
    
    /* key Push Button */  
    0x85, 0x05,            /*     REPORT_ID (5)              */
    0x09, 0x05,            /*     USAGE (Push Button)        */
    0x15, 0x00,            /*     LOGICAL_MINIMUM (0)        */
    0x25, 0x01,            /*     LOGICAL_MAXIMUM (1)        */
    0x75, 0x02,            /*     REPORT_SIZE (1)            */
    0x96, 0x00, 0x0C,            /*     REPORT_COUNT (1)           */
    0x81, 0x82,            /*     INPUT (Data,Var,Abs,Vol)   */
    
    0x09, 0x05,            /*     USAGE (Push Button)        */
    0x75, 0x02,            /*     REPORT_SIZE (1)            */
    0x96, 0x00, 0x0C,            /*     REPORT_COUNT (1)           */
    0xb1, 0x82,            /*     FEATURE (Data,Var,Abs,Vol) */
         
    0x75, 0x07,            /*     REPORT_SIZE (7)            */
    0x81, 0x83,            /*     INPUT (Cnst,Var,Abs,Vol)   */
    0x85, 0x05,            /*     REPORT_ID (2)              */
                    
    0x75, 0x07,            /*     REPORT_SIZE (7)            */
    0xb1, 0x83,            /*     FEATURE (Cnst,Var,Abs,Vol) */
    /* 114 */

    0xc0                   /*     END_COLLECTION            */
}; 

/**
*\*\name    USBD_CUSTOM_HID_Init.
*\*\fun     Initialize the HID interface.
*\*\param   pdev: device instance.
*\*\param   cfgidx: Configuration index
*\*\return  USBD_OK.
*\*\
**/
uint8_t  USBD_CUSTOM_HID_Init (void *pdev, uint8_t cfgidx)
{
    /* Open EP IN */
    USBDEV_EP_Open(pdev, HID_IN_EP, HID_IN_PACKET, USB_EP_INT);
    /* Open EP OUT */
    USBDEV_EP_Open(pdev, HID_OUT_EP, HID_OUT_PACKET, USB_EP_INT);
    /*Receive Data*/
    USBDEV_EP_PrepareRx(pdev, HID_OUT_EP, Report_buf, 2);

    return USBD_OK;
}

/**
*\*\name    USBD_CUSTOM_HID_DeInit.
*\*\fun     DeInitialize the HID layer.
*\*\param   pdev: device instance.
*\*\param   cfgidx: Configuration index
*\*\return  USBD_OK.
*\*\
**/
uint8_t  USBD_CUSTOM_HID_DeInit (void  *pdev, uint8_t cfgidx)
{
    /* Close HID EPs */
    USBDEV_EP_Close (pdev , HID_IN_EP);
    USBDEV_EP_Close (pdev , HID_OUT_EP);
    return USBD_OK;
}

/**
*\*\name    USBD_CUSTOM_HID_Setup.
*\*\fun     Handle the HID specific requests.
*\*\param   pdev: device instance.
*\*\param   req: usb requests
*\*\return  USBD_OK.
*\*\
**/
uint8_t  USBD_CUSTOM_HID_Setup (void  *pdev, USB_SETUP_REQ *req)
{
    uint8_t USBD_HID_Report_LENGTH=0;
    uint16_t len = 0;
    uint8_t  *pbuf = NULL;

    switch (req->bmRequest & USB_REQ_TYPE_MASK)
    {
        case USB_REQ_TYPE_CLASS :
            switch (req->bRequest)
            {
                case CUSTOM_HID_REQ_SET_PROTOCOL:
                    USBD_HID_Protocol = (uint8_t)(req->wValue);
                    break;
                  
                case CUSTOM_HID_REQ_GET_PROTOCOL:
                    USBD_CtrlSendData(pdev, (uint8_t *)&USBD_HID_Protocol, 1);
                    break;
                  
                case CUSTOM_HID_REQ_SET_IDLE:
                    USBD_HID_IdleState = (uint8_t)(req->wValue >> 8);
                    break;
                  
                case CUSTOM_HID_REQ_GET_IDLE:
                    USBD_CtrlSendData(pdev, (uint8_t *)&USBD_HID_IdleState, 1);
                    break;

                case CUSTOM_HID_REQ_SET_REPORT:
                    IsReportAvailable = 1;
                    USBD_HID_Report_ID = (uint8_t)(req->wValue);
                    USBD_HID_Report_LENGTH = (uint8_t)(req->wLength);
                    USBD_CtrlPrepareRx (pdev, Report_buf, USBD_HID_Report_LENGTH);
                    break;
                  
                default:
                    USBD_CtrlError (pdev, req);
                    return USBD_FAIL; 
            }
        break;

        case USB_REQ_TYPE_STANDARD:
            switch (req->bRequest)
            {
                case USB_REQ_GET_DESCRIPTOR: 
                    if( req->wValue >> 8 == CUSTOM_HID_REPORT_DESC)
                    {
                        len = MIN(USBD_CUSTOM_HID_REPORT_DESC_SIZE , req->wLength);
                        pbuf = (uint8_t*)CustomHID_ReportDesc;
                    }
                    else if( req->wValue >> 8 == CUSTOM_HID_DESCRIPTOR_TYPE)
                    {
                        pbuf = (uint8_t*)USBD_CUSTOM_HID_CfgDesc + 0x12;
                        len = MIN(USB_CUSTOM_HID_DESC_SIZ , req->wLength);
                    }
                  
                    USBD_CtrlSendData (pdev, pbuf, len);
                    break;

                case USB_REQ_GET_INTERFACE :
                    USBD_CtrlSendData (pdev, (uint8_t *)&USBD_HID_AltSet, 1);
                    break;

                case USB_REQ_SET_INTERFACE :
                    USBD_HID_AltSet = (uint8_t)(req->wValue);
                    break;
                default:
                    break;
            }
    }
    return USBD_OK;
}

/**
*\*\name    USBD_CUSTOM_HID_SendReport.
*\*\fun     Send HID Report.
*\*\param   pdev: device instance.
*\*\param   report: pointer to report.
*\*\param   len: length of report.
*\*\return  USBD_OK.
*\*\
**/
uint8_t USBD_CUSTOM_HID_SendReport (USB_CORE_MODULE  *pdev, uint8_t *report, uint16_t len)
{
    if (pdev->dev.device_status == USB_CONFIGURED)
    {
        USBDEV_EP_Tx(pdev, HID_IN_EP, report, len);
    }
    return USBD_OK;
}

/**
*\*\name    USBD_CUSTOM_HID_GetCfgDesc.
*\*\fun     return configuration descriptor.
*\*\param   speed : current device speed.
*\*\param   length : pointer data length.
*\*\return  pointer to descriptor buffer.
*\*\
**/
uint8_t  *USBD_CUSTOM_HID_GetCfgDesc (uint8_t speed, uint16_t *length)
{
    *length = sizeof (USBD_CUSTOM_HID_CfgDesc);
    return USBD_CUSTOM_HID_CfgDesc;
}

/**
*\*\name    USBD_CUSTOM_HID_DataIn.
*\*\fun     handle data IN Stage.
*\*\param   pdev: device instance.
*\*\param   epnum: endpoint index.
*\*\return  USBD_OK.
*\*\
**/
uint8_t USBD_CUSTOM_HID_DataIn (void *pdev, uint8_t epnum)
{
    /* Ensure that the FIFO is empty before a new transfer, this condition could 
    be caused by  a new transfer before the end of the previous transfer */
    USBDEV_EP_Flush(pdev, HID_IN_EP);

    if (epnum == (HID_IN_EP & 0x0F)) 
    {
        PrevXferDone = 1;
    }

    return USBD_OK;
}

/**
*\*\name    USBD_CUSTOM_HID_DataOut.
*\*\fun     handle data OUT Stage.
*\*\param   pdev: device instance.
*\*\param   epnum: endpoint index.
*\*\return  USBD_OK.
*\*\
**/
uint8_t USBD_CUSTOM_HID_DataOut (void  *pdev, uint8_t epnum)
{
    Bit_OperateType Led_State;
    if (epnum == HID_OUT_EP) 
    {
        if (Report_buf[1] == 0u)
        {
            Led_State = Bit_RESET;
        }
        else
        {
            Led_State = Bit_SET;
        }
        switch (Report_buf[0])
        {
        case 1: /* Led 1 */
          if (Led_State != Bit_RESET)
          {
            LED_On(LED1_PORT, LED1_PIN);
          }
          else
          {
            LED_Off(LED1_PORT, LED1_PIN);
          }
          break;
          
        case 2: /* Led 2 */
          if (Led_State != Bit_RESET)
          {
            LED_On(LED2_PORT, LED2_PIN);
          }
          else
          {
            LED_Off(LED2_PORT, LED2_PIN);
          }
          break;
        case 3: /* Led 3 */
          if (Led_State != Bit_RESET)
          {
            LED_On(LED3_PORT, LED3_PIN);
          }
          else
          {
            LED_Off(LED3_PORT, LED3_PIN);
          }
          break;
        default:
          LED_Off(LED1_PORT, LED1_PIN);
          LED_Off(LED2_PORT, LED2_PIN);
          LED_Off(LED3_PORT, LED3_PIN);
          break;
        }
    }

    USBDEV_EP_PrepareRx(pdev, HID_OUT_EP,Report_buf,2);
    USBDEV_SetEPStatus(pdev, HID_OUT_EP, USB_EP_RX_VALID);

    return USBD_OK;
}

/**
*\*\name    USBD_CUSTOM_HID_EP0_RxReady.
*\*\fun     Handles control request data.
*\*\param   pdev: device instance.
*\*\param   epnum: endpoint index.
*\*\return  USBD_OK.
*\*\
**/
uint8_t USBD_CUSTOM_HID_EP0_RxReady(void *pdev)
{
    Bit_OperateType Led_State;

    if (IsReportAvailable == 1)
    {
        IsReportAvailable = 0;
        if (Report_buf[1] == 0)
        {
            Led_State = Bit_RESET;
        }
        else 
        {
            Led_State = Bit_SET;
        }

        switch (Report_buf[0])
        {
            case 1: /* Led 1 */
                if (Led_State != Bit_RESET)
                {
                    LED_On(LED1_PORT, LED1_PIN);
                }
                else
                {
                    LED_Off(LED1_PORT, LED1_PIN);
                }
            break;

            case 2: /* Led 2 */
                if (Led_State != Bit_RESET)
                {
                    LED_On(LED2_PORT, LED2_PIN);
                }
                else
                {
                    LED_Off(LED2_PORT, LED2_PIN);
                }
            break;
            case 3: /* Led 3 */
                if (Led_State != Bit_RESET)
                {
                    LED_On(LED3_PORT, LED3_PIN);
                }
                else
                {
                    LED_Off(LED3_PORT, LED3_PIN);
                }
            break;
            default:
                LED_Off(LED1_PORT, LED1_PIN);
                LED_Off(LED2_PORT, LED2_PIN);
                LED_Off(LED3_PORT, LED3_PIN);
            break;
        }
    }
    return USBD_OK;
}
