/**
 ******************************************************************************
 * @file      usbh_ioreq.c
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

#include "usbh_ioreq.h"

static USBH_Status USBH_SubmitSetupRequest(USBH_HOST *phost, uint8_t* buff, uint16_t length);

/**
*\*\name    USBH_CtrlReq.
*\*\fun     USBH_CtrlReq sends a control request and provide the status after completion of the request
*\*\param   USBx: selected device.
*\*\param   phost: host state set.
*\*\param   buff: data buffer address to store the response
*\*\param   length: length of the response
*\*\return  status.
*\*\
**/
USBH_Status USBH_CtrlReq(USB_CORE_MODULE *USBx, USBH_HOST *phost, uint8_t *buff, uint16_t length)
{
    USBH_Status status;
    status = USBH_BUSY;

    switch(phost->RequestState)
    {
        case CMD_SEND:
            /* Start a SETUP transfer */
            USBH_SubmitSetupRequest(phost, buff, length);
            phost->RequestState = CMD_WAIT;
            status = USBH_BUSY;
        break;

        case CMD_WAIT:
            if(phost->Control.state == CTRL_COMPLETE) 
            {
                /* Commands successfully sent and Response Received  */       
                phost->RequestState = CMD_SEND;
                phost->Control.state =CTRL_IDLE;  
                status = USBH_OK;      
            }
            else if(phost->Control.state == CTRL_ERROR)
            {
                /* Failure Mode */
                phost->RequestState = CMD_SEND;
                status = USBH_FAIL;
            }   
            else if(phost->Control.state == CTRL_STALLED)
            {
                /* Commands successfully sent and Response Received  */       
                phost->RequestState = CMD_SEND;
                status = USBH_NOT_SUPPORTED;
            }
        break;

        default:
        break; 
    }
    return status;
}

/**
*\*\name    USBH_CtrlSendSetup.
*\*\fun     Sends the Setup Packet to the Device.
*\*\param   USBx: selected device.
*\*\param   buff: data buffer address to store the response
*\*\param   hch_num: Host channel Number
*\*\return  status.
*\*\
**/
USBH_Status USBH_CtrlSendSetup(USB_CORE_MODULE *USBx, uint8_t *buff, uint8_t hch_num)
{
    USBx->host.hch[hch_num].ep_is_in = 0;
    USBx->host.hch[hch_num].data_pid = HCH_PID_SETUP;   
    USBx->host.hch[hch_num].xfer_buff = buff;
    USBx->host.hch[hch_num].xfer_len = USBH_SETUP_PKT_SIZE;   

    return (USBH_Status)HOST_SubmitRequest (USBx , hch_num);   
}

/**
*\*\name    USBH_CtrlSendSetup.
*\*\fun     Sends the Setup Packet to the Device.
*\*\param   USBx: selected device.
*\*\param   buff: data buffer address to store the response
*\*\param   length: Length of the data to be sent
*\*\param   hch_num: Host channel Number
*\*\return  status.
*\*\
**/
USBH_Status USBH_CtrlSendData(USB_CORE_MODULE *USBx, uint8_t *buff, uint16_t length, uint8_t hch_num)
{
    USBx->host.hch[hch_num].ep_is_in = 0;
    USBx->host.hch[hch_num].xfer_buff = buff;
    USBx->host.hch[hch_num].xfer_len = length;

    if(length == 0)
    {   /* For Status OUT stage, Length==0, Status Out PID = 1 */
        USBx->host.hch[hch_num].toggle_out = 1;   
    }

    /* Set the Data Toggle bit as per the Flag */
    if(USBx->host.hch[hch_num].toggle_out == 0)
    {   /* Put the PID 0 */
        USBx->host.hch[hch_num].data_pid = HCH_PID_DATA0;    
    }
    else
    {   /* Put the PID 1 */
        USBx->host.hch[hch_num].data_pid = HCH_PID_DATA1 ;
    }

    HOST_SubmitRequest (USBx , hch_num);   

    return USBH_OK;
}

/**
*\*\name    USBH_CtrlReceiveData.
*\*\fun     Receives the Device Response to the Setup Packet.
*\*\param   USBx: selected device.
*\*\param   buff: data buffer address to store the response
*\*\param   length: Length of the data to be sent
*\*\param   hch_num: Host channel Number
*\*\return  status.
*\*\
**/
USBH_Status USBH_CtrlReceiveData(USB_CORE_MODULE *USBx, uint8_t* buff, uint16_t length, uint8_t hch_num)
{
    USBx->host.hch[hch_num].ep_is_in = 1;
    USBx->host.hch[hch_num].data_pid = HCH_PID_DATA1;
    USBx->host.hch[hch_num].xfer_buff = buff;
    USBx->host.hch[hch_num].xfer_len = length;  

    HOST_SubmitRequest (USBx , hch_num);

    return USBH_OK;
}

/**
*\*\name    USBH_BulkSendData.
*\*\fun     Sends the Bulk Packet to the device.
*\*\param   USBx: selected device.
*\*\param   buff: Buffer pointer from which the Data will be sent to Device
*\*\param   length: Length of the data to be sent
*\*\param   hch_num: Host channel Number
*\*\return  status.
*\*\
**/
USBH_Status USBH_BulkSendData(USB_CORE_MODULE *USBx, uint8_t *buff, uint16_t length, uint8_t hch_num)
{ 
    USBx->host.hch[hch_num].ep_is_in = 0;
    USBx->host.hch[hch_num].xfer_buff = buff;
    USBx->host.hch[hch_num].xfer_len = length;  

    /* Set the Data Toggle bit as per the Flag */
    if(USBx->host.hch[hch_num].toggle_out == 0)
    {   /* Put the PID 0 */
        USBx->host.hch[hch_num].data_pid = HCH_PID_DATA0;    
    }
    else
    {   /* Put the PID 1 */
        USBx->host.hch[hch_num].data_pid = HCH_PID_DATA1 ;
    }

    HOST_SubmitRequest (USBx , hch_num);   
    return USBH_OK;
}

/**
*\*\name    USBH_BulkReceiveData.
*\*\fun     Receives IN bulk packet from device.
*\*\param   USBx: selected device.
*\*\param   buff: Buffer pointer from which the received data packet to be copied.
*\*\param   length: Length of the data to be received.
*\*\param   hch_num: Host channel Number
*\*\return  status.
*\*\
**/
USBH_Status USBH_BulkReceiveData(USB_CORE_MODULE *USBx, uint8_t *buff, uint16_t length, uint8_t hch_num)
{
    USBx->host.hch[hch_num].ep_is_in = 1;   
    USBx->host.hch[hch_num].xfer_buff = buff;
    USBx->host.hch[hch_num].xfer_len = length;

    if( USBx->host.hch[hch_num].toggle_in == 0)
    {
        USBx->host.hch[hch_num].data_pid = HCH_PID_DATA0;
    }
    else
    {
        USBx->host.hch[hch_num].data_pid = HCH_PID_DATA1;
    }

    HOST_SubmitRequest (USBx , hch_num);  
    return USBH_OK;
}

/**
*\*\name    USBH_InterruptReceiveData.
*\*\fun     Receives the Device Response to the Interrupt IN token.
*\*\param   USBx: selected device.
*\*\param   buff: Buffer pointer from which the received data packet to be copied.
*\*\param   length: Length of the data to be received.
*\*\param   hch_num: Host channel Number
*\*\return  status.
*\*\
**/
USBH_Status USBH_InterruptReceiveData(USB_CORE_MODULE *USBx, uint8_t *buff, uint8_t length, uint8_t hch_num)
{
    USBx->host.hch[hch_num].ep_is_in = 1;  
    USBx->host.hch[hch_num].xfer_buff = buff;
    USBx->host.hch[hch_num].xfer_len = length;

    if(USBx->host.hch[hch_num].toggle_in == 0)
    {
        USBx->host.hch[hch_num].data_pid = HCH_PID_DATA0;
    }
    else
    {
        USBx->host.hch[hch_num].data_pid = HCH_PID_DATA1;
    }

    /* toggle DATA PID */
    USBx->host.hch[hch_num].toggle_in ^= 1;  

    HOST_SubmitRequest (USBx , hch_num);  

    return USBH_OK;
}

/**
*\*\name    USBH_InterruptSendData.
*\*\fun     Sends the data on Interrupt OUT Endpoint.
*\*\param   USBx: selected device.
*\*\param   buff: Buffer pointer from where the data needs to be copied.
*\*\param   length: Length of the data to be sent.
*\*\param   hch_num: Host channel Number
*\*\return  status.
*\*\
**/
USBH_Status USBH_InterruptSendData( USB_CORE_MODULE *USBx, uint8_t *buff, uint8_t length, uint8_t hch_num)
{
    USBx->host.hch[hch_num].ep_is_in = 0;  
    USBx->host.hch[hch_num].xfer_buff = buff;
    USBx->host.hch[hch_num].xfer_len = length;

    if(USBx->host.hch[hch_num].toggle_in == 0)
    {
        USBx->host.hch[hch_num].data_pid = HCH_PID_DATA0;
    }
    else
    {
        USBx->host.hch[hch_num].data_pid = HCH_PID_DATA1;
    }

    USBx->host.hch[hch_num].toggle_in ^= 1;  

    HOST_SubmitRequest (USBx , hch_num);  

    return USBH_OK;
}

/**
*\*\name    USBH_SubmitSetupRequest.
*\*\fun     Start a setup transfer by changing the state-machine and initializing  the required variables needed for the Control Transfer.
*\*\param   USBx: selected device.
*\*\param   buff: Buffer used for setup request.
*\*\param   length: Length of the data.
*\*\return  status.
*\*\
**/
static USBH_Status USBH_SubmitSetupRequest(USBH_HOST *phost, uint8_t* buff, uint16_t length)
{
    /* Save Global State */
    phost->gStateBkp =   phost->gState; 

    /* Prepare the Transactions */
    phost->gState = HOST_CTRL_XFER;
    phost->Control.buff = buff; 
    phost->Control.length = length;
    phost->Control.state = CTRL_SETUP;  

    return USBH_OK;  
}

/**
*\*\name    USBH_IsocReceiveData.
*\*\fun     Receives the Device Response to the Isochronous IN token.
*\*\param   USBx: selected device.
*\*\param   buff: Buffer pointer in which the response needs to be copied.
*\*\param   length: Length of the data to be received.
*\*\param   hch_num: Host channel Number
*\*\return  status.
*\*\
**/
USBH_Status USBH_IsocReceiveData( USB_CORE_MODULE *USBx, uint8_t *buff, uint32_t length, uint8_t hch_num)
{
    USBx->host.hch[hch_num].ep_is_in = 1;  
    USBx->host.hch[hch_num].xfer_buff = buff;
    USBx->host.hch[hch_num].xfer_len = length;
    USBx->host.hch[hch_num].data_pid = HCH_PID_DATA0;

    HOST_SubmitRequest (USBx , hch_num);  

    return USBH_OK;
}

/**
*\*\name    USBH_IsocSendData.
*\*\fun     Sends the data on Isochronous OUT Endpoint.
*\*\param   USBx: selected device.
*\*\param   buff: Buffer pointer from where the data needs to be copied.
*\*\param   length: Length of the data to be sent.
*\*\param   hch_num: Host channel Number
*\*\return  status.
*\*\
**/
USBH_Status USBH_IsocSendData( USB_CORE_MODULE *USBx, uint8_t *buff, uint32_t length, uint8_t hch_num)
{
    USBx->host.hch[hch_num].ep_is_in = 0;  
    USBx->host.hch[hch_num].xfer_buff = buff;
    USBx->host.hch[hch_num].xfer_len = length;
    USBx->host.hch[hch_num].data_pid = HCH_PID_DATA0;

    HOST_SubmitRequest (USBx , hch_num);  

    return USBH_OK;
}
