/**
 ******************************************************************************
 * @file      x32m7xx_wwdg.c
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

#include "x32m7xx_wwdg.h"
#include "x32m7xx_rcc.h"

/**
*\*\name    WWDG_DeInit.
*\*\fun     Resets the WWDG peripheral registers to their default reset values.
*\*\param   WWDGx (The input parameters must be the following values):
*\*\          - WWDG1
*\*\          - WWDG2
*\*\return  none
**/
void WWDG_DeInit(WWDG_Module* WWDGx)
{
    if (WWDGx == WWDG1)
    {
        RCC_EnableAXIPeriphReset2(RCC_AXI_PERIPHRST_WWDG1);
    }
    else
    {
        if (WWDGx == WWDG2)
        {
            RCC_EnableAPB1PeriphReset2(RCC_APB1_PERIPHRST_WWDG2);
        }
    }
}

/**
*\*\name    WWDG_SetPrescalerDiv.
*\*\fun     Set the WWDOG Prescaler Division Value.
*\*\param   WWDGx (The input parameters must be the following values):
*\*\          - WWDG1
*\*\          - WWDG2
*\*\param   WWDG_Prescaler :
*\*\          - WWDG_PRESCALER_DIV1   WWDG Counter Clock (PCLK1 / 4096) / 1
*\*\          - WWDG_PRESCALER_DIV2   WWDG Counter Clock (PCLK1 / 4096) / 2
*\*\          - WWDG_PRESCALER_DIV4   WWDG Counter Clock (PCLK1 / 4096) / 4
*\*\          - WWDG_PRESCALER_DIV8   WWDG Counter Clock (PCLK1 / 4096) / 8
*\*\return  none
**/
void WWDG_SetPrescalerDiv(WWDG_Module* WWDGx, uint32_t WWDG_Prescaler)
{
    uint32_t tmpregister;

    /* Clear TIMERB[1:0] bits */
    tmpregister = WWDGx->CFG & CFG_TIMERB_MASK;
    /* Set TIMERB[1:0] bits according to WWDG_Prescaler value */
    tmpregister |= WWDG_Prescaler;
    /* Store the new value */
    WWDGx->CFG = tmpregister;
}

/**
*\*\name    WWDG_SetWValue.
*\*\fun     Set the WWDOG Window Value.
*\*\param   WWDGx (The input parameters must be the following values):
*\*\          - WWDG1
*\*\          - WWDG2
*\*\param   WindowValue : WWDOG Window Value
*\*\          The value range of this parameter :
*\*\          - 0x0040 ~ 0x3FFF
*\*\return  none
**/
void WWDG_SetWValue(WWDG_Module* WWDGx, uint16_t WindowValue)
{
    __IO uint32_t tmpregister = 0;

    /* Clear W[13:0] bits */
    tmpregister = WWDGx->CFG & CFG_W_MASK;

    /* Set W[13:0] bits according to WindowValue value */
    tmpregister |= WindowValue & (uint32_t)BIT_MASK;

    /* Store the new value */
    WWDGx->CFG = tmpregister;
}

/**
*\*\name    WWDG_EnableInt.
*\*\fun     Enable WWDG Early Wakeup interrupt(EWINT).
*\*\param   WWDGx (The input parameters must be the following values):
*\*\          - WWDG1
*\*\          - WWDG2
*\*\return  none
**/
void WWDG_EnableInt(WWDG_Module* WWDGx)
{
    WWDGx->CFG |= EARLY_WAKEUP_INT;
}

/**
*\*\name    WWDG_SetCnt.
*\*\fun     Set the WWDOG Counter Value.
*\*\param   WWDGx (The input parameters must be the following values):
*\*\          - WWDG1
*\*\          - WWDG2
*\*\param   Counter : WWDOG Counter value
*\*\          The value range of this parameter :
*\*\          - 0x40 ~ 0x3FFF
*\*\return  none
**/
void WWDG_SetCnt(WWDG_Module* WWDGx, uint16_t Counter)
{
    /* Write to T[6:0] bits to configure the counter value, no need to do
       a read-modify-write; writing a 0 to WDGA bit does nothing */
    WWDGx->CTRL = (uint32_t)Counter & (uint32_t)BIT_MASK;
}

/**
*\*\name    WWDG_Enable.
*\*\fun     Set the WWDOG Counter Value and Enable WWDOG .
*\*\param   WWDGx (The input parameters must be the following values):
*\*\          - WWDG1
*\*\          - WWDG2
*\*\param   Counter : WWDOG Counter value
*\*\          The value range of this parameter :
*\*\          - 0x40 ~ 0x3FFF
*\*\return  none
**/
//uint32_t data;
void WWDG_Enable(WWDG_Module* WWDGx, uint16_t Counter)
{
    //data=CTRL_ACTB_SET | Counter;
    WWDGx->CTRL = CTRL_ACTB_SET | Counter;
}

/**
*\*\name    WWDG_GetEWINTF.
*\*\fun     Get WWDOG Early Wake-up Interrupt Flag.
*\*\param   WWDGx (The input parameters must be the following values):
*\*\          - WWDG1
*\*\          - WWDG2
*\*\param   none
*\*\return  SET or RESET
**/
FlagStatus WWDG_GetEWINTF(WWDG_Module* WWDGx)
{
    FlagStatus bitstatus;

    /* Return the status of the flag */
    if ((WWDGx->STS & EARLY_WAKEUP_FLAG) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    return bitstatus;
}

/**
*\*\name    WWDG_ClrEWINTF.
*\*\fun     Clear WWDOG Early Wake-up Interrupt Flag.
*\*\param   WWDGx (The input parameters must be the following values):
*\*\          - WWDG1
*\*\          - WWDG2
*\*\return  none
**/
void WWDG_ClrEWINTF(WWDG_Module* WWDGx)
{
    WWDGx->STS = (uint32_t)RESET;
}
