/**
 ******************************************************************************
 * @file      x32m7xx_dcmu.c
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

#include "x32m7xx_dcmu.h"
#include "x32m7xx_rcc.h"

/* DCMU Driving Functions Declaration */


/* DCMU_Private_Defines  */
/**
*\*\name    DCMU_Reset.
*\*\fun     Resets the DCMUx peripheral
*\*\param   none
*\*\return  none
**/
void DCMU_DeInit( void )
{
    RCC_EnableDCMUReset();
}

#if defined(CORE_CM7)
/**
*\*\name    DCMU_SelfReset.
*\*\fun     Self-resets the DCMUx peripheral by CM7 or CM4
*\*\param   none
*\*\return  none
**/
void DCMU_SelfReset( void )
{
    DCMU->CTRL |= DCMU_CTRL_RST_MASK;
    __NOP();
    __NOP();

    /* Wait for CPU reset bit in DCMU status register is completed */
    while( 0U != (DCMU->STS & DCMU_STS_RSTF_MASK) )
    {
        __NOP();
    }
}
#endif

/**
*\*\name    DCMU_TransmitMsg.
*\*\fun     Transmit Message through Blocking or NonBlocking Mode to counterpart MCU
*\*\param   TxIdx :
*\*\          - TXMSG_IDX0
*\*\          - TXMSG_IDX1
*\*\          - TXMSG_IDX2
*\*\          - TXMSG_IDX3
*\*\param   TxMsgData :
*\*\          - 32bit unsigned integer message data payload [0x00000000U--0xFFFFFFFFU]
*\*\param   Mode :
*\*\          - NON_BLOCKING
*\*\          - BLOCKING
*\*\return  none
**/
void DCMU_TransmitMsg( uint8_t TxIdx, uint32_t TxMsgData, dcmu_wait_mode_t Mode)
{
    /* Wait until transmit message register is empty in blocking receive mode */
    if( BLOCKING == Mode)
    {
        while( 0u == (DCMU->STS & (1U << (DCMU_STS_TEF3_Pos + 3U - TxIdx))))
        {
            __NOP();
        }
    }

    /* NonBlocking and Blocking common code */
    DCMU->TXMSG[TxIdx] = TxMsgData;
}

/**
*\*\name    DCMU_ReceiveMsg.
*\*\fun     Receive Message through Blocking or NonBlocking Mode from counterpart mcu
*\*\param   RcvIdx :
*\*\          - RCVMSG_IDX0
*\*\          - RCVMSG_IDX1
*\*\          - RCVMSG_IDX2
*\*\          - RCVMSG_IDX3
*\*\param   Mode :
*\*\          - NON_BLOCKING
*\*\          - BLOCKING
*\*\return  uint32_t message data from counterpart mcu
**/
uint32_t DCMU_ReceiveMsg( uint8_t RcvIdx, dcmu_wait_mode_t Mode)
{
    /* Wait until receive message register is full in blocking receive mode */
    if( BLOCKING == Mode)
    {
        while( 0u == (DCMU->STS & (1U << (DCMU_STS_RFF3_Pos + 3U - RcvIdx))))
        {
            __NOP();
        }
    }

    /* NonBlocking and Blocking mode common code */
    return DCMU->RCVMSG[RcvIdx];
}

/**
*\*\name    DCMU_Set3BitFlagNumbers.
*\*\fun     Set three bits flag number in order to transmit to counterpart
*\*\param   FlagData :
*\*\          - 3 bits flag number data [0x0U - 0x7U]
*\*\param   Mode :
*\*\          - NON_BLOCKING
*\*\          - BLOCKING
*\*\return  none
**/
void DCMU_Set3BitFlagNumbers( uint8_t FlagData, dcmu_wait_mode_t Mode)
{
    __IO uint32_t tempreg;

    /* Wait until receive flag is pending in case of blocking receive mode */
    if( BLOCKING == Mode)
    {
        while( 0u != (DCMU->STS & DCMU_STS_FUPF_MASK ))
        {
            __NOP();
        }
    }

    /* NonBlocking and Blocking mode common code */
    tempreg = DCMU->CTRL;
    tempreg &= ~(uint32_t)(DCMU_CTRL_TXFN_ALL_MASK);
    tempreg |= (uint32_t)FlagData;
    DCMU->CTRL = tempreg;

}

/**
*\*\name    DCMU_Get3BitFlagNumbers.
*\*\fun     Get three bits flag number received from counterpart
*\*\param   none
*\*\return  uint8_t received 3bits flag number data from counterpart mcu [0x00U-0x07U]
**/
uint8_t DCMU_Get3BitFlagNumbers( void)
{
    return (uint8_t)(DCMU->STS & (uint32_t)( DCMU_STS_RCVFN_ALL_MASK));
}

/**
*\*\name    DCMU_GetStatusFlags.
*\*\fun     Get status flags from status register, only FUPF, EPF and RSTF flags are captured
*\*\param   StatusMask :
*\*\          - DCMU_STS_EPF_MASK
*\*\          - DCMU_STS_FUPF_MASK
*\*\          - DCMU_STS_RSTF_MASK
*\*\return  uint32_t the existing status flags
**/
uint32_t DCMU_GetStatusFlags( uint32_t StatusMask)
{
    DCMU_Module *DCMUx = DCMU;
    return DCMUx->STS & (uint32_t)(( DCMU_STS_EPF_MASK | DCMU_STS_RSTF_MASK | DCMU_STS_FUPF_MASK) & StatusMask);
}

/**
*\*\name    DCMU_GetIntPendingFlags.
*\*\fun     Get interrupt pendding flags from status register and GPIF, TEIF and RFIF flags are captured
*\*\param   IntPendingMask :
*\*\          - DCMU_STS_GPIF0_MASK
*\*\          - DCMU_STS_GPIF1_MASK
*\*\          - DCMU_STS_GPIF2_MASK
*\*\          - DCMU_STS_GPIF3_MASK
*\*\          - DCMU_STS_RFF0_MASK
*\*\          - DCMU_STS_RFF1_MASK
*\*\          - DCMU_STS_RFF2_MASK
*\*\          - DCMU_STS_RFF3_MASK
*\*\          - DCMU_STS_TEF0_MASK
*\*\          - DCMU_STS_TEF1_MASK
*\*\          - DCMU_STS_TEF2_MASK
*\*\          - DCMU_STS_TEF3_MASK
*\*\return  uint32_t the existing interrupt pending flags which are in range of IntPendingMask
**/
uint32_t DCMU_GetIntPendingFlags( uint32_t IntPendingMask)
{

    return DCMU->STS & (uint32_t)( IntPendingMask &  \
                                   ( DCMU_STS_GPIF_ALL_MASK | \
                                     DCMU_STS_TEF_ALL_MASK | \
                                     DCMU_STS_RFF_ALL_MASK   )  );
}

/**
*\*\name    GPIntPendingMask.
*\*\fun     Clear general purpose interrupt pending as GPIF
*\*\param   GPIntPendingMask :
*\*\          - DCMU_STS_GPIF0_MASK
*\*\          - DCMU_STS_GPIF1_MASK
*\*\          - DCMU_STS_GPIF2_MASK
*\*\          - DCMU_STS_GPIF3_MASK
*\*\return  none
**/
void DCMU_ClearIntPendingFlags( uint32_t GPIntPendingMask)
{
    __IO uint32_t tempreg;

    tempreg = DCMU->STS;
    tempreg &= ~(uint32_t)(DCMU_STS_GPIF_ALL_MASK);
    tempreg |= (uint32_t)(GPIntPendingMask & (uint32_t)(DCMU_STS_GPIF_ALL_MASK));
    DCMU->STS = tempreg;
}

/**
*\*\name    DCMU_ConfigInt.
*\*\fun     Configure individual interrupts as enable or disabled
*\*\param   IntMask :
*\*\          - DCMU_CTRL_GPIE0_MASK
*\*\          - DCMU_CTRL_GPIE1_MASK
*\*\          - DCMU_CTRL_GPIE2_MASK
*\*\          - DCMU_CTRL_GPIE3_MASK
*\*\          - DCMU_CTRL_RFIE0_MASK
*\*\          - DCMU_CTRL_RFIE1_MASK
*\*\          - DCMU_CTRL_RFIE2_MASK
*\*\          - DCMU_CTRL_RFIE3_MASK
*\*\          - DCMU_CTRL_TEIE0_MASK
*\*\          - DCMU_CTRL_TEIE1_MASK
*\*\          - DCMU_CTRL_TEIE2_MASK
*\*\          - DCMU_CTRL_TEIE3_MASK
*\*\param   Cmd :
*\*\          - ENABLE
*\*\          - DISABLE
*\*\return  none
**/
void DCMU_ConfigInt( uint32_t IntMask, FunctionalState Cmd)
{
    __IO uint32_t tempreg;

    tempreg = DCMU->CTRL;

    if( ENABLE == Cmd)
    {
        tempreg |= (uint32_t)(IntMask & DCMU_CTRL_ALL_IE_MASK);
    }
    else
    {
        tempreg &= ~(uint32_t)IntMask;
    }

    DCMU->CTRL = tempreg;
}

/**
*\*\name    DCMU_RequestGPInt.
*\*\fun     Send request general purpose interrupt to counterpart mcu
*\*\param   ReqGPIntMask :
*\*\          - DCMU_CTRL_GPIR0_MASK
*\*\          - DCMU_CTRL_GPIR1_MASK
*\*\          - DCMU_CTRL_GPIR2_MASK
*\*\          - DCMU_CTRL_GPIR3_MASK
*\*\return  none
**/
void DCMU_RequestGPInt( uint32_t ReqGPIntMask)
{
    __IO uint32_t tempreg;

    tempreg = DCMU->CTRL;
    tempreg &= (~DCMU_CTRL_GPIR_ALL_MASK );
    tempreg |= ReqGPIntMask  ;

    DCMU->CTRL = tempreg;
}
