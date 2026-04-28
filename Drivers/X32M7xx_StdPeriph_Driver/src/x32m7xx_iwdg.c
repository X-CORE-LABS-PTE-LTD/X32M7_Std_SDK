/**
 ******************************************************************************
 * @file      x32m7xx_iwdg.c
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

#include "x32m7xx_iwdg.h"

/**
*\*\name    IWDG_WriteConfig.
*\*\fun     Enables or disables write protection to IWDG_PREDIV and IWDG_RELV registers.
*\*\param   IWDGx (The input parameters must be the following values):
*\*\          - IWDG1
*\*\          - IWDG2
*\*\param   IWDG_WriteAccess
*\*\        - IWDG_WRITE_ENABLE
*\*\        - IWDG_WRITE_DISABLE
*\*\return  none
**/
void IWDG_WriteConfig(IWDG_Module* IWDGx, IWDOG_WRITE_CONFIG IWDG_WriteAccess)
{
    /* Check the parameters */
    IWDGx->KEY = (uint32_t)IWDG_WriteAccess;
}

/**
*\*\name    IWDG_SetPrescalerDiv.
*\*\fun     IWDG_Prescaler specifies the IWDG prescaler value.
*\*\param   IWDGx (The input parameters must be the following values):
*\*\          - IWDG1
*\*\          - IWDG2
*\*\param   IWDG_Prescaler :
*\*\          - IWDG_PRESCALER_DIV4
*\*\          - IWDG_PRESCALER_DIV8
*\*\          - IWDG_PRESCALER_DIV16
*\*\          - IWDG_PRESCALER_DIV32
*\*\          - IWDG_PRESCALER_DIV64
*\*\          - IWDG_PRESCALER_DIV128
*\*\          - IWDG_PRESCALER_DIV256
*\*\return  none
**/
void IWDG_SetPrescalerDiv(IWDG_Module* IWDGx, uint8_t IWDG_Prescaler)
{
    IWDGx->PREDIV = IWDG_Prescaler;
}


/**
*\*\name    IWDG_CntReload.
*\*\fun     Sets IWDG reload value.
*\*\param   IWDGx (The input parameters must be the following values):
*\*\          - IWDG1
*\*\          - IWDG2
*\*\param   Reload :
*\*\          -0x000 ~ 0xFFF
*\*\return  none
**/
void IWDG_CntReload(IWDG_Module* IWDGx, uint16_t Reload)
{
    IWDGx->RELV = Reload;
}


/**
*\*\name    IWDG_ReloadKey.
*\*\fun     Reload IWDG counter with value defined in IWDG_RELV register.
*\*\param   IWDGx (The input parameters must be the following values):
*\*\          - IWDG1
*\*\          - IWDG2
*\*\param   none
*\*\return  none
**/
void IWDG_ReloadKey(IWDG_Module* IWDGx)
{
    IWDGx->KEY = KEY_ReloadKey;
}


/**
*\*\name    IWDG_Enable.
*\*\fun     Start watch dog counter.
*\*\param   IWDGx (The input parameters must be the following values):
*\*\          - IWDG1
*\*\          - IWDG2
*\*\param   none
*\*\return  none
**/
void IWDG_Enable(IWDG_Module* IWDGx)
{
    IWDGx->KEY = KEY_EnableKey;
}

/**
*\*\name    IWDG_Freeze_Enable.
*\*\fun     Freeze or unfreeze IWDG while IWDG is running in run mode.
*\*\param   IWDGx (The input parameters must be the following values):
*\*\          - IWDG1
*\*\          - IWDG2
 *\*\param  Cmd :
 *\*\          - ENABLE
 *\*\          - DISABLE
*\*\return  none
**/
void IWDG_Freeze_Enable(IWDG_Module* IWDGx, FunctionalState Cmd)
{
    if(Cmd == ENABLE)
    {
        IWDGx->KEY = IWDG_FREEZE;
    }
    else
    {
        IWDGx->KEY = IWDG_UNFREEZE;
    }

}

/**
*\*\name    IWDG_GetStatus.
*\*\fun     Checks whether the specified IWDG flag is set or not.
*\*\param   IWDGx (The input parameters must be the following values):
*\*\          - IWDG1
*\*\          - IWDG2
*\*\param   IWDG_FLAG :
*\*\          - IWDG_PVU_FLAG
*\*\          - IWDG_CRVU_FLAG
*\*\          - IWDG_FRZF_FLAG
*\*\return  FlagStatus :
*\*\          - RESET
*\*\          - SET
**/
FlagStatus IWDG_GetStatus(IWDG_Module* IWDGx, IWDG_STATUS_FLAG IWDG_FLAG)
{
    FlagStatus bitstatus;

    if ((IWDGx->STS & (uint32_t)IWDG_FLAG) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    /* Return the flag status */
    return bitstatus;
}
