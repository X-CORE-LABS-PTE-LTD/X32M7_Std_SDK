/**
 ******************************************************************************
 * @file      usbh_hcs.c
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

#include "usbh_hcs.h"
#include "usbhs_core.h"

static uint16_t USBH_GetFreeChannel(USB_CORE_MODULE *USBx);

/**
*\*\name    USBH_Open_Channel.
*\*\fun     Open a  pipe.
*\*\param   USBx: selected device.
*\*\param   hch_num: Host channel Number
*\*\param   dev_address: USB Device address allocated to attached device
*\*\param   speed : USB device speed (Full/Low)
*\*\param   ep_type: end point type (Bulk/int/ctl)
*\*\param   mps: max pkt size
*\*\return  HC_OK.
*\*\
**/
uint8_t USBH_Open_Channel(USB_CORE_MODULE *USBx, uint8_t hch_num, uint8_t dev_address, uint8_t speed, uint8_t ep_type, uint16_t mps)
{
    USBx->host.hch[hch_num].ep_num = USBx->host.channel[hch_num]& 0x7F;
    USBx->host.hch[hch_num].ep_is_in = (USBx->host.channel[hch_num] & 0x80) == 0x80;  
    USBx->host.hch[hch_num].dev_addr = dev_address;  
    USBx->host.hch[hch_num].ep_type = ep_type;  
    USBx->host.hch[hch_num].max_packet = mps; 
    USBx->host.hch[hch_num].speed = speed; 
    USBx->host.hch[hch_num].toggle_in = 0; 
    USBx->host.hch[hch_num].toggle_out = 0;
    if(speed == HPCS_PRTSPD_HIGH_SPEED)
    {
        USBx->host.hch[hch_num].do_ping = 1;
    }
  
    USB_HCH_Init(USBx, hch_num) ;

    return HC_OK; 
}

/**
*\*\name    USBH_Modify_Channel.
*\*\fun     Modify a pipe.
*\*\param   USBx: selected device.
*\*\param   hch_num: Host channel Number
*\*\param   dev_address: USB Device address allocated to attached device
*\*\param   speed : USB device speed (Full/Low)
*\*\param   ep_type: end point type (Bulk/int/ctl)
*\*\param   mps: max pkt size
*\*\return  HC_OK.
*\*\
**/
uint8_t USBH_Modify_Channel(USB_CORE_MODULE *USBx, uint8_t hch_num, uint8_t dev_address, uint8_t speed, uint8_t ep_type, uint16_t mps)
{
    if(dev_address != 0)
    {
        USBx->host.hch[hch_num].dev_addr = dev_address;  
    }

    if((USBx->host.hch[hch_num].max_packet != mps) && (mps != 0))
    {
        USBx->host.hch[hch_num].max_packet = mps; 
    }

    if((USBx->host.hch[hch_num].speed != speed ) && (speed != 0 )) 
    {
        USBx->host.hch[hch_num].speed = speed; 
    }

    USB_HCH_Init(USBx, hch_num);
    return HC_OK; 

}

/**
*\*\name    USBH_Alloc_Channel.
*\*\fun     Allocate a new channel for the pipe.
*\*\param   USBx: selected device.
*\*\param   ep_addr: End point for which the channel to be allocated
*\*\return  hch_num: Host channel number.
*\*\
**/
uint8_t USBH_Alloc_Channel(USB_CORE_MODULE *USBx, uint8_t ep_addr)
{
    uint16_t hch_num;

    hch_num =  USBH_GetFreeChannel(USBx);

    if (hch_num != HC_ERROR)
    {
        USBx->host.channel[hch_num & (USB_MAX_TX_FIFOS - 1U)] = HC_USED | ep_addr;
    }
    return hch_num;
}

/**
*\*\name    USBH_Free_Channel.
*\*\fun     Free the USB host channel.
*\*\param   USBx: selected device.
*\*\param   idx: Channel number to be freed.
*\*\return  USBH_OK.
*\*\
**/
uint8_t USBH_Free_Channel(USB_CORE_MODULE *USBx, uint8_t idx)
{
    if(idx < HC_MAX)
    {
        USBx->host.channel[idx & (USB_MAX_TX_FIFOS - 1U)] = 0;
    }
    return USBH_OK;
}

/**
*\*\name    USBH_DeAllocate_AllChannel.
*\*\fun     Free all USB host channel.
*\*\param   USBx: selected device.
*\*\return  USBH_OK.
*\*\
**/
uint8_t USBH_DeAllocate_AllChannel(USB_CORE_MODULE *USBx)
{
    uint8_t idx;

    for (idx = 2; idx < HC_MAX ; idx ++)
    {
        USBx->host.channel[idx] = 0;
    }
    return USBH_OK;
}

/**
*\*\name    USBH_GetFreeChannel.
*\*\fun     Get a free channel number for allocation to a device endpoint.
*\*\param   USBx: selected device.
*\*\return  idx: Free Channel number.
*\*\
**/
static uint16_t USBH_GetFreeChannel(USB_CORE_MODULE *USBx)
{
    uint8_t idx = 0;

    for (idx = 0 ; idx < HC_MAX ; idx++)
    {
        if ((USBx->host.channel[idx] & HC_USED) == 0)
        {
            return idx;
        } 
    }
    return HC_ERROR;
}
