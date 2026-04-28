/**
 ******************************************************************************
 * @file      x32m7xx_iwdg.h
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

#ifndef __X32M7XX_IWDG_H__
#define __X32M7XX_IWDG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "x32m7xx.h"

typedef enum 
{
    IWDG_PVU_FLAG  = IWDG_STS_PVU,
    IWDG_CRVU_FLAG = IWDG_STS_CRVU,
    IWDG_FRZF_FLAG = IWDG_STS_FRZF,
}IWDG_STATUS_FLAG;

/** KEY register bit mask **/
#define KEY_ReloadKey                      ((uint16_t)0xAAAA)
#define KEY_EnableKey                      ((uint16_t)0xCCCC)
#define IWDG_FREEZE                        ((uint16_t)0x4567)
#define IWDG_UNFREEZE                      ((uint16_t)0x89AB)

/** PREDIV and RELV register write permission **/
typedef enum 
{
    IWDG_WRITE_ENABLE  = 0x5555,
    IWDG_WRITE_DISABLE = 0x0000
}IWDOG_WRITE_CONFIG;

#define IWDG_PRESCALER_DIV4    ((uint8_t)0x00)
#define IWDG_PRESCALER_DIV8    (IWDG_PREDIV_PD0)
#define IWDG_PRESCALER_DIV16   (IWDG_PREDIV_PD1)
#define IWDG_PRESCALER_DIV32   (IWDG_PREDIV_PD1 | IWDG_PREDIV_PD0)
#define IWDG_PRESCALER_DIV64   (IWDG_PREDIV_PD2)
#define IWDG_PRESCALER_DIV128  (IWDG_PREDIV_PD2 | IWDG_PREDIV_PD0)
#define IWDG_PRESCALER_DIV256  (IWDG_PREDIV_PD2 | IWDG_PREDIV_PD1 | IWDG_PREDIV_PD0)


void IWDG_WriteConfig(IWDG_Module* IWDGx,IWDOG_WRITE_CONFIG IWDG_WriteAccess);
void IWDG_SetPrescalerDiv(IWDG_Module* IWDGx,uint8_t IWDG_Prescaler);
void IWDG_CntReload(IWDG_Module* IWDGx,uint16_t Reload);
void IWDG_ReloadKey(IWDG_Module* IWDGx);
void IWDG_Enable(IWDG_Module* IWDGx);
void IWDG_Freeze_Enable(IWDG_Module* IWDGx,FunctionalState Cmd);
FlagStatus IWDG_GetStatus(IWDG_Module* IWDGx,IWDG_STATUS_FLAG IWDG_FLAG);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
}
#endif

#endif /* __X32M7XX_IWDG_H__ */
