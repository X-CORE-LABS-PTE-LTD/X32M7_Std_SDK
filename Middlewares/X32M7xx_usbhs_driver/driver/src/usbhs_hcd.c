/**
 ******************************************************************************
 * @file      usbhs_hcd.c
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

#include "usbhs_core.h"
#include "usbhs_hcd.h"
#include "usbhs_conf.h"
#include "usbhs_bsp.h"

/**
*\*\name    USBHOST_CoreInit.
*\*\fun     Initialize the HOST portion of the driver.
*\*\param   USBx: selected device.
*\*\return  none.
*\*\
**/
void USBHOST_CoreInit(USB_CORE_MODULE *USBx, USB_CORE_ID_TypeDef coreID)
{
    uint8_t i = 0;
    USBx->host.ConnSts = 0;

    for (i= 0; i< USB_MAX_TX_FIFOS; i++)
    {
        USBx->host.ErrCnt[i] = 0;
        USBx->host.XferCnt[i] = 0;
        USBx->host.HCH_Status[i] = HCH_IDLE;
    }
    USBx->host.hch[0].max_packet = 8;

    USB_BasicInit(USBx, coreID);

    USB_EnableGlobalInt(USBx, DISABLE);
    USB_CoreInit(USBx);

    /* Force Host Mode*/
    USB_SetCurrentMode(USBx, HOST_MODE);
    /* Init Host */
    USB_CoreInitHost(USBx);
    /* Enable USB Global interrupt */
    USB_EnableGlobalInt(USBx, ENABLE);
}

/**
*\*\name    HOST_GetCurrentSpeed.
*\*\fun     Get Current device Speed.
*\*\param   USBx: selected device.
*\*\return  speed.
*\*\
**/
uint32_t HOST_GetCurrentSpeed(USB_CORE_MODULE *USBx)
{
    uint32_t hpcs = 0;
    hpcs = USB_READ_REG32(USBx->regs.HPCS);
    return (hpcs & USBHS_HPCS_PSPD) >> USBHS_HPCS_PSPD_POS;
}

/**
*\*\name    HOST_ResetPort.
*\*\fun     Issues the reset command to device.
*\*\param   USBx: selected device.
*\*\return  none.
*\*\
**/
uint32_t HOST_ResetPort(USB_CORE_MODULE *USBx)
{
    USB_ResetPort(USBx);
    return 0;    
}

/**
*\*\name    HOST_IsDeviceConnected.
*\*\fun     Check if the device is connected.
*\*\param   USBx: selected device.
*\*\return  Device connection status. 1 -> connected and 0 -> disconnected.
*\*\
**/
uint32_t HOST_IsDeviceConnected(USB_CORE_MODULE *USBx)
{
    return (USBx->host.ConnSts);
}

/**
*\*\name    HOST_IsPortEnabled.
*\*\fun     This function checks if port is enabled.
*\*\param   USBx: selected device.
*\*\return  port enable or disable.
*\*\
**/
uint32_t HOST_IsPortEnabled(USB_CORE_MODULE *USBx)
{
    return (USBx->host.PortEnabled);
}

/**
*\*\name    HOST_GetCurrentFrame.
*\*\fun     This function returns the frame number for sof packet.
*\*\param   USBx: selected device.
*\*\return  Frame number.
*\*\
**/
uint32_t HOST_GetCurrentFrame(USB_CORE_MODULE *USBx) 
{
    return (USB_READ_REG32(&USBx->regs.HCSR->HFNUM) & 0xFFFF) ;
}

/**
*\*\name    HOST_GetCurrentFrame.
*\*\fun     This function returns the last URBstate.
*\*\param   USBx: selected device.
*\*\param   ch_num: channel number.
*\*\return  URB_STATE.
*\*\
**/
URB_STATE HOST_GetURB_State(USB_CORE_MODULE *USBx, uint8_t ch_num) 
{
    return USBx->host.URB_State[ch_num] ;
}

/**
*\*\name    HOST_GetXferCnt.
*\*\fun     This function returns the last URBstate.
*\*\param   USBx: selected device.
*\*\param   ch_num: channel number.
*\*\return  Number of data bytes transferred.
*\*\
**/
uint32_t HOST_GetXferCnt(USB_CORE_MODULE *USBx, uint8_t ch_num) 
{
    return USBx->host.XferCnt[ch_num] ;
}

/**
*\*\name    HOST_GetHCState.
*\*\fun     This function returns the HC Status.
*\*\param   USBx: selected device.
*\*\param   ch_num: channel number.
*\*\return  HCH_STATUS.
*\*\
**/
HCH_STATUS HOST_GetHCState(USB_CORE_MODULE *USBx,  uint8_t ch_num) 
{
    return USBx->host.HCH_Status[ch_num] ;
}

/**
*\*\name    HOST_HC_Init.
*\*\fun     This function prepare a HC and start a transfer.
*\*\param   USBx: selected device.
*\*\param   ch_num: channel number.
*\*\return  status.
*\*\
**/
uint32_t HOST_HC_Init(USB_CORE_MODULE *USBx, uint8_t ch_num) 
{
    return USB_HCH_Init(USBx, ch_num);  
}

/**
*\*\name    HOST_SubmitRequest.
*\*\fun     This function prepare a HC and start a transfer.
*\*\param   USBx: selected device.
*\*\param   ch_num: channel number.
*\*\return  status.
*\*\
**/
uint32_t HOST_SubmitRequest(USB_CORE_MODULE *USBx, uint8_t ch_num) 
{
    USBx->host.URB_State[ch_num] = URB_IDLE;  
    USBx->host.hch[ch_num].xfer_count = 0 ;
    return USB_HCH_StartXfer(USBx, ch_num);
}
