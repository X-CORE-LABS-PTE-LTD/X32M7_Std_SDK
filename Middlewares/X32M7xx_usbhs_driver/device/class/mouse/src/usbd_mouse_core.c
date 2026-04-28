/**
 ******************************************************************************
 * @file      usbd_mouse_core.c
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

/* Includes ------------------------------------------------------------------*/
#include "usbd_mouse_core.h"
#include "usbd_desc.h"
#include "usbd_req.h"
#include "x32m7xx_gpio.h"
#include "usbhs_bsp.h"

static uint8_t usb_dev_mouse_init(void *pdev, uint8_t cfgidx);
static uint8_t usb_dev_mouse_deinit(void *pdev, uint8_t cfgidx);
static uint8_t usb_dev_mouse_setup(void *pdev, USB_SETUP_REQ *req);
static uint8_t *usb_dev_mouse_getcfgdesc(uint8_t speed, uint16_t *length);
static uint8_t usb_dev_mouse_datain(void *pdev, uint8_t epnum);
static uint8_t usb_dev_mouse_dataout(void *pdev, uint8_t epnum);

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
USBD_Class_cb_TypeDef  USBD_MOUSE_cb = {
    usb_dev_mouse_init,
    usb_dev_mouse_deinit,
    usb_dev_mouse_setup,
    NULL,
    NULL,
    usb_dev_mouse_datain,
    usb_dev_mouse_dataout,
    NULL,
    NULL,
    NULL,
    usb_dev_mouse_getcfgdesc,
    usb_dev_mouse_getcfgdesc,
};

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
__ALIGN_BEGIN static uint32_t  USB_DEV_MOUSE_AltSet  = 0UL;
__ALIGN_BEGIN static uint32_t  USB_DEV_MOUSE_Protocol  = 0UL;
__ALIGN_BEGIN static uint32_t  USB_DEV_MOUSE_IdleState = 0UL;

/* USB HID device Configuration Descriptor */
__ALIGN_BEGIN static uint8_t USB_DEV_MOUSE_CfgDesc[MOUSE_CFGDESC_SIZE] = {
    0x09,                             /* bLength: Configuration Descriptor size */
    USB_CONFIGURATION_DESCRIPTOR_TYPE,/* bDescriptorType: Configuration */
    MOUSE_CFGDESC_SIZE,               /* wTotalLength: Bytes returned */
    0x00,
    0x01,                             /* bNumInterfaces: 1 interface */
    0x01,                             /* bConfigurationValue: Configuration value */
    0x00,                             /* iConfiguration: Index of string descriptor describing the configuration */
    0x80,                             /* bmAttributes: bus powered and Support Remote Wake-up  */
    0x32,                             /* MaxPower 100 mA: this current is used for detecting Vbus */
    /************** Descriptor of Joystick Mouse interface ****************/
    /* 09 */
    0x09,                             /* bLength: Interface Descriptor size */
    USB_INTERFACE_DESCRIPTOR_TYPE,    /* bDescriptorType: Interface descriptor type */
    0x00,                             /* bInterfaceNumber: Number of Interface */
    0x00,                             /* bAlternateSetting: Alternate setting */
    0x02,                             /* bNumEndpoints */
    0x03,                             /* bInterfaceClass: HID */
    0x01,                             /* bInterfaceSubClass : 1=BOOT, 0=no boot */
    0x02,                             /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
    0x00,                             /* iInterface: Index of string descriptor */
    /******************** Descriptor of Joystick Mouse HID ********************/
    /* 18 */
    0x09,                             /* bLength: HID Descriptor size */
    MOUSE_DESCRIPTOR_TYPE,            /* bDescriptorType: HID */
    0x11,                             /* bcdHID: HID Class Spec release number */
    0x01,
    0x00,                             /* bCountryCode: Hardware target country */
    0x01,                             /* bNumDescriptors: Number of HID class descriptors to follow */
    0x22,                             /* bDescriptorType */
    MOUSE_REPORT_DESC_SIZE,           /* wItemLength: Total length of Report descriptor */
    0x00,
    /******************** Descriptor of Mouse endpoint ********************/
    /* 27 */
    0x07,                             /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,     /* bDescriptorType: */
    HID_IN_EP,                        /* bEndpointAddress: Endpoint Address (IN) */
    0x03,                             /* bmAttributes: Interrupt endpoint */
    HID_IN_PACKET,                    /* wMaxPacketSize: 4 Byte max  */
    0x00,
    0x0A,                             /* bInterval: Polling Interval (10 ms) */
    /* 34 */
    0x07,                             /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,     /* bDescriptorType: */
    HID_OUT_EP,                       /* bEndpointAddress: Endpoint Address (IN) */
    0x03,                             /* bmAttributes: Interrupt endpoint */
    HID_OUT_PACKET,                   /* wMaxPacketSize: 4 Byte max  */
    0x00,
    0x0A,                             /* bInterval: Polling Interval (10 ms) */
    /* 41 */
} ;

__ALIGN_BEGIN static uint8_t HID_MOUSE_ReportDesc[MOUSE_REPORT_DESC_SIZE] = {
    0x05,   0x01,
    0x09,   0x02,
    0xA1,   0x01,
    0x09,   0x01,

    0xA1,   0x00,
    0x05,   0x09,
    0x19,   0x01,
    0x29,   0x03,

    0x15,   0x00,
    0x25,   0x01,
    0x95,   0x03,
    0x75,   0x01,

    0x81,   0x02,
    0x95,   0x01,
    0x75,   0x05,
    0x81,   0x01,

    0x05,   0x01,
    0x09,   0x30,
    0x09,   0x31,
    0x09,   0x38,

    0x15,   0x81,
    0x25,   0x7F,
    0x75,   0x08,
    0x95,   0x03,

    0x81,   0x06,
    0xC0,   0x09,
    0x3C,   0x05,
    0xFF,   0x09,

    0x01,   0x15,
    0x00,   0x25,
    0x01,   0x75,
    0x01,   0x95,

    0x02,   0xB1,
    0x22,   0x75,
    0x06,   0x95,
    0x01,   0xB1,

    0x01,   0xC0
};

static uint8_t u8HidRevBuf[4];

/**
*\*\name    usb_dev_mouse_init.
*\*\fun     Initialize the interface of MOUSE HID.
*\*\param   pdev: device instance.
*\*\param   cfgidx: Configuration index.
*\*\return  USBD_OK.
*\*\
**/
static uint8_t usb_dev_mouse_init(void *pdev, uint8_t cfgidx)
{
    USBDEV_EP_Open(pdev, HID_IN_EP, HID_IN_PACKET, USB_EP_INT);
    USBDEV_EP_Open(pdev, HID_OUT_EP, HID_OUT_PACKET, USB_EP_INT);
    USBDEV_EP_PrepareRx(pdev, HID_OUT_EP, u8HidRevBuf, (uint16_t)(sizeof(u8HidRevBuf) / sizeof(uint8_t)));
    USBDEV_SetEPStatus(pdev, HID_OUT_EP, USB_EP_RX_VALID);
    return USBD_OK;
}

/**
*\*\name    usb_dev_mouse_deinit.
*\*\fun     DeInitialize the interface of MOUSE HID.
*\*\param   pdev�� device instance.
*\*\param   cfgidx: cfg instance.
*\*\return  USBD_OK.
*\*\
**/
static uint8_t usb_dev_mouse_deinit(void *pdev, uint8_t cfgidx)
{
    USBDEV_EP_Close(pdev, HID_IN_EP);
    USBDEV_EP_Close(pdev, HID_OUT_EP);
    return USBD_OK;
}

/**
*\*\name    usb_dev_mouse_setup.
*\*\fun     Handle the MOUSE HID SETUP request.
*\*\param   pdev: device instance.
*\*\param   req: usb requests.
*\*\return  status.
*\*\
**/
static uint8_t usb_dev_mouse_setup(void *pdev, USB_SETUP_REQ *req)
{
    uint16_t len = 0U;
    uint8_t  *pbuf = NULL;
    uint8_t  u8Res = USBD_OK;

    switch (req->bmRequest & USB_REQ_TYPE_MASK) 
    {
        case USB_REQ_TYPE_CLASS :
            switch (req->bRequest) {
                case MOUSE_REQ_SET_PROTOCOL:
                    USB_DEV_MOUSE_Protocol = (uint8_t)(req->wValue);
                    break;
                case MOUSE_REQ_GET_PROTOCOL:
                    USBD_CtrlSendData(pdev, (uint8_t *)&USB_DEV_MOUSE_Protocol, 1U);
                    break;
                case MOUSE_REQ_SET_IDLE:
                    USB_DEV_MOUSE_IdleState = (uint8_t)(req->wValue >> 8U);
                    break;
                case MOUSE_REQ_GET_IDLE:
                    USBD_CtrlSendData(pdev, (uint8_t *)&USB_DEV_MOUSE_IdleState, 1U);
                    break;
                default:
                    USBD_CtrlError(pdev, req);
                    u8Res = USBD_FAIL;
                    break;
            }
            break;
        case USB_REQ_TYPE_STANDARD:
            switch (req->bRequest) 
            {
                case USB_REQ_GET_DESCRIPTOR:
                    if ((req->wValue >> 8U) == (uint16_t)MOUSE_REPORT_DESC) {
                        len  = (uint16_t)MIN(MOUSE_REPORT_DESC_SIZE, req->wLength);
                        pbuf = HID_MOUSE_ReportDesc;
                    } else if ((req->wValue >> 8U) == (uint16_t)MOUSE_DESCRIPTOR_TYPE) {
                        pbuf = USB_DEV_MOUSE_CfgDesc + 0x12;
                        len  = (uint16_t)MIN(MOUSE_DESC_SIZE, req->wLength);
                    } else {
                        ;
                    }
                    USBD_CtrlSendData(pdev, pbuf, len);
                    break;
                case USB_REQ_GET_INTERFACE :
                    USBD_CtrlSendData(pdev, (uint8_t *)&USB_DEV_MOUSE_AltSet, 1U);
                    break;
                case USB_REQ_SET_INTERFACE :
                    USB_DEV_MOUSE_AltSet = (uint8_t)(req->wValue);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    return u8Res;
}

/**
*\*\name    usb_dev_mouse_txreport.
*\*\fun     Send MOUSE HID report to the host.
*\*\param   pdev: device instance.
*\*\param   report: pointer to the report buffer
*\*\param   length: the length of the report buffer in bytes
*\*\return  USBD_OK.
*\*\
**/
uint8_t usb_dev_mouse_txreport(USB_CORE_MODULE *pdev, uint8_t *report, uint16_t length)
{
    if (pdev->dev.device_status == USB_CONFIGURED) 
    {
        USBDEV_EP_Tx(pdev, HID_IN_EP, report, (uint32_t)length);
    }
    return USBD_OK;
}

/**
*\*\name    usb_dev_mouse_getcfgdesc.
*\*\fun     get the configuration descriptor.
*\*\param   speed: device speed.
*\*\param   length:pointer data length of the configuration descriptor in bytes
*\*\return  pointer of configuration descriptor buffer.
*\*\
**/
static uint8_t *usb_dev_mouse_getcfgdesc(uint8_t speed, uint16_t *length)
{
    *length = (uint16_t)sizeof(USB_DEV_MOUSE_CfgDesc);
    return USB_DEV_MOUSE_CfgDesc;
}

/**
*\*\name    usb_dev_mouse_datain.
*\*\fun     handle the IN data Stage.
*\*\param   pdev: device instance.
*\*\param   epnum: endpoint index
*\*\return  USBD_OK.
*\*\
**/
static uint8_t usb_dev_mouse_datain(void *pdev, uint8_t epnum)
{
    USBDEV_EP_Flush(pdev, HID_IN_EP);
    return USBD_OK;
}

/**
*\*\name    usb_dev_mouse_dataout.
*\*\fun     handle the OUT data Stage.
*\*\param   pdev: device instance.
*\*\param   epnum: endpoint index
*\*\return  USBD_OK.
*\*\
**/
static uint8_t usb_dev_mouse_dataout(void *pdev, uint8_t epnum)
{
    USBDEV_EP_PrepareRx(pdev, HID_OUT_EP, u8HidRevBuf, (uint16_t)(sizeof(u8HidRevBuf) / sizeof(uint8_t)));
    USBDEV_SetEPStatus(pdev, HID_OUT_EP, USB_EP_RX_VALID);
    return USBD_OK;
}
