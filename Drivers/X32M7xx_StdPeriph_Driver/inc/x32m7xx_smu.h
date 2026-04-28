/**
 ******************************************************************************
 * @file      x32m7xx_smu.h
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

#ifndef __X32M7XX_SMU_H__
#define __X32M7XX_SMU_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "x32m7xx.h"

/** x32m7xx_StdPeriph_Driver **/

/** SMU_Exported_Constants **/

/** Flash Operation Error Code Macro Definition **/
typedef enum{
    FLASH_SUCCESS           = 0U,
    FLASH_BUS_ADDR_ERROR    = 1U,
    FLASH_LOGIC_ADDR_ERROR  = 2U,
    FLASH_RESTRICTED        = 3U,
    FLASH_RDP_PROTECTED     = 4U,
    FLASH_SECURE_AREA       = 5U,
    FLASH_PFOER_AREA        = 6U,
    FLASH_WRP_PROTECTED     = 7U,
    FLASH_FAILED            = 8U
}FLASH_ErrCode;


typedef enum{
    RDP_L0 = 0U,
    RDP_L1 = 1U,
    RDP_L2 = 2U,
}RDPLEVEL;



/*API Status Code*/
#define  SMU_SUCCESS                       ((uint32_t)0x1000U)
#define  SMU_INVAILED                      ((uint32_t)0x1002U)
#define  SMU_RDP_L2_ERR                    ((uint32_t)0x1003U)
#define  SMU_UNCHANGE_ERR                  ((uint32_t)0x1004U)
#define  SMU_PARA_ERR                      ((uint32_t)0x1005U)
#define  SMU_OTHER_ERR                     ((uint32_t)0x101BU)





/*Boot API Address Pointer adresss*/
#define  GET_M4ADDR                         (0x1ff00601)
#define  SET_M4ADDR                         (0x1ff00631)
#define  GET_M7ADDR                         (0x1ff005c1)
#define  SET_M7ADDR                         (0x1ff00501)
#define  GET_RDPLevel                       (0x1ff00011)
#define  SET_RDPLevel                       (0x1ff00081)
#define  SET_AHBSRAMProtection              (0x1ff01701)
#define  SET_ITCMProtection                 (0x1ff01601)
#define  GET_FLASHWRProtection              (0x1ff001a1)
#define  SET_FLASHWRProtection              (0x1ff00201)
#define  WR_FLASH                           (0x1fff7b81)
#define  ER_FLASH                           (0x1fff7c81)






uint32_t SMU_GetM4BootAddr( void );
ErrorStatus SMU_SetM4BootAddr( uint32_t addr );
uint32_t SMU_GetM7BootAddr( void );
ErrorStatus SMU_SetM7BootAddr( uint32_t addr );

/*Set RDP Function*/
RDPLEVEL SMU_GetRDPLevel( void );
ErrorStatus SMU_SetRDPLevel( RDPLEVEL RDPLevel );

/*Set Write Protectio Area*/
ErrorStatus SMU_EnWriteProtection(uint32_t WRPSector);
uint32_t SMU_GetWriteProtection( void );

/*Set Protectio Area*/
ErrorStatus SMU_SetITCMProtection(uint32_t StrAddr, uint32_t EndAddr);
ErrorStatus SMU_SetSRAMProtection(uint32_t StrAddr, uint32_t EndAddr);

/*Flash operation*/
uint32_t SMU_EraseFlash(uint32_t StrAddr);
uint32_t SMU_WriteFlash(uint32_t StrAddr, uint8_t *SrcBuf, uint32_t Len);   
    
#ifdef __cplusplus
}
#endif

#endif /*__X32M7XX_SMU_H__ */
