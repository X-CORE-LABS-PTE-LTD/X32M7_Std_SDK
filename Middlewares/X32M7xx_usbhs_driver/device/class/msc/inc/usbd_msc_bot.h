/**
 ******************************************************************************
 * @file      usbd_msc_bot.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_MSC_BOT_H
#define __USBD_MSC_BOT_H

#define BOT_IDLE                      0       /* Idle state */
#define BOT_DATA_OUT                  1       /* Data Out state */
#define BOT_DATA_IN                   2       /* Data In state */
#define BOT_LAST_DATA_IN              3       /* Last Data In Last */
#define BOT_SEND_DATA                 4       /* Send Immediate data */

#define BOT_CBW_SIGNATURE             0x43425355
#define BOT_CSW_SIGNATURE             0x53425355
#define BOT_CBW_LENGTH                31
#define BOT_CSW_LENGTH                13

/* CSW Status Definitions */
#define CSW_CMD_PASSED                0x00
#define CSW_CMD_FAILED                0x01
#define CSW_PHASE_ERROR               0x02

/* BOT Status */
#define BOT_STATE_NORMAL              0
#define BOT_STATE_RECOVERY            1
#define BOT_STATE_ERROR               2


#define DIR_IN                        0
#define DIR_OUT                       1
#define BOTH_DIR                      2

typedef struct _MSC_BOT_CBW
{
  uint32_t dSignature;
  uint32_t dTag;
  uint32_t dDataLength;
  uint8_t  bmFlags;
  uint8_t  bLUN;
  uint8_t  bCBLength;
  uint8_t  CB[16];
}
MSC_BOT_CBW_TypeDef;


typedef struct _MSC_BOT_CSW
{
  uint32_t dSignature;
  uint32_t dTag;
  uint32_t dDataResidue;
  uint8_t  bStatus;
}
MSC_BOT_CSW_TypeDef;

extern uint8_t              MSC_BOT_Data[];
extern uint16_t             MSC_BOT_DataLen;
extern uint8_t              MSC_BOT_State;
extern uint8_t              MSC_BOT_BurstMode;
extern MSC_BOT_CBW_TypeDef  MSC_BOT_cbw;
extern MSC_BOT_CSW_TypeDef  MSC_BOT_csw;

void MSC_BOT_Init (USB_CORE_MODULE  *pdev);
void MSC_BOT_Reset (USB_CORE_MODULE  *pdev);
void MSC_BOT_DeInit (USB_CORE_MODULE  *pdev);
void MSC_BOT_DataIn (USB_CORE_MODULE  *pdev, uint8_t epnum);

void MSC_BOT_DataOut (USB_CORE_MODULE  *pdev, uint8_t epnum);

void MSC_BOT_SendCSW (USB_CORE_MODULE  *pdev,uint8_t CSW_Status);

void  MSC_BOT_CplClrFeature (USB_CORE_MODULE  *pdev, uint8_t epnum);

#endif /* __USBD_MSC_BOT_H */
