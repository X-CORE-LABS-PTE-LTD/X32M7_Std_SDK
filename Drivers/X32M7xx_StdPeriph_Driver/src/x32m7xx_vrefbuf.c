/**
 ******************************************************************************
 * @file      x32m7xx_vrefbuf.c
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

#include "x32m7xx_vrefbuf.h"

/** VREFBUF Driving Functions Declaration **/

/**
*\*\name    VREFBUF_SetVoltageScale.
*\*\fun     Set the Voltage reference scale.
*\*\param   Scale
*\*\          - VREFBUF_VOLTAGE_SCALE_2_5V
*\*\          - VREFBUF_VOLTAGE_SCALE_2_0V
*\*\          - VREFBUF_VOLTAGE_SCALE_1_8V
*\*\          - VREFBUF_VOLTAGE_SCALE_1_5V
*\*\return  none
**/
void VREFBUF_SetVoltageScale(uint32_t Scale)
{
    uint32_t tempreg;
    /* Get the old register value */
    tempreg = *(uint32_t * )VREFBUF_CTRL2_REG_ADDR;
    /* Clear vrefbuf voltage scale select bits */
    tempreg &= (~VREFBUF_VOLTAGE_SCALE_MASK);
    /* Set vrefbuf voltage scale select bits */
    tempreg |= Scale;
    /* Store the new register value */
    *(uint32_t * )VREFBUF_CTRL2_REG_ADDR = tempreg;
}

/**
*\*\name    VREFBUF_GetVoltageScale.
*\*\fun     Get the Voltage reference scale.
*\*\param   none
*\*\return  the Voltage reference scale;
*\*\          - VREFBUF_VOLTAGE_SCALE_2_5V
*\*\          - VREFBUF_VOLTAGE_SCALE_2_0V
*\*\          - VREFBUF_VOLTAGE_SCALE_1_8V
*\*\          - VREFBUF_VOLTAGE_SCALE_1_5V
**/
uint32_t VREFBUF_GetVoltageScale(void)
{
    /* Clear vrefbuf voltage scale select bits */
    return ( (*(uint32_t * )VREFBUF_CTRL2_REG_ADDR) &VREFBUF_VOLTAGE_SCALE_MASK);
}
/**
*\*\name    VREFBUF_Enable.
*\*\fun     Enable or disable internal voltage reference.
*\*\param   Cmd :
*\*\          - ENABLE
*\*\          - DISABLE
*\*\return  none
**/
void VREFBUF_Enable(FunctionalState Cmd)
{
    if (Cmd != DISABLE)
    {
        *(uint32_t * )VREFBUF_CTRL1_REG_ADDR |= VREFBUF_EN_MASK;
    }
    else
    {
        *(uint32_t * )VREFBUF_CTRL1_REG_ADDR &= (~VREFBUF_EN_MASK);
    }
}

/**
*\*\name    VREFBUF_EnableHIM.
*\*\fun     Enable or disable high impedance (VREF+ pin is high impedance).
*\*\param   Cmd :
*\*\          - ENABLE
*\*\          - DISABLE
*\*\return  none
**/
void VREFBUF_EnableHIM(FunctionalState Cmd)
{
    if (Cmd != DISABLE)
    {
        *(uint32_t * )VREFBUF_CTRL1_REG_ADDR |= VREFBUF_HIM_EN_MASK;
    }
    else
    {
        *(uint32_t * )VREFBUF_CTRL1_REG_ADDR &= (~VREFBUF_HIM_EN_MASK);
    }
}

/**
*\*\name    VREFBUF_SetTrimming.
*\*\fun     Set the trimming code for VREFBUF calibration.
*\*\param   Value :Value Between 0 and 0x3F
*\*\return  none
**/
void VREFBUF_SetTrimming(uint32_t Value)
{
    uint32_t tempreg, tempreg1;
    tempreg = ((*(uint32_t * )VREFBUF_CTRL2_REG_ADDR)&VREFBUF_VOLTAGE_SCALE_MASK);

    if(tempreg == VREFBUF_VOLTAGE_SCALE_2_5V)
    {
        tempreg1 = *(uint32_t * )VREFBUF_TRIM1_REG_ADDR;
        tempreg1 &= (~VREFBUF_TRIMING_2_5V_MASK);
        tempreg1 |= (Value << VREFBUF_TRIMING_2_5V_POS);
        /* Store the new register value */
        *(uint32_t * )VREFBUF_TRIM1_REG_ADDR = tempreg1;
    }
    else if(tempreg == VREFBUF_VOLTAGE_SCALE_2_048V)
    {
        tempreg1 = *(uint32_t * )VREFBUF_TRIM1_REG_ADDR;
        tempreg1 &= (~VREFBUF_TRIMING_2_0V_MASK);
        tempreg1 |= (Value << VREFBUF_TRIMING_2_0V_POS);
        /* Store the new register value */
        *(uint32_t * )VREFBUF_TRIM1_REG_ADDR = tempreg1;
    }
    else if(tempreg == VREFBUF_VOLTAGE_SCALE_1_8V)
    {
        tempreg1 = *(uint32_t * )VREFBUF_TRIM2_REG_ADDR;
        tempreg1 &= (~VREFBUF_TRIMING_1_8V_MASK);
        tempreg1 |= (Value << VREFBUF_TRIMING_1_8V_POS);
        /* Store the new register value */
        *(uint32_t * )VREFBUF_TRIM2_REG_ADDR = tempreg1;
    }
    else
    {
        tempreg1 = *(uint32_t * )VREFBUF_TRIM2_REG_ADDR;
        tempreg1 &= (~VREFBUF_TRIMING_1_5V_MASK);
        tempreg1 |= (Value << VREFBUF_TRIMING_1_5V_POS);
        /* Store the new register value */
        *(uint32_t * )VREFBUF_TRIM2_REG_ADDR = tempreg1;
    }
}

/**
*\*\name    VREFBUF_IsVREFReady.
*\*\fun     Checks whether VREFBUF ready flag is set or not.
*\*\param   none
*\*\return  The new state of VREFBUF_READY (SET or RESET).
**/
FlagStatus VREFBUF_IsVREFReady(void)
{
    return (((*(uint32_t * )VREFBUF_STS_REG_ADDR) & VREFBUF_READY_MASK) != 0U) ? SET : RESET;
}

