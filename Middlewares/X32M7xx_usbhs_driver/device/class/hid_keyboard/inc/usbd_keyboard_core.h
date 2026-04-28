/**
 ******************************************************************************
 * @file      usbd_keyboard_core.h
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

#ifndef __USBD_KEYBOARD_CORE_H__
#define __USBD_KEYBOARD_CORE_H__

#include  "usbd_ioreq.h"

#define KEYBOARD_CFGDESC_SIZE              (41u)
#define KEYBOARD_DESC_SIZE                 (9u)
#define KEYBOARD_REPORT_DESC_SIZE          (66u)

#define KEYBOARD_DESCRIPTOR_TYPE           (0x21u)
#define KEYBOARD_REPORT_DESC               (0x22u)


#define KEYBOARD_REQ_SET_PROTOCOL          (0x0Bu)
#define KEYBOARD_REQ_GET_PROTOCOL          (0x03u)

#define KEYBOARD_REQ_SET_IDLE              (0x0Au)
#define KEYBOARD_REQ_GET_IDLE              (0x02u)


extern USBD_Class_cb_TypeDef  USBD_HID_Keyboard_cb;

extern uint8_t usb_dev_mouse_txreport(USB_CORE_MODULE *pdev, uint8_t *report, uint16_t len);

#endif  // __USBD_KEYBOARD_CORE_H__
