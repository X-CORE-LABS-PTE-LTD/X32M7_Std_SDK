/**
 ******************************************************************************
 * @file      usbhs_dcd.h
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

#ifndef __USBHS_DEV_H__
#define __USBHS_DEV_H__

#include "usbhs_core.h"

#define USB_EP_CONTROL                       0
#define USB_EP_ISOC                          1
#define USB_EP_BULK                          2
#define USB_EP_INT                           3
#define USB_EP_MASK                          3

/*  Device Status */
#define USB_DEFAULT                          1
#define USB_ADDRESSED                        2
#define USB_CONFIGURED                       3
#define USB_SUSPENDED                        4

typedef struct
{
  uint8_t  bLength;
  uint8_t  bDescriptorType;
  uint8_t  bEndpointAddress;
  uint8_t  bmAttributes;
  uint16_t wMaxPacketSize;
  uint8_t  bInterval;
}EP_DESCRIPTOR;

void USBDEV_CoreInit(USB_CORE_MODULE *USBx, USB_CORE_ID_TypeDef coreID);

void USBDEV_DevConnect(USB_CORE_MODULE *USBx);
void USBDEV_DevDisconnect(USB_CORE_MODULE *USBx);

void USBDEV_EP_SetAddress(USB_CORE_MODULE *USBx, uint8_t address);
uint32_t USBDEV_EP_Open(USB_CORE_MODULE *USBx , uint8_t ep_addr, uint16_t ep_mps, uint8_t ep_type);
uint32_t USBDEV_EP_Close(USB_CORE_MODULE *USBx, uint8_t  ep_addr);
uint32_t USBDEV_EP_PrepareRx(USB_CORE_MODULE *USBx, uint8_t ep_addr, uint8_t *pbuf, uint16_t buf_len);
uint32_t USBDEV_EP_Tx (USB_CORE_MODULE *USBx, uint8_t ep_addr, uint8_t  *pbuf, uint32_t buf_len);
uint32_t USBDEV_EP_Stall (USB_CORE_MODULE *USBx, uint8_t epnum);
uint32_t USBDEV_EP_ClrStall (USB_CORE_MODULE *USBx, uint8_t epnum);
uint32_t USBDEV_EP_Flush(USB_CORE_MODULE *USBx, uint8_t epnum);

uint32_t USBDEV_Handle_ISTR(USB_CORE_MODULE *USBx);
uint32_t USBDEV_GetEPStatus(USB_CORE_MODULE *USBx, uint8_t epnum);

void USBDEV_SetEPStatus(USB_CORE_MODULE *USBx , uint8_t epnum , uint32_t Status);

#endif /* __USBHS_DEV_H__ */
