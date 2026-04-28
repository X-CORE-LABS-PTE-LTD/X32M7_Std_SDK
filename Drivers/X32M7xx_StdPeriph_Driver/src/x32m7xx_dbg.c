/**
 ******************************************************************************
 * @file      x32m7xx_dbg.c
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

#include "x32m7xx_dbg.h"


/**
*\*\name    DBG_GetRevNum.
*\*\fun     get the revision number.
*\*\param   none
*\*\return  revision number
**/
uint32_t DBG_GetRevNum(void)
{
    return (uint32_t)((DBG->ID & DBG_REV_NUM_MASK) >> 24);
}

/**
*\*\name    DBG_GetDevNum.
*\*\fun     get the device identifer.
*\*\param   none
*\*\return  Device identifier
*\*\        - 0x760     : X32M760 Series
*\*\        - 0x762     : X32M762 Series
*\*\        - 0x765     : X32M765 Series
*\*\        - 0x785     : X32M785 Series
*\*\        - 0x787     : X32M787 Series
*\*\        - 0x788     : X32M788 Series
**/
uint32_t DBG_GetDevNum(void)
{
    return (uint32_t)((DBG->ID & DBG_DEV_NUM_MASK) >> 12);
}


/**
*\*\name    DBG_SetExternalTriggerPinDirection.
*\*\fun     Set the direction of the bi-directional trigger pin TRGIO.
*\*\param   PinDirection :
              - DBG_TRGIO_INPUT_DIRECTION
              - DBG_TRGIO_OUTPUT_DIRECTION
*\*\return  none
**/
void DBG_SetExternalTriggerPinDirection(uint32_t PinDirection)
{
    if(PinDirection == DBG_TRGIO_OUTPUT_DIRECTION)
    {
        DBG->CTRL |= DBG_TRGIO_OUTPUT_DIRECTION;
    }
    else
    {
        DBG->CTRL &= (~DBG_TRGIO_OUTPUT_DIRECTION) ;
    }
}

/**
*\*\name    DBG_M7ConfigPeriph.
*\*\fun     Configures the specified peripheral run or stop when the M7 MCU under Debug mode.
*\*\param   DBG_Periph :
              - DBG_M7SLEEP               * Keep debugger connection during M7 SLEEP mode *
              - DBG_M7STOP                * Keep debugger connection during M7 STOP mode *
              - DBG_M7STANDBY             * Keep debugger connection during M7 STANDBY mode *
              - DBG_WWDG1_STOP            * WWDG1 stopped when Core is halted. *
              - DBG_WWDG2_STOP            * WWDG2 stopped when Core is halted. *
              - DBG_ATIM3_STOP            * ATIM3 counter stopped when core is halted *
              - DBG_ATIM4_STOP            * ATIM4 counter stopped when core is halted *
              - DBG_I2C7SMBUS_TIMEOUT     * I2C7 counter stopped when core is halted *
              - DBG_I2C8SMBUS_TIMEOUT     * I2C8 counter stopped when core is halted *
              - DBG_I2C9SMBUS_TIMEOUT     * I2C9 counter stopped when core is halted *
              - DBG_I2C10SMBUS_TIMEOUT    * I2C10 counter stopped when core is halted *
              - DBG_IWDG1_STOP            * IWDG1 counter stopped when core is halted *
              - DBG_IWDG2_STOP            * IWDG2 counter stopped when core is halted *
              - DBG_LPTIM1_STOP           * LPTIM1 counter stopped when core is halted *
              - DBG_LPTIM2_STOP           * LPTIM2 counter stopped when core is halted *
              - DBG_LPTIM3_STOP           * LPTIM3 counter stopped when core is halted *
              - DBG_LPTIM4_STOP           * LPTIM4 counter stopped when core is halted *
              - DBG_LPTIM5_STOP           * LPTIM5 counter stopped when core is halted *
              - DBG_RTC_STOP              * RTC counter stopped when core is halted *
              - DBG_CANFD3_STOP           * CANFD3 counter stopped when core is halted *
              - DBG_CANFD4_STOP           * CANFD4 counter stopped when core is halted *
              - DBG_CANFD7_STOP           * CANFD7 counter stopped when core is halted *
              - DBG_CANFD8_STOP           * CANFD8 counter stopped when core is halted *
              - DBG_I2C4SMBUS_TIMEOUT     * I2C4 counter stopped when core is halted *
              - DBG_I2C5SMBUS_TIMEOUT     * I2C5 counter stopped when core is halted *
              - DBG_I2C6SMBUS_TIMEOUT     * I2C6 counter stopped when core is halted *
              - DBG_ATIM1_STOP            * ATIM1 counter stopped when core is halted *
              - DBG_ATIM2_STOP            * ATIM2 counter stopped when core is halted *
              - DBG_GTIMA1_STOP           * GTIMA1 counter stopped when core is halted *
              - DBG_GTIMA2_STOP           * GTIMA2 counter stopped when core is halted *
              - DBG_GTIMA3_STOP           * GTIMA3 counter stopped when core is halted *
              - DBG_SHRTIM1_STOP          * SHRTIM1 counter stopped when core is halted *
              - DBG_SHRTIM2_STOP          * SHRTIM2 counter stopped when core is halted *
              - DBG_BTIM1_STOP            * BTIM1 counter stopped when core is halted *
              - DBG_BTIM2_STOP            * BTIM2 counter stopped when core is halted *
              - DBG_BTIM3_STOP            * BTIM3 counter stopped when core is halted *
              - DBG_BTIM4_STOP            * BTIM4 counter stopped when core is halted *
              - DBG_GTIMA4_STOP           * GTIMA4 counter stopped when core is halted *
              - DBG_GTIMA5_STOP           * GTIMA5 counter stopped when core is halted *
              - DBG_GTIMA6_STOP           * GTIMA6 counter stopped when core is halted *
              - DBG_GTIMA7_STOP           * GTIMA7 counter stopped when core is halted *
              - DBG_GTIMB1_STOP           * GTIMB1 counter stopped when core is halted *
              - DBG_GTIMB2_STOP           * GTIMB2 counter stopped when core is halted *
              - DBG_GTIMB3_STOP           * GTIMB3 counter stopped when core is halted *
              - DBG_I2C1SMBUS_TIMEOUT     * I2C1 counter stopped when core is halted *
              - DBG_I2C2SMBUS_TIMEOUT     * I2C2 counter stopped when core is halted *
              - DBG_I2C3SMBUS_TIMEOUT     * I2C3 counter stopped when core is halted *
              - DBG_CANFD1_STOP           * CANFD1 counter stopped when core is halted *
              - DBG_CANFD2_STOP           * CANFD2 counter stopped when core is halted *
              - DBG_CANFD5_STOP           * CANFD5 counter stopped when core is halted *
              - DBG_CANFD6_STOP           * CANFD6 counter stopped when core is halted *
*\*\return  none
**/
void DBG_M7ConfigPeriph(uint32_t DBG_Periph, FunctionalState Cmd)
{
    uint32_t Offset = ( (DBG_Periph & REGISTER_OFFSET_MASK) >> REGISTER_OFFSET_POS);
    __IO uint32_t  Register_Base = DBGMCU_BASE;

    Register_Base += Offset;

    if (Cmd != DISABLE)
    {
        *(uint32_t *)Register_Base |= DBG_Periph;
    }
    else
    {
        *(uint32_t *)Register_Base &= ~DBG_Periph;
    }
}

/**
*\*\name    DBG_M4ConfigPeriph.
*\*\fun     Configures the specified peripheral run or stop when the M4 MCU under Debug mode.
*\*\param   DBG_Periph :
              - DBG_M4SLEEP               * Keep debugger connection during M4 SLEEP mode *
              - DBG_M4STOP                * Keep debugger connection during M4 STOP mode *
              - DBG_M4STANDBY             * Keep debugger connection during M4 STANDBY mode *
              - DBG_WWDG1_STOP            * WWDG1 stopped when Core is halted. *
              - DBG_WWDG2_STOP            * WWDG2 stopped when Core is halted. *
              - DBG_ATIM3_STOP            * ATIM3 counter stopped when core is halted *
              - DBG_ATIM4_STOP            * ATIM4 counter stopped when core is halted *
              - DBG_I2C7SMBUS_TIMEOUT     * I2C7 counter stopped when core is halted *
              - DBG_I2C8SMBUS_TIMEOUT     * I2C8 counter stopped when core is halted *
              - DBG_I2C9SMBUS_TIMEOUT     * I2C9 counter stopped when core is halted *
              - DBG_I2C10SMBUS_TIMEOUT    * I2C10 counter stopped when core is halted *
              - DBG_IWDG1_STOP            * IWDG1 counter stopped when core is halted *
              - DBG_IWDG2_STOP            * IWDG2 counter stopped when core is halted *
              - DBG_LPTIM1_STOP           * LPTIM1 counter stopped when core is halted *
              - DBG_LPTIM2_STOP           * LPTIM2 counter stopped when core is halted *
              - DBG_LPTIM3_STOP           * LPTIM3 counter stopped when core is halted *
              - DBG_LPTIM4_STOP           * LPTIM4 counter stopped when core is halted *
              - DBG_LPTIM5_STOP           * LPTIM5 counter stopped when core is halted *
              - DBG_RTC_STOP              * RTC counter stopped when core is halted *
              - DBG_CANFD3_STOP           * CANFD3 counter stopped when core is halted *
              - DBG_CANFD4_STOP           * CANFD4 counter stopped when core is halted *
              - DBG_CANFD7_STOP           * CANFD7 counter stopped when core is halted *
              - DBG_CANFD8_STOP           * CANFD8 counter stopped when core is halted *
              - DBG_I2C4SMBUS_TIMEOUT     * I2C4 counter stopped when core is halted *
              - DBG_I2C5SMBUS_TIMEOUT     * I2C5 counter stopped when core is halted *
              - DBG_I2C6SMBUS_TIMEOUT     * I2C6 counter stopped when core is halted *
              - DBG_ATIM1_STOP            * ATIM1 counter stopped when core is halted *
              - DBG_ATIM2_STOP            * ATIM2 counter stopped when core is halted *
              - DBG_GTIMA1_STOP           * GTIMA1 counter stopped when core is halted *
              - DBG_GTIMA2_STOP           * GTIMA2 counter stopped when core is halted *
              - DBG_GTIMA3_STOP           * GTIMA3 counter stopped when core is halted *
              - DBG_SHRTIM1_STOP          * SHRTIM1 counter stopped when core is halted *
              - DBG_SHRTIM2_STOP          * SHRTIM2 counter stopped when core is halted *
              - DBG_BTIM1_STOP            * BTIM1 counter stopped when core is halted *
              - DBG_BTIM2_STOP            * BTIM2 counter stopped when core is halted *
              - DBG_BTIM3_STOP            * BTIM3 counter stopped when core is halted *
              - DBG_BTIM4_STOP            * BTIM4 counter stopped when core is halted *
              - DBG_GTIMA4_STOP           * GTIMA4 counter stopped when core is halted *
              - DBG_GTIMA5_STOP           * GTIMA5 counter stopped when core is halted *
              - DBG_GTIMA6_STOP           * GTIMA6 counter stopped when core is halted *
              - DBG_GTIMA7_STOP           * GTIMA7 counter stopped when core is halted *
              - DBG_GTIMB1_STOP           * GTIMB1 counter stopped when core is halted *
              - DBG_GTIMB2_STOP           * GTIMB2 counter stopped when core is halted *
              - DBG_GTIMB3_STOP           * GTIMB3 counter stopped when core is halted *
              - DBG_I2C1SMBUS_TIMEOUT     * I2C1 counter stopped when core is halted *
              - DBG_I2C2SMBUS_TIMEOUT     * I2C2 counter stopped when core is halted *
              - DBG_I2C3SMBUS_TIMEOUT     * I2C3 counter stopped when core is halted *
              - DBG_CANFD1_STOP           * CANFD1 counter stopped when core is halted *
              - DBG_CANFD2_STOP           * CANFD2 counter stopped when core is halted *
              - DBG_CANFD5_STOP           * CANFD5 counter stopped when core is halted *
              - DBG_CANFD6_STOP           * CANFD6 counter stopped when core is halted *
*\*\return  none
**/
void DBG_M4ConfigPeriph(uint32_t DBG_Periph, FunctionalState Cmd)
{
    uint32_t Offset = ( (DBG_Periph & REGISTER_OFFSET_MASK) >> REGISTER_OFFSET_POS);
    __IO uint32_t  Register_Base = DBGMCU_BASE;

    Register_Base += (Offset + 0x04u);

    if (Cmd != DISABLE)
    {
        *(uint32_t *)Register_Base |= DBG_Periph;
    }
    else
    {
        *(uint32_t *)Register_Base &= ~DBG_Periph;
    }
}
/**
*\*\name    DBG_GetFlashSize.
*\*\fun     Get FLASH size of this chip.
*\*\param   none
*\*\return  FLASH size in bytes.
**/
uint32_t DBG_GetFlashSize(void)
{
    uint32_t Flash_size = (DBG->ID & DBG_FLASH_SIZE_MASK);

    if(Flash_size != DBG_FLASH_SIZE_MASK)
    {
        Flash_size = (uint32_t)(Flash_size << 15);
    }
    else
    {
        Flash_size = 0;
    }

    return Flash_size;
}

