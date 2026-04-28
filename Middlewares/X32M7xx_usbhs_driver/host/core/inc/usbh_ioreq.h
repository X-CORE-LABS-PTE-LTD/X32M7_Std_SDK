/**
 ******************************************************************************
 * @file      usbh_ioreq.h
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

#ifndef __USBH_IOREQ_H__
#define __USBH_IOREQ_H__

#include "usbhs_conf.h"
#include "usbh_core.h"
#include "usbh_def.h"

#define USBH_SETUP_PKT_SIZE   8
#define USBH_EP0_EP_NUM       0
#define USBH_MAX_PACKET_SIZE  0x40

USBH_Status USBH_CtrlSendSetup(USB_CORE_MODULE *USBx, uint8_t *buff, uint8_t hch_num);

USBH_Status USBH_CtrlSendData(USB_CORE_MODULE *USBx, uint8_t *buff, uint16_t length, uint8_t hch_num);

USBH_Status USBH_CtrlReceiveData(USB_CORE_MODULE *USBx, uint8_t *buff, uint16_t length, uint8_t hch_num);

USBH_Status USBH_BulkReceiveData(USB_CORE_MODULE *USBx, uint8_t *buff, uint16_t length, uint8_t hch_num);

USBH_Status USBH_BulkSendData(USB_CORE_MODULE *USBx, uint8_t *buff, uint16_t length, uint8_t hch_num);

USBH_Status USBH_InterruptReceiveData(USB_CORE_MODULE *USBx, uint8_t *buff, uint8_t length, uint8_t hch_num);

USBH_Status USBH_InterruptSendData(USB_CORE_MODULE *USBx, uint8_t *buff, uint8_t length, uint8_t hch_num);

USBH_Status USBH_CtrlReq(USB_CORE_MODULE *USBx, USBH_HOST *phost, uint8_t *buff,uint16_t length);

USBH_Status USBH_IsocReceiveData(USB_CORE_MODULE *USBx, uint8_t *buff, uint32_t length, uint8_t hch_num);

USBH_Status USBH_IsocSendData(USB_CORE_MODULE *USBx, uint8_t *buff, uint32_t length, uint8_t hch_num);

#endif /* __USBH_IOREQ_H__ */
