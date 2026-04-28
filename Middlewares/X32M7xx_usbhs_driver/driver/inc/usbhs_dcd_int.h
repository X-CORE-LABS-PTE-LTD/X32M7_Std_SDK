/**
 ******************************************************************************
 * @file      usbhs_dcd_int.h
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

#ifndef __USBHS_DEV_INT_H__
#define __USBHS_DEV_INT_H__


#include "usbhs_dcd.h"

typedef struct _USBD_DEV_INT
{
  uint8_t (* DataOutStage) (USB_CORE_MODULE *USBx , uint8_t epnum);
  uint8_t (* DataInStage)  (USB_CORE_MODULE *USBx , uint8_t epnum);
  uint8_t (* SetupStage) (USB_CORE_MODULE *USBx);
  uint8_t (* SOF) (USB_CORE_MODULE *USBx);
  uint8_t (* Reset) (USB_CORE_MODULE *USBx);
  uint8_t (* Suspend) (USB_CORE_MODULE *USBx);
  uint8_t (* Resume) (USB_CORE_MODULE *USBx);
  uint8_t (* IsoINIncomplete) (USB_CORE_MODULE *USBx);
  uint8_t (* IsoOUTIncomplete) (USB_CORE_MODULE *USBx);  
  
  uint8_t (* DevConnected) (USB_CORE_MODULE *USBx);
  uint8_t (* DevDisconnected) (USB_CORE_MODULE *USBx);   
  
}USBD_DEV_INT_cb_TypeDef;

extern USBD_DEV_INT_cb_TypeDef *USBD_DEV_INT_fops;

uint32_t USBD_ISTR_Handler(USB_CORE_MODULE *USBx);
void USBD_ISTR_WKUP_handler(USB_CORE_MODULE *USBx);
void USBD_EP_OUT_ISTR_Handler(USB_CORE_MODULE *USBx);
void USBD_EP_IN_ISTR_Handler(USB_CORE_MODULE *USBx);

#endif /* __USBHS_DEV_INT_H__ */
