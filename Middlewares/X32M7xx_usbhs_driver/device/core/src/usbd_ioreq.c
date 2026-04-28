/**
 ******************************************************************************
 * @file      usbd_ioreq.c
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

#include "usbd_ioreq.h"

#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif


/**
*\*\name   USBD_CtrlSendData.
*\*\fun    send data on the ctrl pipe
*\*\param  USBx : USB device
*\*\param  pbuf: pointer to data buffer
*\*\param  len: length of data to be sent
*\*\return USBD_OK
**/
USBD_Status USBD_CtrlSendData (USB_CORE_MODULE *USBx, uint8_t *pbuf, uint16_t len)
{
    USBD_Status ret = USBD_OK;

    USBx->dev.in_ep[0].total_data_len = len;
    USBx->dev.in_ep[0].rem_data_len   = len;
    USBx->dev.device_state = USB_EP0_DATA_IN;

    USBDEV_EP_Tx (USBx, 0, pbuf, len);

    return ret;
}

/**
*\*\name   USBD_CtrlContinueSendData.
*\*\fun    continue sending data on the ctrl pipe
*\*\param  USBx : USB device
*\*\param  pbuf: pointer to data buffer
*\*\param  len: length of data to be sent
*\*\return USBD_OK
**/
USBD_Status USBD_CtrlContinueSendData (USB_CORE_MODULE *USBx, uint8_t *pbuf, uint16_t len)
{
    USBD_Status ret = USBD_OK;

    USBDEV_EP_Tx (USBx, 0, pbuf, len);

    return ret;
}

/**
*\*\name   USBD_CtrlPrepareRx.
*\*\fun    receive data on the ctrl pipe
*\*\param  USBx : USB device
*\*\param  pbuf: pointer to data buffer
*\*\param  len: length of data to be sent
*\*\return USBD_OK
**/
USBD_Status USBD_CtrlPrepareRx (USB_CORE_MODULE *USBx, uint8_t *pbuf, uint16_t len)
{
    USBD_Status ret = USBD_OK;

    USBx->dev.out_ep[0].total_data_len = len;
    USBx->dev.out_ep[0].rem_data_len   = len;
    USBx->dev.device_state = USB_EP0_DATA_OUT;

    USBDEV_EP_PrepareRx (USBx, 0, pbuf, len);

    return ret;
}

/**
*\*\name   USBD_CtrlContinueRx.
*\*\fun    continue receive data on the ctrl pipe
*\*\param  USBx : USB device
*\*\param  pbuf: pointer to data buffer
*\*\param  len: length of data to be sent
*\*\return USBD_OK
**/
USBD_Status USBD_CtrlContinueRx (USB_CORE_MODULE *USBx, uint8_t *pbuf, uint16_t len)
{
    USBD_Status ret = USBD_OK;

    USBDEV_EP_PrepareRx (USBx, 0, pbuf, len);
    return ret;
}

/**
*\*\name   USBD_CtrlSendStatus.
*\*\fun    send zero length packet on the ctrl pipe
*\*\param  USBx : USB device
*\*\return USBD_OK
**/
USBD_Status USBD_CtrlSendStatus (USB_CORE_MODULE *USBx)
{
    USBD_Status ret = USBD_OK;
    USBx->dev.device_state = USB_EP0_STATUS_IN;
    USBDEV_EP_Tx (USBx, 0, USBx->dev.out_ep[0].xfer_buff, 0); 

    USB_EP0_OutStart(USBx);

    return ret;
}

/**
*\*\name   USBD_CtrlReceiveStatus.
*\*\fun    receive zero length packet on the ctrl pipe
*\*\param  USBx : USB device
*\*\return USBD_OK
**/
USBD_Status USBD_CtrlReceiveStatus (USB_CORE_MODULE *USBx)
{
    USBD_Status ret = USBD_OK;
    USBx->dev.device_state = USB_EP0_STATUS_OUT;
    USBDEV_EP_PrepareRx ( USBx, 0, USBx->dev.setup_packet, 0);

    USB_EP0_OutStart(USBx);

    return ret;
}

/**
*\*\name   USBD_GetRxCount.
*\*\fun    returns the received data length
*\*\param  USBx : USB device
*\*\param  epnum: endpoint index
*\*\return USBD_OK
**/
uint16_t  USBD_GetRxCount (USB_CORE_MODULE *USBx , uint8_t epnum)
{
    return USBx->dev.out_ep[epnum].xfer_count;
}

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

