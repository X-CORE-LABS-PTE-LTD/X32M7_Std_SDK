/**
 ******************************************************************************
 * @file      x32m7xx_wwdg.h
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

#ifndef __X32M7XX_WWDG_H__
#define __X32M7XX_WWDG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "x32m7xx.h"

#define WWDG_PRESCALER_DIV1             ((uint32_t)0x00000000)
#define WWDG_PRESCALER_DIV2             ((uint32_t)WWDG_CFG_TIMERB0)
#define WWDG_PRESCALER_DIV4             ((uint32_t)WWDG_CFG_TIMERB1)
#define WWDG_PRESCALER_DIV8             ((uint32_t)(WWDG_CFG_TIMERB1 | WWDG_CFG_TIMERB0))


/** EWINT bit **/
#define EARLY_WAKEUP_INT                (WWDG_CFG_EWINT)
#define EARLY_WAKEUP_FLAG               (WWDG_STS_EWINTF)

/** CTRL register bit mask **/
#define CTRL_ACTB_SET                   ((uint32_t)WWDG_CTRL_ACTB)

/* CFG register bit mask **/
#define CFG_TIMERB_MASK                 ((uint32_t)0xFFFF3FFFU)
#define CFG_W_MASK                      ((uint32_t)0xFFFFC000U)
#define BIT_MASK                        ((uint16_t)0x3FFF)


void WWDG_DeInit(WWDG_Module* WWDGx);
void WWDG_SetPrescalerDiv(WWDG_Module* WWDGx,uint32_t WWDG_Prescaler);
void WWDG_SetWValue(WWDG_Module* WWDGx,uint16_t WindowValue);
void WWDG_EnableInt(WWDG_Module* WWDGx);
void WWDG_SetCnt(WWDG_Module* WWDGx,uint16_t Counter);
void WWDG_Enable(WWDG_Module* WWDGx,uint16_t Counter);
FlagStatus WWDG_GetEWINTF(WWDG_Module* WWDGx);
void WWDG_ClrEWINTF(WWDG_Module* WWDGx);

#ifdef __cplusplus
}
#endif

#endif /* __X32M7XX_WWDG_H__ */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
