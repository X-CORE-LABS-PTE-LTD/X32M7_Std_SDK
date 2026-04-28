/**
 ******************************************************************************
 * @file      usbh_stdreq.h
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

#ifndef __USBH_STDREQ_H__
#define __USBH_STDREQ_H__

#include "usbhs_conf.h"
#include "usbhs_hcd.h"
#include "usbh_core.h"
#include "usbh_def.h"

/*Standard Feature Selector for clear feature command*/
#define FEATURE_SELECTOR_ENDPOINT         0X00
#define FEATURE_SELECTOR_DEVICE           0X01


#define INTERFACE_DESC_TYPE               0x04
#define ENDPOINT_DESC_TYPE                0x05
#define INTERFACE_DESC_SIZE               0x09


#define USBH_HID_CLASS                    0x03

extern uint8_t USBH_CfgDesc[512];

USBH_Status USBH_GetDescriptor(USB_CORE_MODULE *USBx, USBH_HOST *phost, uint8_t req_type,uint16_t value_idx, uint8_t* buff, uint16_t length );

USBH_Status USBH_Get_DevDesc(USB_CORE_MODULE *USBx, USBH_HOST *phost, uint8_t length);

USBH_Status USBH_Get_StringDesc(USB_CORE_MODULE *USBx, USBH_HOST *phost, uint8_t string_index, uint8_t *buff, uint16_t length);

USBH_Status USBH_SetCfg(USB_CORE_MODULE *USBx, USBH_HOST *phost, uint16_t configuration_value);

USBH_Status USBH_Get_CfgDesc(USB_CORE_MODULE *USBx, USBH_HOST *phost, uint16_t length);

USBH_Status USBH_SetAddress(USB_CORE_MODULE *USBx, USBH_HOST *phost, uint8_t DeviceAddress);

USBH_Status USBH_ClrFeature(USB_CORE_MODULE *USBx, USBH_HOST *phost, uint8_t ep_num, uint8_t hch_num); 

USBH_Status USBH_SetInterface(USB_CORE_MODULE *USBx, USBH_HOST *phost, uint8_t ep_num, uint8_t altSetting);

USBH_Status USBH_Issue_ClrFeature(USB_CORE_MODULE *USBx, USBH_HOST *phost, uint8_t ep_num);

USBH_DescHeader_t *USBH_GetNextDesc(uint8_t *pbuf, uint16_t *ptr);

#endif /* __USBH_STDREQ_H__ */
