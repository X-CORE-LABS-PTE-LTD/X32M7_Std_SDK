/**
 ******************************************************************************
 * @file      x32m7xx_smu.c
 * @version   v1.1.0
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

#include "x32m7xx_smu.h"

/* SMU Driving Functions Declaration */

/* SMU_Private_Defines  */

/**
*\*\name    SMU_GetM4BootAddr.
*\*\fun     Read boot address of CM4
*\*\param   none
*\*\return  CM4 boot address
**/
uint32_t SMU_GetM4BootAddr( void )
{
    return (*(uint32_t (*)(void))GET_M4ADDR)();
}

/**
*\*\name    SMU_SetM4BootAddr.
*\*\fun     Setting boot address of CM4
*\*\param   CM4 boot address
*\*\return  ErrorStatus:
 *\*\         - SUCCESS   Set M4 boot addr success
 *\*\         - ERROR     Set M4 boot addr failed
**/
ErrorStatus SMU_SetM4BootAddr( uint32_t addr )
{
    uint32_t ErrCode = (*(uint32_t (*)(uint32_t))(SET_M4ADDR))( addr );
    if((ErrCode == SMU_UNCHANGE_ERR) || (ErrCode == SMU_SUCCESS))
    {
        return SUCCESS;
    }
    else
    {
        /* no process */
    }
    return ERROR;
}

/**
*\*\name    SMU_GetM7BootAddr.
*\*\fun     Read boot address of CM7
*\*\param   none
*\*\return  CM7 boot address
**/
uint32_t SMU_GetM7BootAddr( void )
{
    return (*(uint32_t (*)(void))GET_M7ADDR)();
}

/**
*\*\name    SMU_SetM7BootAddr.
*\*\fun     Setting boot address of CM7
*\*\param   CM7 boot address
*\*\return  ErrorStatus:
 *\*\         - SUCCESS   Set M7 boot addr success
 *\*\         - ERROR     Set M7 boot addr failed
**/
ErrorStatus SMU_SetM7BootAddr( uint32_t addr )
{
    uint32_t ErrCode = (*(uint32_t (*)(uint32_t))(SET_M7ADDR))( addr );
    if((ErrCode == SMU_UNCHANGE_ERR) || (ErrCode == SMU_SUCCESS))
    {
        return SUCCESS;
    }
    else
    {
        /* no process */
    }
    return ERROR;
}

/**
*\*\name    SMU_GetRDPLevel.
*\*\fun     Get RDP level status
*\*\param   none
*\*\return  RDPLEVEL: RDP Level status
*\*\          - RDP_L0
*\*\          - RDP_L1
*\*\          - RDP_L2
**/
RDPLEVEL SMU_GetRDPLevel( void )
{
    return (RDPLEVEL)(*(uint32_t (*)(void))GET_RDPLevel)();
}

/**
*\*\name    SMU_SetRDPLevel.
*\*\fun     Config RDP level.
*\*\param   RDPLevel:
*\*\          - RDP_L0   : L0
*\*\          - RDP_L1   : L1
*\*\          - RDP_L2   : L2
*\*\return  ErrorStatus:
*\*\          - SUCCESS   Set RDP level success
*\*\          - ERROR     Set RDP level failed
*\*\Note:   when it configs success for the first time, this function will actively triggers a system reset. 
**/
ErrorStatus SMU_SetRDPLevel( RDPLEVEL RDPLevel )
{
    uint32_t ErrCode = (*(uint32_t (*)(uint32_t))(SET_RDPLevel))( (uint32_t)RDPLevel );
    if((ErrCode == SMU_UNCHANGE_ERR) || (ErrCode == SMU_SUCCESS))
    {
        return SUCCESS;
    }
    else
    {
        /* no process */
    }
    return ERROR;
}

/**
*\*\name    SMU_SetITCMProtection.
*\*\fun     Set ITCM protection area.
*\*\param   StrAddr: range from 0 ~ 0x00100000
*\*\param   EndAddr: range from 0 ~ 0x00100000 
*\*\return  ErrorStatus:
 *\*\         - SUCCESS   Set TCM Protection success
 *\*\         - ERROR     Set TCM Protection failed
*\*\Note1:   EndAddr must bigger than StrAddr; 
*\*\Note2:   when it configs success for the first time, this function will actively triggers a system reset. 
**/
ErrorStatus SMU_SetITCMProtection(uint32_t StrAddr, uint32_t EndAddr)
{
    uint32_t ErrCode = (*(uint32_t (*)(uint32_t, uint32_t))(SET_ITCMProtection))( StrAddr, EndAddr);
    
    if((ErrCode == SMU_UNCHANGE_ERR) || (ErrCode == SMU_SUCCESS))
    {
        return SUCCESS;
    }
    else
    {
        /* no process */
    }
    return ERROR;
}

/**
*\*\name    SMU_SetSRAMProtection.
*\*\fun     Set AHB-SRAM protection area.
*\*\param   StrAddr: range from 0x30000000 ~ 0x30058000
*\*\param   EndAddr: range from 0x30000000 ~ 0x30058000
*\*\return  ErrorStatus:
 *\*\         - SUCCESS   Set AHB-SRAM Protection success
 *\*\         - ERROR     Set AHB-SRAM Protection failed
*\*\Note1:   EndAddr must bigger than StrAddr; 
*\*\Note2:   when it configs success for the first time, this function will actively triggers a system reset.
**/
ErrorStatus SMU_SetSRAMProtection(uint32_t StrAddr, uint32_t EndAddr)
{
    uint32_t ErrCode = (*(uint32_t (*)(uint32_t, uint32_t))(SET_AHBSRAMProtection))( StrAddr, EndAddr);
    
    if((ErrCode == SMU_PARA_ERR) || (ErrCode == SMU_SUCCESS))
    {
        return SUCCESS;
    }
    else
    {
        /* no process */
    }
    return ERROR;
}


/**
*\*\name    SMU_EnWriteProtection.
*\*\fun     Config Flash Write Protection Area.
*\*\param   WRPSector: set Bitx = 0 means enable AREAx Write Protection. Every bit stand for 128k size of Flash
 *\*\         -BIT0 : Reserved
 *\*\         -BIT1 : AREA1  0x15000000 ~ 0x1501FFFF : Size = 128K [FLASH SIZE  =2M]
 *\*\         -BIT2 : AREA2  0x15020000 ~ 0x1503FFFF : Size = 128K [FLASH SIZE  =2M]
 *\*\         - ......
 *\*\         -BIT15 :AREA15 0x151C0000 ~ 0x151DFFFF : Size = 128K [FLASH SIZE  =2M]
 *\*\         -BIT16 :AREA16 0x151E0000 ~ 0x151FFFFF : Size = 128K [FLASH SIZE  =4M]
 *\*\         -BIT17 :AREA17 0x15200000 ~ 0x1521FFFF : Size = 128K [FLASH SIZE  =4M]
 *\*\         - ......
 *\*\         -BIT31 :AREA31 0x153C0000 ~ 0x153DFFFF : Size = 128K [FLASH SIZE  =4M]
*\*\return  ErrorStatus:
 *\*\         - SUCCESS   Config Flash Write Protection Area success
 *\*\         - ERROR     Config Flash Write Protection Area failed
*\*\Note:   WRPSector = 0xFFFE; when it configs success for the first time, this function will actively triggers a system reset.  
**/
ErrorStatus SMU_EnWriteProtection(uint32_t WRPSector)
{
    uint32_t ErrCode = (*(uint32_t (*)(uint32_t))(SET_FLASHWRProtection))( WRPSector);
    
    if((ErrCode == SMU_UNCHANGE_ERR) || (ErrCode == SMU_SUCCESS))
    {
        return SUCCESS;
    }
    else
    {
        /* no process */
    }
    return ERROR;
}

/**
*\*\name    SMU_GetWriteProtection.
*\*\fun     Get Flash Write Protection Area.
*\*\param   none
*\*\return  WRPSector: 
 *\*\         -BIT0 : Reserved
 *\*\         -BIT1 : 0x15000000 ~ 0x1501FFFF : Size = 128K [FLASH SIZE  =2M]
 *\*\         -BIT2 : 0x15020000 ~ 0x1503FFFF : Size = 128K [FLASH SIZE  =2M]
 *\*\         - ......
 *\*\         -BIT15 :0x151C0000 ~ 0x151DFFFF : Size = 128K [FLASH SIZE  =2M]
 *\*\         -BIT16 :0x151E0000 ~ 0x151FFFFF : Size = 128K [FLASH SIZE  =4M]
 *\*\         -BIT17 :0x15200000 ~ 0x1521FFFF : Size = 128K [FLASH SIZE  =4M]
 *\*\         - ......
 *\*\         -BIT31 :0x153C0000 ~ 0x153DFFFF : Size = 128K [FLASH SIZE  =4M]
**/
uint32_t SMU_GetWriteProtection( void )
{
    return (uint32_t)(*(uint32_t (*)(void))GET_FLASHWRProtection)();
}
/**
*\*\name    SMU_EraseFlash.
*\*\fun     Flash erase
*\*\param   Flash start address, erase the 4KB sector where the start address is located
*\*\return  Error flag
 *\*\         - FLASH_SUCCESS  : Flash operate success
 *\*\         - Other Value    : Flash operate failure
*\*\note    Erase in 4KB units
**/
uint32_t SMU_EraseFlash(uint32_t StrAddr)
{
    return (*(uint32_t (*)(uint32_t))(ER_FLASH))( StrAddr );
}

/**
*\*\name    SMU_WriteFlash.
*\*\fun     Write data to internal xSPI flash
*\*\param   Flash start address
*\*\param   The pointer of data buffer
*\*\param   Data lenght
*\*\return  Error flag
 *\*\         - FLASH_SUCCESS  : Flash operate success
 *\*\         - Other Value    : Flash operate failure
**/
uint32_t SMU_WriteFlash(uint32_t StrAddr, uint8_t *SrcBuf, uint32_t Len)
{
    return (*(uint32_t (*)(uint32_t, uint8_t*, uint32_t))(WR_FLASH))( StrAddr, SrcBuf, Len );
}
