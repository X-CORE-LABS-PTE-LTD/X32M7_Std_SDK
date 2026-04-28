/**
 ******************************************************************************
 * @file      usbd_core.c
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

#include "usbd_core.h"
#include "usbd_req.h"
#include "usbd_ioreq.h"
#include "usbhs_dcd_int.h"
#include "usbhs_bsp.h"

#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

static uint8_t USBD_SetupStage(USB_CORE_MODULE *USBx);
static uint8_t USBD_DataOutStage(USB_CORE_MODULE *USBx , uint8_t epnum);
static uint8_t USBD_DataInStage(USB_CORE_MODULE *USBx , uint8_t epnum);
static uint8_t USBD_SOF(USB_CORE_MODULE *USBx);
static uint8_t USBD_Reset(USB_CORE_MODULE *USBx);
static uint8_t USBD_Suspend(USB_CORE_MODULE *USBx);
static uint8_t USBD_Resume(USB_CORE_MODULE *USBx);
static uint8_t USBD_IsoINIncomplete(USB_CORE_MODULE *USBx);
static uint8_t USBD_IsoOUTIncomplete(USB_CORE_MODULE *USBx);
static uint8_t USBD_RunTestMode (USB_CORE_MODULE *USBx) ;

static uint8_t USBD_DevConnected(USB_CORE_MODULE  *pdev);
static uint8_t USBD_DevDisconnected(USB_CORE_MODULE  *pdev);



USBD_DEV_INT_cb_TypeDef USBD_DEV_INT_cb = 
{
    USBD_DataOutStage,
    USBD_DataInStage,
    USBD_SetupStage,
    USBD_SOF,
    USBD_Reset,
    USBD_Suspend,
    USBD_Resume,
    USBD_IsoINIncomplete,
    USBD_IsoOUTIncomplete,
    USBD_DevConnected, 
    USBD_DevDisconnected,
};

USBD_DEV_INT_cb_TypeDef  *USBD_DEV_INT_fops = &USBD_DEV_INT_cb;

uint32_t SET_TEST_MODE = 0;

/**
*\*\name   USBD_Init.
*\*\fun    Initializes the device stack and load the class driver.
*\*\param  USBx : USB device
*\*\param  pDevice: device instance
*\*\param  class_cb: Class callback structure address
*\*\param  user_cb: User callback structure address
*\*\return none
**/
void USBD_Init(USB_CORE_MODULE *USBx, USB_CORE_ID_TypeDef coreID, USBD_DEVICE_DESC *pDevice, USBD_Class_cb_TypeDef *class_cb, USBD_User_cb_TypeDef *user_cb)
{
    /* Hardware Init */
    USB_BSP_Init();

    /*Register class and user callbacks */
    USBx->dev.class_cb = class_cb;
    USBx->dev.user_cb = user_cb;
    USBx->dev.user_device = pDevice;

    /* Initializes the USB core*/
    USBDEV_CoreInit(USBx, coreID);

    /* Upon Init call user callback */
    USBx->dev.user_cb->Init();

    /* Enable Interrupts */
    USB_BSP_EnableInterrupt(USBx, coreID);
}

/**
*\*\name   USBD_DeInit.
*\*\fun    Re-Initialize the device library
*\*\param  USBx : USB device
*\*\return USBD_OK
**/
USBD_Status USBD_DeInit(USB_CORE_MODULE *USBx)
{
    /* Software Init */
    return USBD_OK;
}

/**
*\*\name   USBD_SetupStage.
*\*\fun    Handle the setup stage
*\*\param  USBx : USB device
*\*\return USBD_OK
**/
static uint8_t USBD_SetupStage(USB_CORE_MODULE *USBx)
{
    USB_SETUP_REQ req;

    USBD_ParseSetupRequest(USBx , &req);

    switch (req.bmRequest & 0x1F) 
    {
        case USB_REQ_RECIPIENT_DEVICE:   
            USBD_StdDevReq (USBx, &req);
        break;

        case USB_REQ_RECIPIENT_INTERFACE:     
            USBD_StdItfReq(USBx, &req);
        break;

        case USB_REQ_RECIPIENT_ENDPOINT:        
            USBD_StdEPReq(USBx, &req);   
        break;

        default:
            USBDEV_EP_Stall(USBx , req.bmRequest & 0x80U);
        break;
    }  
    return USBD_OK;
}

/**
*\*\name   USBD_DataOutStage.
*\*\fun    Handle data out stage
*\*\param  USBx : USB device
*\*\param  epnum: endpoint index
*\*\return USBD_OK
**/
static uint8_t USBD_DataOutStage(USB_CORE_MODULE *USBx , uint8_t epnum)
{
    USB_EP *ep;

    if(epnum == 0) 
    {
        ep = &USBx->dev.out_ep[0];
        if ( USBx->dev.device_state == USB_EP0_DATA_OUT)
        {
            if(ep->rem_data_len > ep->maxpacket)
            {
                ep->rem_data_len -=  ep->maxpacket;

                if(USBx->cfg.dma_enable == 1)
                {
                    /* in slave mode this, is handled by the RxSTSQLvl ISR */
                    ep->xfer_buff += ep->maxpacket; 
                }
                USBD_CtrlContinueRx (USBx, ep->xfer_buff, MIN(ep->rem_data_len ,ep->maxpacket));
            }
            else
            {
                if((USBx->dev.class_cb->EP0_RxReady != NULL) && (USBx->dev.device_status == USB_CONFIGURED))
                {
                    USBx->dev.class_cb->EP0_RxReady(USBx); 
                }
                USBD_CtrlSendStatus(USBx);
            }
        }
    }
    else if((USBx->dev.class_cb->DataOut != NULL) && (USBx->dev.device_status == USB_CONFIGURED))
    {
        USBx->dev.class_cb->DataOut(USBx, epnum); 
    } 

    else
    {
        /* Do Nothing */
    }
    return USBD_OK;
}

/**
*\*\name   USBD_DataInStage.
*\*\fun    Handle data in stage
*\*\param  USBx : USB device
*\*\param  epnum: endpoint index
*\*\return USBD_OK
**/
static uint8_t USBD_DataInStage(USB_CORE_MODULE *USBx , uint8_t epnum)
{
    USB_EP *ep;

    if(epnum == 0) 
    {
        ep = &USBx->dev.in_ep[0];
        if ( USBx->dev.device_state == USB_EP0_DATA_IN)
        {
            if(ep->rem_data_len > ep->maxpacket)
            {
                ep->rem_data_len -=  ep->maxpacket;
                if(USBx->cfg.dma_enable == 1)
                {
                    /* in slave mode this, is handled by the TxFifoEmpty ISR */
                    ep->xfer_buff += ep->maxpacket;
                }
                USBD_CtrlContinueSendData (USBx, ep->xfer_buff, ep->rem_data_len);

                /* Start the transfer */  
                USBDEV_EP_PrepareRx (USBx, 0, ep->xfer_buff, 0);
            }
            else
            { /* last packet is MPS multiple, so send ZLP packet */
                if((ep->total_data_len % ep->maxpacket == 0) &&(ep->total_data_len >= ep->maxpacket) && (ep->total_data_len < ep->ctrl_data_len ))
                {
                    USBD_CtrlContinueSendData(USBx , ep->xfer_buff, 0);
                    ep->ctrl_data_len = 0;
                    /* Start the transfer */  
                    USBDEV_EP_PrepareRx (USBx, 0, ep->xfer_buff, 0);
                }
                else
                {
                    if((USBx->dev.class_cb->EP0_TxSent != NULL) && (USBx->dev.device_status == USB_CONFIGURED))
                    {
                        USBx->dev.class_cb->EP0_TxSent(USBx); 
                    }          
                    USBD_CtrlReceiveStatus(USBx);
                }
            }
        }
        if (USBx->dev.test_mode == 1)
        {
            USBD_RunTestMode(USBx); 
            USBx->dev.test_mode = 0;
        }
    }
    else if((USBx->dev.class_cb->DataIn != NULL) && (USBx->dev.device_status == USB_CONFIGURED))
    {
        USBx->dev.class_cb->DataIn(USBx, epnum); 
    } 

    else
    {
        /* Do Nothing */
    }
    return USBD_OK;
}

/**
*\*\name   USBD_RunTestMode.
*\*\fun    Launch test mode process
*\*\param  USBx : USB device
*\*\return USBD_OK
**/
static uint8_t USBD_RunTestMode (USB_CORE_MODULE *USBx) 
{
    USB_WRITE_REG32(&USBx->regs.DCSR->DCTRL, SET_TEST_MODE);
    return USBD_OK;  
}

/**
*\*\name   USBD_Reset.
*\*\fun    Handle Reset event
*\*\param  USBx : USB device
*\*\return USBD_OK
**/
static uint8_t USBD_Reset(USB_CORE_MODULE *USBx)
{
    /* Open EP0 OUT */
    USBDEV_EP_Open(USBx, 0x00, USB_MAX_EP0_SIZE, EP_TYPE_CTRL);
    /* Open EP0 IN */
    USBDEV_EP_Open(USBx, 0x80, USB_MAX_EP0_SIZE, EP_TYPE_CTRL);

    /* Upon Reset call user call back */
    USBx->dev.device_status = USB_DEFAULT;
    USBx->dev.user_cb->DeviceReset(USBx->cfg.speed);

    return USBD_OK;
}

/**
*\*\name   USBD_Resume.
*\*\fun    Handle Resume event
*\*\param  USBx : USB device
*\*\return USBD_OK
**/
static uint8_t USBD_Resume(USB_CORE_MODULE *USBx)
{
    /* Upon Resume call user call back */
    USBx->dev.user_cb->DeviceResumed(); 
    USBx->dev.device_status = USBx->dev.device_old_status;  
    USBx->dev.device_status = USB_CONFIGURED;  
    return USBD_OK;
}

/**
*\*\name   USBD_Suspend.
*\*\fun    Handle Suspend event
*\*\param  USBx : USB device
*\*\return USBD_OK
**/
static uint8_t USBD_Suspend(USB_CORE_MODULE *USBx)
{
    USBx->dev.device_old_status = USBx->dev.device_status;
    USBx->dev.device_status  = USB_SUSPENDED;
    /* Upon Resume call user call back */
    USBx->dev.user_cb->DeviceSuspended(); 
    return USBD_OK;
}

/**
*\*\name   USBD_SOF.
*\*\fun    Handle SOF event
*\*\param  USBx : USB device
*\*\return USBD_OK
**/
static uint8_t USBD_SOF(USB_CORE_MODULE *USBx)
{
    if(USBx->dev.class_cb->SOF)
    {
        USBx->dev.class_cb->SOF(USBx); 
    }
    return USBD_OK;
}

/**
*\*\name   USBD_SetCfg.
*\*\fun    Configure device and start the interface
*\*\param  USBx : USB device
*\*\param  cfgidx: configuration index
*\*\return USBD_OK
**/
USBD_Status USBD_SetCfg(USB_CORE_MODULE *USBx, uint8_t cfgidx)
{
    USBx->dev.class_cb->Init(USBx, cfgidx); 

    /* Upon set config call user call back */
    USBx->dev.user_cb->DeviceConfigured();
    return USBD_OK; 
}

/**
*\*\name   USBD_ClrCfg.
*\*\fun    Clear current configuration
*\*\param  USBx : USB device
*\*\param  cfgidx: configuration index
*\*\return USBD_OK
**/
USBD_Status USBD_ClrCfg(USB_CORE_MODULE *USBx, uint8_t cfgidx)
{
    USBx->dev.class_cb->DeInit(USBx, cfgidx);
    return USBD_OK;
}

/**
*\*\name   USBD_IsoINIncomplete.
*\*\fun    Handle iso in incomplete event
*\*\param  USBx : USB device
*\*\return USBD_OK
**/
static uint8_t USBD_IsoINIncomplete(USB_CORE_MODULE *USBx)
{
    USBx->dev.class_cb->IsoINIncomplete(USBx);   
    return USBD_OK;
}

/**
*\*\name   USBD_IsoOUTIncomplete.
*\*\fun    Handle iso out incomplete event
*\*\param  USBx : USB device
*\*\return USBD_OK
**/
static uint8_t USBD_IsoOUTIncomplete(USB_CORE_MODULE *USBx)
{
    USBx->dev.class_cb->IsoOUTIncomplete(USBx);   
    return USBD_OK;
}

/**
*\*\name   USBD_DevConnected.
*\*\fun    Handle device connection event
*\*\param  USBx : USB device
*\*\return USBD_OK
**/
static uint8_t USBD_DevConnected(USB_CORE_MODULE *USBx)
{
    USBx->dev.user_cb->DeviceConnected();
    USBx->dev.connection_status = 1;  
    return USBD_OK;
}

/**
*\*\name   USBD_DevDisconnected.
*\*\fun    Handle device disconnection event
*\*\param  USBx : USB device
*\*\return USBD_OK
**/
static uint8_t USBD_DevDisconnected(USB_CORE_MODULE  *USBx)
{
    USBx->dev.user_cb->DeviceDisconnected();
    USBx->dev.class_cb->DeInit(USBx, 0);
    USBx->dev.connection_status = 0;
    return USBD_OK;
}

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

