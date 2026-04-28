/**
 ******************************************************************************
 * @file      x32m7xx_eccmon.c
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

#include "x32m7xx_eccmon.h"

/**
*\*\brief  Get ECCMON Monitor Type
*\*\param  ECCON_OneMemType ECCMON One Memory Type ,can'T |(or) operation
*\*\             -ECCMON_AXI_SRAM1
*\*\             -ECCMON_AXI_SRAM2
*\*\             -ECCMON_AXI_SRAM3
*\*\             -ECCMON_ITCM
*\*\             -ECCMON_D0TCM
*\*\             -ECCMON_D1TCM
*\*\             -ECCMON_AHB_SRAM1
*\*\             -ECCMON_AHB_SRAM2
*\*\             -ECCMON_AHB_SRAM3
*\*\             -ECCMON_AHB_SRAM4
*\*\             -ECCMON_AHB_SRAM5_BANK1
*\*\             -ECCMON_AHB_SRAM5_BANK2
*\*\             -ECCMON_BACKUP_SRAM
*\*\return ECCMON_MonitorType  Monitor Unit and Index for the MemType
**/
static ECCMON_MonitorType ECCMON_GetMonitorType(uint32_t ECCON_OneMemType)
{
    ECCMON_MonitorType oneMonitor;

    switch (ECCON_OneMemType)
    {
        case ECCMON_AXI_SRAM1:
            oneMonitor.MonitorUnit = ECCMON1P1;
            oneMonitor.MonitorIndex = 1;
            break;

        case ECCMON_AXI_SRAM2:
            oneMonitor.MonitorUnit = ECCMON1P2;
            oneMonitor.MonitorIndex = 0;
            break;

        case ECCMON_AXI_SRAM3:
            oneMonitor.MonitorUnit = ECCMON1P2;
            oneMonitor.MonitorIndex = 1;
            break;

        case ECCMON_ITCM:
            oneMonitor.MonitorUnit = ECCMON1P3;
            oneMonitor.MonitorIndex = 0;
            break;

        case ECCMON_D0TCM:
            oneMonitor.MonitorUnit = ECCMON1P4;
            oneMonitor.MonitorIndex = 0;
            break;

        case ECCMON_D1TCM:
            oneMonitor.MonitorUnit = ECCMON1P4;
            oneMonitor.MonitorIndex = 1;
            break;

        case ECCMON_AHB_SRAM1:
            oneMonitor.MonitorUnit = ECCMON2;
            oneMonitor.MonitorIndex = 0;
            break;

        case ECCMON_AHB_SRAM2:
            oneMonitor.MonitorUnit = ECCMON2;
            oneMonitor.MonitorIndex = 1;
            break;

        case ECCMON_AHB_SRAM3:
            oneMonitor.MonitorUnit = ECCMON2;
            oneMonitor.MonitorIndex = 2;
            break;

        case ECCMON_AHB_SRAM4:
            oneMonitor.MonitorUnit = ECCMON2;
            oneMonitor.MonitorIndex = 3;
            break;

        case ECCMON_AHB_SRAM5_BANK1:
            oneMonitor.MonitorUnit = ECCMON2;
            oneMonitor.MonitorIndex = 4;
            break;

        case ECCMON_AHB_SRAM5_BANK2:
            oneMonitor.MonitorUnit = ECCMON2;
            oneMonitor.MonitorIndex = 5;
            break;

        case ECCMON_BACKUP_SRAM:
            oneMonitor.MonitorUnit = ECCMON3;
            oneMonitor.MonitorIndex = 0;
            break;

        default:
            break;
    }

    return oneMonitor;
}

/**
*\*\brief  Initialize the ECCMON structure
*\*\param  ECCMON_InitStruct ECCMON handle
*\*\           -MemType
*\*\           -CaptureSelect
*\*\           -ErrFlagOut
*\*\return none
**/
void ECCMON_StructInit(ECCMON_InitType* ECCMON_InitStruct)
{
    ECCMON_InitStruct->MemType = ECCMON_NONE_SRAM;
    ECCMON_InitStruct->CaptureSelect = ECCMON_CAPTURE_1_2BIT_ERROR;
    ECCMON_InitStruct->ErrFlagOut = ECCMON_NO_ERROR_FLAG_OUT;
}

/**
*\*\brief  Initialize the ECCMON according to the specified parameters in the ECCMON_InitType.
*\*\param  ECCMON_InitStruct ECCMON handle
*\*\         -MemType
*\*\             -ECCMON_AXI_SRAM1
*\*\             -ECCMON_AXI_SRAM2
*\*\             -ECCMON_AXI_SRAM3
*\*\             -ECCMON_ITCM
*\*\             -ECCMON_D0TCM
*\*\             -ECCMON_D1TCM
*\*\             -ECCMON_AHB_SRAM1
*\*\             -ECCMON_AHB_SRAM2
*\*\             -ECCMON_AHB_SRAM3
*\*\             -ECCMON_AHB_SRAM4
*\*\             -ECCMON_AHB_SRAM5_BANK1
*\*\             -ECCMON_AHB_SRAM5_BANK2
*\*\             -ECCMON_BACKUP_SRAM
*\*\         -CaptureSelect
*\*\             -ECCMON_CAPTURE_1_2BIT_ERROR
*\*\             -ECCMON_CAPTURE_1BIT_ERROR
*\*\             -ECCMON_CAPTURE_2BIT_ERROR
*\*\             -ECCMON_CAPTURE_NONE_ERROR
*\*\         -ErrFlagOut
*\*\             -ECCMON_NO_ERROR_FLAG_OUT
*\*\             -ECCMON_1BIT_ERROR_FLAG_OUT
*\*\             -ECCMON_2BIT_ERROR_FLAG_OUT
*\*\             -ECCMON_1_2BIT_ERROR_FLAG_OUT
*\*\return none
**/
void ECCMON_Init(ECCMON_InitType* ECCMON_InitStruct)
{
    ECCMON_MonitorType oneMonitorType;

    for (uint8_t index = 0; index < 14U; index++)
    {
        if ((ECCMON_InitStruct->MemType & (1UL << index)) != 0x00UL)
        {
            //Get ECCMON Monitor unit and index
            oneMonitorType = ECCMON_GetMonitorType(ECCMON_InitStruct->MemType & (1UL << index));
            //set capture select
            oneMonitorType.MonitorUnit->EINJ &= ~(ECCMON_CAPTURE_NONE_ERROR << (oneMonitorType.MonitorIndex * 2UL));
            oneMonitorType.MonitorUnit->EINJ |= ECCMON_InitStruct->CaptureSelect << (oneMonitorType.MonitorIndex * 2UL);

            //set eror flag out to FLAG_EN_OT
            if ((ECCMON_InitStruct->ErrFlagOut & 0x01UL) != 0x00UL)
            {
                oneMonitorType.MonitorUnit->CTRL2 |= ECCMON_CTRL2_E1FOEN1 << oneMonitorType.MonitorIndex;
            }
            else
            {
                oneMonitorType.MonitorUnit->CTRL2 &= ~(ECCMON_CTRL2_E1FOEN1 << oneMonitorType.MonitorIndex);
            }

            if ((ECCMON_InitStruct->ErrFlagOut & 0x02UL) != 0x00UL)
            {
                oneMonitorType.MonitorUnit->CTRL2 |= ECCMON_CTRL2_E2FOEN1 << oneMonitorType.MonitorIndex;
            }
            else
            {
                oneMonitorType.MonitorUnit->CTRL2 &= ~(ECCMON_CTRL2_E2FOEN1 << oneMonitorType.MonitorIndex);
            }
        }

    }

}
/**
*\*\brief  Initialize the ECCMON interrupt
*\*\param  ECCMON_MemType ECCMON Memory Type ,can |(or) operation
*\*\             -ECCMON_AXI_SRAM1
*\*\             -ECCMON_AXI_SRAM2
*\*\             -ECCMON_AXI_SRAM3
*\*\             -ECCMON_ITCM
*\*\             -ECCMON_D0TCM
*\*\             -ECCMON_D1TCM
*\*\             -ECCMON_AHB_SRAM1
*\*\             -ECCMON_AHB_SRAM2
*\*\             -ECCMON_AHB_SRAM3
*\*\             -ECCMON_AHB_SRAM4
*\*\             -ECCMON_AHB_SRAM5_BANK1
*\*\             -ECCMON_AHB_SRAM5_BANK2
*\*\             -ECCMON_BACKUP_SRAM
*\*\param  ECCMON_IntType ECCMON intertupt Type
*\*\             -ECCMON_1BIT_ERROR_INT
*\*\             -ECCMON_2BIT_ERROR_INT
*\*\             -ECCMON_1_2BIT_ERROR_INT
*\*\return none
**/
void ECCMON_ConfigInt(uint32_t ECCMON_MemType, uint32_t ECCMON_IntType)
{
    ECCMON_MonitorType oneMonitorType;

    for (uint8_t index = 0; index < 14U; index++)
    {
        if ((ECCMON_MemType & (1UL << index)) != 0x00UL)
        {
            //Get ECCMON Monitor unit and index
            oneMonitorType = ECCMON_GetMonitorType(ECCMON_MemType & (1UL << index));

            //set eror interrupt
            if ((ECCMON_IntType & 0x01UL) != 0x00UL)
            {
                oneMonitorType.MonitorUnit->CTRL1 |= ECCMON_CTRL1_E1INTEN1 << oneMonitorType.MonitorIndex;
            }
            else
            {
                oneMonitorType.MonitorUnit->CTRL1 &= ~(ECCMON_CTRL1_E1INTEN1 << oneMonitorType.MonitorIndex);
            }

            if ((ECCMON_IntType & 0x02UL) != 0x00UL)
            {
                oneMonitorType.MonitorUnit->CTRL1 |= ECCMON_CTRL1_E2INTEN1 << oneMonitorType.MonitorIndex;
            }
            else
            {
                oneMonitorType.MonitorUnit->CTRL1 &= ~(ECCMON_CTRL1_E2INTEN1 << oneMonitorType.MonitorIndex);
            }

        }

    }
}

/**
*\*\brief  Inject Error for test
*\*\param  ECCMON_MemType ECCMON Memory Type ,can |(or) operation
*\*\             -ECCMON_AXI_SRAM1
*\*\             -ECCMON_AXI_SRAM2
*\*\             -ECCMON_AXI_SRAM3
*\*\             -ECCMON_ITCM
*\*\             -ECCMON_D0TCM
*\*\             -ECCMON_D1TCM
*\*\             -ECCMON_AHB_SRAM1
*\*\             -ECCMON_AHB_SRAM2
*\*\             -ECCMON_AHB_SRAM3
*\*\             -ECCMON_AHB_SRAM4
*\*\             -ECCMON_AHB_SRAM5_BANK1
*\*\             -ECCMON_AHB_SRAM5_BANK2
*\*\             -ECCMON_BACKUP_SRAM
*\*\param  ECCMON_InjectType ECCMON Inject Type
*\*\             -ECCMON_NO_INJECT
*\*\             -ECCMON_INJECT_1BIT
*\*\             -ECCMON_INJECT_2BIT
*\*\return none
**/
void ECCMON_InjectError(uint32_t ECCMON_MemType, uint32_t ECCMON_InjectType)
{
    ECCMON_MonitorType oneMonitorType;

    for (uint8_t index = 0; index < 14U; index++)
    {
        if ((ECCMON_MemType & (1UL << index)) != 0x00UL)
        {
            //Get ECCMON Monitor unit and index
            oneMonitorType = ECCMON_GetMonitorType(ECCMON_MemType & (1UL << index));

            //set eror inject
            oneMonitorType.MonitorUnit->EINJ &= ~(ECCMON_INJECT_MASK << (2UL * oneMonitorType.MonitorIndex));
            oneMonitorType.MonitorUnit->EINJ |= ECCMON_InjectType << (2UL * oneMonitorType.MonitorIndex);
        }

    }
}
/**
*\*\brief  set Temporary data register bypass
*\*\       temporary data register is used to temporarily store read-modify-write data on partial write access
*\*\       the temporary data register is bypassed, the read-modify-write data is written back the memory
*\*\param  ECCMON_MemType ECCMON Memory Type ,can |(or) operation
*\*\             -ECCMON_AHB_SRAM1
*\*\             -ECCMON_AHB_SRAM2
*\*\             -ECCMON_AHB_SRAM3
*\*\             -ECCMON_AHB_SRAM4
*\*\             -ECCMON_AHB_SRAM5_BANK1
*\*\             -ECCMON_AHB_SRAM5_BANK2
*\*\param  FunctionalState Cmd
*\*\             -DISABLE
*\*\             -ENABLE
*\*\return none
**/
void ECCMON_SetBypassTempRegister(uint32_t ECCMON_MemType, FunctionalState Cmd)
{
    ECCMON_MonitorType oneMonitorType;

    for (uint8_t index = 0; index < 14U; index++)
    {
        if ((ECCMON_MemType & (1UL << index)) != 0x00UL)
        {
            //Get ECCMON Monitor unit and index
            oneMonitorType = ECCMON_GetMonitorType(ECCMON_MemType & (1UL << index));

            if (Cmd != DISABLE)
            {
                //set bypass
                oneMonitorType.MonitorUnit->CTRL1 |= ECCMON_CTRL1_TDRBYP1 << oneMonitorType.MonitorIndex;
            }
            else
            {
                oneMonitorType.MonitorUnit->CTRL1 &= ~(ECCMON_CTRL1_TDRBYP1 << oneMonitorType.MonitorIndex);
            }
        }

    }

}

/**
*\*\brief  set Partial write on ECC 2-bit enable or disable
*\*\       allow or don't allow partial write to perform read-modify-write to the memory while ECC 2-bit error is detected.
*\*\param  ECCMON_MemType ECCMON Memory Type ,can |(or) operation
*\*\             -ECCMON_AXI_SRAM1
*\*\             -ECCMON_AXI_SRAM2
*\*\             -ECCMON_AXI_SRAM3
*\*\             -ECCMON_ITCM
*\*\             -ECCMON_D0TCM
*\*\             -ECCMON_D1TCM
*\*\             -ECCMON_AHB_SRAM1
*\*\             -ECCMON_AHB_SRAM2
*\*\             -ECCMON_AHB_SRAM3
*\*\             -ECCMON_AHB_SRAM4
*\*\             -ECCMON_AHB_SRAM5_BANK1
*\*\             -ECCMON_AHB_SRAM5_BANK2
*\*\             -ECCMON_BACKUP_SRAM
*\*\param  FunctionalState Cmd
*\*\             -DISABLE
*\*\             -ENABLE
*\*\return none
**/
void ECCMON_SetWriteOn2Bit(uint32_t ECCMON_MemType, FunctionalState Cmd)
{
    ECCMON_MonitorType oneMonitorType;

    for (uint8_t index = 0; index < 14U; index++)
    {
        if ((ECCMON_MemType & (1UL << index)) != 0x00UL)
        {
            //Get ECCMON Monitor unit and index
            oneMonitorType = ECCMON_GetMonitorType(ECCMON_MemType & (1UL << index));

            if (Cmd != DISABLE)
            {
                //set write on ECC 2-bit enable
                oneMonitorType.MonitorUnit->CTRL2 |= ECCMON_CTRL2_WROE2EN1 << oneMonitorType.MonitorIndex;
            }
            else
            {
                oneMonitorType.MonitorUnit->CTRL2 &= ~(ECCMON_CTRL2_WROE2EN1 << oneMonitorType.MonitorIndex);
            }
        }

    }

}
/**
*\*\brief  set enable ECCMON
*\*\param  ECCMON_MemType ECCMON Memory Type ,can |(or) operation
*\*\             -ECCMON_AXI_SRAM1
*\*\             -ECCMON_AXI_SRAM2
*\*\             -ECCMON_AXI_SRAM3
*\*\             -ECCMON_ITCM
*\*\             -ECCMON_D0TCM
*\*\             -ECCMON_D1TCM
*\*\             -ECCMON_AHB_SRAM1
*\*\             -ECCMON_AHB_SRAM2
*\*\             -ECCMON_AHB_SRAM3
*\*\             -ECCMON_AHB_SRAM4
*\*\             -ECCMON_AHB_SRAM5_BANK1
*\*\             -ECCMON_AHB_SRAM5_BANK2
*\*\             -ECCMON_BACKUP_SRAM
*\*\param   Cmd (The input parameters must be the following values):
*\*\          - ENABLE
*\*\          - DISABLE
*\*\return none
**/
void ECCMON_Enable(uint32_t ECCMON_MemType, FunctionalState Cmd)
{
    ECCMON_MonitorType oneMonitorType;

    for (uint8_t index = 0; index < 14U; index++)
    {
        if ((ECCMON_MemType & (1UL << index)) != 0x00UL)
        {
            //Get ECCMON Monitor unit and index
            oneMonitorType = ECCMON_GetMonitorType(ECCMON_MemType & (1UL << index));

            if (Cmd != DISABLE)
            {
                //set ECC enable
                oneMonitorType.MonitorUnit->CTRL1 |= ECCMON_CTRL1_ECCEN1 << oneMonitorType.MonitorIndex;
            }
            else
            {
                //set ECC enable
                oneMonitorType.MonitorUnit->CTRL1 &= ~(ECCMON_CTRL1_ECCEN1 << oneMonitorType.MonitorIndex);
            }
        }

    }
}
/**
*\*\brief  set enable ECCMON CLOCK
*\*\param  none
*\*\return none
**/
void ECCMON_EnableClk(void)
{
    /**note:No matter which ECCMON you want to use, you need to enable the three ECCMON clocks***/
    RCC_EnableAXIPeriphClk1(RCC_AXI_PERIPHEN_M7_ECCM1 | RCC_AXI_PERIPHEN_M4_ECCM1, ENABLE);
    RCC_EnableAHB2PeriphClk1(RCC_AHB2_PERIPHEN_M7_ECCM2 | RCC_AHB2_PERIPHEN_M4_ECCM2, ENABLE);
    RCC_EnableAHB5PeriphClk2(RCC_AHB5_PERIPHEN_M7_ECCM3 | RCC_AHB5_PERIPHEN_M4_ECCM3, ENABLE);

}
/**
*\*\brief  Get the ECCMON Fail Address
*\*\param  ECCMON_MemType ECCMON Memory Type ,can not |(or) operation
*\*\             -ECCMON_AXI_SRAM1
*\*\             -ECCMON_AXI_SRAM2
*\*\             -ECCMON_AXI_SRAM3
*\*\             -ECCMON_ITCM
*\*\             -ECCMON_D0TCM
*\*\             -ECCMON_D1TCM
*\*\             -ECCMON_AHB_SRAM1
*\*\             -ECCMON_AHB_SRAM2
*\*\             -ECCMON_AHB_SRAM3
*\*\             -ECCMON_AHB_SRAM4
*\*\             -ECCMON_AHB_SRAM5_BANK1
*\*\             -ECCMON_AHB_SRAM5_BANK2
*\*\             -ECCMON_BACKUP_SRAM
*\*\return  FailAddress
**/
uint32_t ECCMON_GetFailAddress(uint32_t ECCMON_MemType)
{
    uint32_t readAddr, FailAddress ;
    ECCMON_MonitorType oneMonitorType;
    //Get ECCMON Monitor unit and index
    oneMonitorType = ECCMON_GetMonitorType(ECCMON_MemType);

    /* read  Fail Address */
    readAddr = (*((&oneMonitorType.MonitorUnit->FEADR1) + (ECCMON_GROUP_REG_ADDR_GAP * oneMonitorType.MonitorIndex))) & 0xFFFFFFFUL;

    //shift left by 2 bit for 32-bit memory, and shift left by 3 bit for 64-bit memory
    switch (ECCMON_MemType)
    {
        case ECCMON_AXI_SRAM1://64 bit data
            FailAddress = AXI_SRAM1_BASE_ADDR + (readAddr << 3);
            break;

        case ECCMON_AXI_SRAM2://64 bit data
            FailAddress = AXI_SRAM2_BASE_ADDR + (readAddr << 3);
            break;

        case ECCMON_AXI_SRAM3://64 bit data
            FailAddress = AXI_SRAM3_BASE_ADDR + (readAddr << 3);
            break;

        case ECCMON_ITCM://64 bit data
            FailAddress = ITCM_BASE_ADDR + (readAddr << 3);
            break;

        case ECCMON_D0TCM://32 bit data
            FailAddress = DTCM_BASE_ADDR + (readAddr << 2);
            break;

        case ECCMON_D1TCM://32 bit data
            FailAddress = DTCM_BASE_ADDR + (readAddr << 2);
            break;

        case ECCMON_AHB_SRAM1://32 bit data
            FailAddress = AHB_SRAM1_BASE_ADDR + (readAddr << 2);
            break;

        case ECCMON_AHB_SRAM2://32 bit data
            FailAddress = AHB_SRAM2_BASE_ADDR + (readAddr << 2);
            break;

        case ECCMON_AHB_SRAM3://32 bit data
            FailAddress = AHB_SRAM3_BASE_ADDR + (readAddr << 2);
            break;

        case ECCMON_AHB_SRAM4://32 bit data
            FailAddress = AHB_SRAM4_BASE_ADDR + (readAddr << 2);
            break;

        case ECCMON_AHB_SRAM5_BANK1://32 bit data
            FailAddress = AHB_SRAM5_BANK1_BASE_ADDR + (readAddr << 2);
            break;

        case ECCMON_AHB_SRAM5_BANK2://32 bit data
            FailAddress = AHB_SRAM5_BANK2_BASE_ADDR + (readAddr << 2);
            break;

        case ECCMON_BACKUP_SRAM://32 bit data
            FailAddress = BACKUP_SRAM_BASE_ADDR + (readAddr << 2);
            break;

        default:
            FailAddress = readAddr;
            break;
    }

    return FailAddress;
}
/**
*\*\brief  Get the ECCMON Fail Data Low
*\*\param  ECCMON_MemType ECCMON Memory Type ,can not |(or) operation
*\*\             -ECCMON_AXI_SRAM1
*\*\             -ECCMON_AXI_SRAM2
*\*\             -ECCMON_AXI_SRAM3
*\*\             -ECCMON_ITCM
*\*\             -ECCMON_D0TCM
*\*\             -ECCMON_D1TCM
*\*\             -ECCMON_AHB_SRAM1
*\*\             -ECCMON_AHB_SRAM2
*\*\             -ECCMON_AHB_SRAM3
*\*\             -ECCMON_AHB_SRAM4
*\*\             -ECCMON_AHB_SRAM5_BANK1
*\*\             -ECCMON_AHB_SRAM5_BANK2
*\*\             -ECCMON_BACKUP_SRAM
*\*\return  Fail Data Low
**/
uint32_t ECCMON_GetFailDataLow(uint32_t ECCMON_MemType)
{
    ECCMON_MonitorType oneMonitorType;
    //Get ECCMON Monitor unit and index
    oneMonitorType = ECCMON_GetMonitorType(ECCMON_MemType);
    /* Return Fail Address */
    return (uint32_t) * ((&oneMonitorType.MonitorUnit->FEDATL1) + (ECCMON_GROUP_REG_ADDR_GAP * oneMonitorType.MonitorIndex));
}

/**
*\*\brief  Get the ECCMON Fail Data High
*\*\param  ECCMON_MemType ECCMON Memory Type ,can not |(or) operation
*\*\             -ECCMON_AXI_SRAM1
*\*\             -ECCMON_AXI_SRAM2
*\*\             -ECCMON_AXI_SRAM3
*\*\             -ECCMON_ITCM
*\*\             -ECCMON_D0TCM
*\*\             -ECCMON_D1TCM
*\*\             -ECCMON_AHB_SRAM1
*\*\             -ECCMON_AHB_SRAM2
*\*\             -ECCMON_AHB_SRAM3
*\*\             -ECCMON_AHB_SRAM4
*\*\             -ECCMON_AHB_SRAM5_BANK1
*\*\             -ECCMON_AHB_SRAM5_BANK2
*\*\             -ECCMON_BACKUP_SRAM
*\*\return  Fail Data High
**/
uint32_t ECCMON_GetFailDataHigh(uint32_t ECCMON_MemType)
{
    ECCMON_MonitorType oneMonitorType;
    //Get ECCMON Monitor unit and index
    oneMonitorType = ECCMON_GetMonitorType(ECCMON_MemType);
    /* Return Fail Address */
    return (uint32_t) * ((&oneMonitorType.MonitorUnit->FEDATH1) + (ECCMON_GROUP_REG_ADDR_GAP * oneMonitorType.MonitorIndex));
}

/**
*\*\brief  Get the ECCMON Fail ECC code
*\*\param  ECCMON_MemType ECCMON Memory Type ,can not |(or) operation
*\*\             -ECCMON_AXI_SRAM1
*\*\             -ECCMON_AXI_SRAM2
*\*\             -ECCMON_AXI_SRAM3
*\*\             -ECCMON_ITCM
*\*\             -ECCMON_D0TCM
*\*\             -ECCMON_D1TCM
*\*\             -ECCMON_AHB_SRAM1
*\*\             -ECCMON_AHB_SRAM2
*\*\             -ECCMON_AHB_SRAM3
*\*\             -ECCMON_AHB_SRAM4
*\*\             -ECCMON_AHB_SRAM5_BANK1
*\*\             -ECCMON_AHB_SRAM5_BANK2
*\*\             -ECCMON_BACKUP_SRAM
*\*\return  Fail ECC code
**/
uint32_t ECCMON_GetFailErrorCode(uint32_t ECCMON_MemType)
{
    ECCMON_MonitorType oneMonitorType;
    //Get ECCMON Monitor unit and index
    oneMonitorType = ECCMON_GetMonitorType(ECCMON_MemType);
    /* Return Fail Address */
    return (uint32_t) * ((&oneMonitorType.MonitorUnit->FECOD1) + (ECCMON_GROUP_REG_ADDR_GAP * oneMonitorType.MonitorIndex));
}

/**
*\*\brief  Get the ECCMON error context Flag
*\*\param  ECCMON_MemType ECCMON Memory Type ,can not |(or) operation
*\*\             -ECCMON_AXI_SRAM1
*\*\             -ECCMON_AXI_SRAM2
*\*\             -ECCMON_AXI_SRAM3
*\*\             -ECCMON_ITCM
*\*\             -ECCMON_D0TCM
*\*\             -ECCMON_D1TCM
*\*\             -ECCMON_AHB_SRAM1
*\*\             -ECCMON_AHB_SRAM2
*\*\             -ECCMON_AHB_SRAM3
*\*\             -ECCMON_AHB_SRAM4
*\*\             -ECCMON_AHB_SRAM5_BANK1
*\*\             -ECCMON_AHB_SRAM5_BANK2
*\*\             -ECCMON_BACKUP_SRAM
*\*\param  ECCMON_ERR_CONTEXT_FLAG
*\*\             -ECCMON_ERR_CONTEXT_FLAG_E1EAD
*\*\             -ECCMON_ERR_CONTEXT_FLAG_E2EAD
*\*\return  Fail ECC code
**/
FlagStatus ECCMON_GetErrorContextFlagStatus(uint32_t ECCMON_MemType, uint32_t ECCMON_ERR_CONTEXT_FLAG)
{
    FlagStatus bitstatus;
    ECCMON_MonitorType oneMonitorType;
    uint32_t registerFlag;
    //Get ECCMON Monitor unit and index
    oneMonitorType = ECCMON_GetMonitorType(ECCMON_MemType);

    registerFlag = (uint32_t) * ((&oneMonitorType.MonitorUnit->FEADR1) + (ECCMON_GROUP_REG_ADDR_GAP * oneMonitorType.MonitorIndex));

    if ((registerFlag & ECCMON_ERR_CONTEXT_FLAG) != (uint32_t)RESET)
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
*\*\brief  Clear  ECCMON error context Flag
*\*\param  ECCMON_MemType ECCMON Memory Type ,can not |(or) operation
*\*\             -ECCMON_AXI_SRAM1
*\*\             -ECCMON_AXI_SRAM2
*\*\             -ECCMON_AXI_SRAM3
*\*\             -ECCMON_ITCM
*\*\             -ECCMON_D0TCM
*\*\             -ECCMON_D1TCM
*\*\             -ECCMON_AHB_SRAM1
*\*\             -ECCMON_AHB_SRAM2
*\*\             -ECCMON_AHB_SRAM3
*\*\             -ECCMON_AHB_SRAM4
*\*\             -ECCMON_AHB_SRAM5_BANK1
*\*\             -ECCMON_AHB_SRAM5_BANK2
*\*\             -ECCMON_BACKUP_SRAM
*\*\param  ECCMON_ERR_CONTEXT_FLAG
*\*\             -ECCMON_ERR_CONTEXT_FLAG_E1EAD
*\*\             -ECCMON_ERR_CONTEXT_FLAG_E2EAD
*\*\return  Fail ECC code
**/
void ECCMON_ClrErrorContextFlag(uint32_t ECCMON_MemType, uint32_t ECCMON_ERR_CONTEXT_FLAG)
{
    ECCMON_MonitorType oneMonitorType;
    __IO uint32_t *oneRegister;
    //Get ECCMON Monitor unit and index
    oneMonitorType = ECCMON_GetMonitorType(ECCMON_MemType);
    //get  ECCMON_INTFx, x=oneMonitorType.MonitorIndex
    oneRegister = (__IO uint32_t *)(&oneMonitorType.MonitorUnit->FEADR1) + (ECCMON_GROUP_REG_ADDR_GAP * oneMonitorType.MonitorIndex);

    *oneRegister |= ECCMON_ERR_CONTEXT_FLAG;

}


/**
*\*\brief  Get the ECCMON Interrupt Flag
*\*\param  ECCMON_MemType ECCMON Memory Type ,can not |(or) operation
*\*\             -ECCMON_AXI_SRAM1
*\*\             -ECCMON_AXI_SRAM2
*\*\             -ECCMON_AXI_SRAM3
*\*\             -ECCMON_ITCM
*\*\             -ECCMON_D0TCM
*\*\             -ECCMON_D1TCM
*\*\             -ECCMON_AHB_SRAM1
*\*\             -ECCMON_AHB_SRAM2
*\*\             -ECCMON_AHB_SRAM3
*\*\             -ECCMON_AHB_SRAM4
*\*\             -ECCMON_AHB_SRAM5_BANK1
*\*\             -ECCMON_AHB_SRAM5_BANK2
*\*\             -ECCMON_BACKUP_SRAM
*\*\param  ECCMON_FLAG
*\*\             -ECCMON_FLAG_E1DCIFW
*\*\             -ECCMON_FLAG_E2DCIFW
*\*\             -ECCMON_FLAG_E1DCIFR
*\*\             -ECCMON_FLAG_E2DCIFR
*\*\return  Fail ECC code
**/
FlagStatus ECCMON_GetFlagStatus(uint32_t ECCMON_MemType, uint32_t ECCMON_FLAG)
{
    FlagStatus bitstatus;
    ECCMON_MonitorType oneMonitorType;
    uint32_t registerFlag;
    //Get ECCMON Monitor unit and index
    oneMonitorType = ECCMON_GetMonitorType(ECCMON_MemType);
    //get  ECCMON_INTFx, x=oneMonitorType.MonitorIndex
    registerFlag = (uint32_t) * ((&oneMonitorType.MonitorUnit->INTF1) + (ECCMON_GROUP_REG_ADDR_GAP * oneMonitorType.MonitorIndex));

    if ((registerFlag & ECCMON_FLAG) != (uint32_t)RESET)
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
*\*\brief  Clear the ECCMON Interrupt Flag
*\*\param  ECCMON_MemType ECCMON Memory Type ,can not |(or) operation
*\*\             -ECCMON_AXI_SRAM1
*\*\             -ECCMON_AXI_SRAM2
*\*\             -ECCMON_AXI_SRAM3
*\*\             -ECCMON_ITCM
*\*\             -ECCMON_D0TCM
*\*\             -ECCMON_D1TCM
*\*\             -ECCMON_AHB_SRAM1
*\*\             -ECCMON_AHB_SRAM2
*\*\             -ECCMON_AHB_SRAM3
*\*\             -ECCMON_AHB_SRAM4
*\*\             -ECCMON_AHB_SRAM5_BANK1
*\*\             -ECCMON_AHB_SRAM5_BANK2
*\*\             -ECCMON_BACKUP_SRAM
*\*\param  ECCMON_FLAG
*\*\             -ECCMON_FLAG_E1DCIFW
*\*\             -ECCMON_FLAG_E2DCIFW
*\*\             -ECCMON_FLAG_E1DCIFR
*\*\             -ECCMON_FLAG_E2DCIFR
*\*\return  Fail ECC code
**/
void ECCMON_ClrFlag(uint32_t ECCMON_MemType, uint32_t ECCMON_FLAG)
{
    ECCMON_MonitorType oneMonitorType;
    __IO uint32_t *oneRegister;
    //Get ECCMON Monitor unit and index
    oneMonitorType = ECCMON_GetMonitorType(ECCMON_MemType);
    //get  ECCMON_INTFx, x=oneMonitorType.MonitorIndex
    oneRegister = (__IO uint32_t *)(&oneMonitorType.MonitorUnit->INTF1) + (ECCMON_GROUP_REG_ADDR_GAP * oneMonitorType.MonitorIndex);

    *oneRegister = ECCMON_FLAG;

}
/**
*\*\brief  Get the ECCMON Interrupt Flag
*\*\param  ECCMON_MemType ECCMON Memory Type ,can not |(or) operation
*\*\             -ECCMON_AXI_SRAM1
*\*\             -ECCMON_AXI_SRAM2
*\*\             -ECCMON_AXI_SRAM3
*\*\             -ECCMON_ITCM
*\*\             -ECCMON_D0TCM
*\*\             -ECCMON_D1TCM
*\*\             -ECCMON_AHB_SRAM1
*\*\             -ECCMON_AHB_SRAM2
*\*\             -ECCMON_AHB_SRAM3
*\*\             -ECCMON_AHB_SRAM4
*\*\             -ECCMON_AHB_SRAM5_BANK1
*\*\             -ECCMON_AHB_SRAM5_BANK2
*\*\             -ECCMON_BACKUP_SRAM
*\*\param  ECCMON_INT
*\*\             -ECCMON_INT_E1DCIFW
*\*\             -ECCMON_INT_E2DCIFW
*\*\             -ECCMON_INT_E1DCIFR
*\*\             -ECCMON_INT_E2DCIFR
*\*\return  Fail ECC code
**/
INTStatus ECCMON_GetIntStatus(uint32_t ECCMON_MemType, uint32_t ECCMON_INT)
{
    FlagStatus bitstatus = RESET;
    ECCMON_MonitorType oneMonitorType;
    uint32_t registerFlag;
    //Get ECCMON Monitor unit and index
    oneMonitorType = ECCMON_GetMonitorType(ECCMON_MemType);
    //get  ECCMON_INTFx, x=oneMonitorType.MonitorIndex
    registerFlag = (uint32_t) * ((&oneMonitorType.MonitorUnit->INTF1) + (ECCMON_GROUP_REG_ADDR_GAP * oneMonitorType.MonitorIndex));

    //judge ECCMON_INT_E1DCIFW|ECCMON_INT_E1DCIFR and  ECC 1-bit Error interrupt enable or disable
    if (((ECCMON_INT & (ECCMON_INT_E1DCIFW | ECCMON_INT_E1DCIFR)) != 0x00UL) &&
            ((oneMonitorType.MonitorUnit->CTRL1 & (ECCMON_CTRL1_E1INTEN1 << oneMonitorType.MonitorIndex)) != 0x00UL))
    {
        if ((registerFlag & ECCMON_INT) != 0x00UL)
        {
            bitstatus = SET;
        }
        else
        {
            bitstatus = RESET;
        }
    }

    //judge ECCMON_INT_E2DCIFW|ECCMON_INT_E2DCIFR and ECC 2-bit Error interrupt enable or disable
    if (((ECCMON_INT & (ECCMON_INT_E2DCIFW | ECCMON_INT_E2DCIFR)) != 0x00UL) &&
            ((oneMonitorType.MonitorUnit->CTRL1 & (ECCMON_CTRL1_E2INTEN1 << oneMonitorType.MonitorIndex)) != 0x00UL))
    {
        if ((registerFlag & ECCMON_INT) != 0x00UL)
        {
            bitstatus = SET;
        }
        else
        {
            bitstatus = RESET;
        }
    }

    return bitstatus;
}
