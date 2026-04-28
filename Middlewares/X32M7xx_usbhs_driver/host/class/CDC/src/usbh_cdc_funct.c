/**
 ******************************************************************************
 * @file      usbh_cdc_funct.c
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

#include "usbh_cdc_funct.h"

CDC_InterfaceDesc_Typedef         CDC_Desc;
CDC_LineCodingTypeDef             CDC_GetLineCode;
CDC_LineCodingTypeDef             CDC_SetLineCode;

extern CDC_Requests               CDC_ReqState;

/**
*\*\name    CDC_GETLineCoding.
*\*\fun     This request allows the host to find out the currently configured line coding.
*\*\param   pdev: Selected device
*\*\param   phost: Selected device property
*\*\return  status.
**/
USBH_Status CDC_GETLineCoding(USB_CORE_MODULE *pdev , USBH_HOST *phost)
{
    phost->Control.setup.b.bmRequestType = USB_D2H | USB_REQ_TYPE_CLASS | USB_REQ_RECIPIENT_INTERFACE;

    phost->Control.setup.b.bRequest = CDC_GET_LINE_CODING;
    phost->Control.setup.b.wValue.w = 0;
    phost->Control.setup.b.wIndex.w = CDC_Desc.CDC_UnionFuncDesc.bMasterInterface; /*At to be checked*/
    phost->Control.setup.b.wLength.w = LINE_CODING_STRUCTURE_SIZE;           

    return USBH_CtrlReq(pdev, phost, CDC_GetLineCode.Array, LINE_CODING_STRUCTURE_SIZE);
}

/**
*\*\name    CDC_SETLineCoding.
*\*\fun     This request allows the host to specify typical asynchronous line-character formatting properties.
*\*\param   pdev: Selected device
*\*\param   phost: Selected device property
*\*\return  status.
**/
USBH_Status CDC_SETLineCoding(USB_CORE_MODULE *pdev, USBH_HOST *phost)
{
    phost->Control.setup.b.bmRequestType = USB_H2D | USB_REQ_TYPE_CLASS | USB_REQ_RECIPIENT_INTERFACE;

    phost->Control.setup.b.bRequest = CDC_SET_LINE_CODING;
    phost->Control.setup.b.wValue.w = 0;

    phost->Control.setup.b.wIndex.w = CDC_Desc.CDC_UnionFuncDesc.bMasterInterface;

    phost->Control.setup.b.wLength.w = LINE_CODING_STRUCTURE_SIZE;           

    return USBH_CtrlReq(pdev, phost, CDC_SetLineCode.Array , LINE_CODING_STRUCTURE_SIZE );  
}

/**
*\*\name    CDC_SETControlLineState.
*\*\fun     This request generates RS-232/V.24 style control signals.
*\*\param   pdev: Selected device
*\*\param   phost: Selected device property
*\*\return  status.
**/
USBH_Status CDC_SETControlLineState(USB_CORE_MODULE *pdev, USBH_HOST *phost)
{  
    phost->Control.setup.b.bmRequestType = USB_H2D | USB_REQ_TYPE_CLASS | USB_REQ_RECIPIENT_INTERFACE;

    phost->Control.setup.b.bRequest = CDC_SET_CONTROL_LINE_STATE;
    /*Control Signal Bitmap Values for SetControlLineState*/
    phost->Control.setup.b.wValue.w = CDC_DEACTIVATE_CARRIER_SIGNAL_RTS | CDC_DEACTIVATE_SIGNAL_DTR;

    phost->Control.setup.b.wIndex.w = CDC_Desc.CDC_UnionFuncDesc.bMasterInterface;

    /*Length feild is zero*/
    phost->Control.setup.b.wLength.w = 0;

    return USBH_CtrlReq(pdev, phost, 0 , 0 );
}

/**
*\*\name    CDC_ChangeStateToIssueSetConfig.
*\*\fun     This function prepares the state before issuing the class specific commands.
*\*\param   pdev: Selected device
*\*\param   phost: Selected device property
*\*\return  none.
**/
void CDC_ChangeStateToIssueSetConfig(USB_CORE_MODULE *pdev, USBH_HOST *phost)
{
    phost->gStateBkp = phost->gState  ;
    phost->gState = HOST_CLASS_REQUEST;
    CDC_ReqState = CDC_SET_LINE_CODING_RQUEST;
}

/**
*\*\name    CDC_ChangeStateToIssueSetConfig.
*\*\fun     This function prepares the state before issuing the class specific commands.
*\*\param   pdev: Selected device
*\*\param   phost: Selected device property
*\*\return  none.
**/
void CDC_IssueGetConfig(USB_CORE_MODULE *pdev, USBH_HOST *phost)
{
    phost->gStateBkp =  phost->gState ;
    phost->gState = HOST_CLASS_REQUEST;
    CDC_ReqState = CDC_GET_LINE_CODING_RQUEST;
}
