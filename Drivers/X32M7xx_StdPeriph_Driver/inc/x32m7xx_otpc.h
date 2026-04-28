/**
 ******************************************************************************
 * @file      x32m7xx_otpc.h
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

#ifndef __X32M7XX_OTPC_H__
#define __X32M7XX_OTPC_H__

#include "x32m7xx.h"

#ifdef __cplusplus
extern "C" {
#endif
    
/** OTPC status**/
typedef enum
{
    OTPC_UNCOMPLETE = 1,
    OTPC_ERR_TIMEOUT,
    OTPC_COMPLETE,
}OTPC_STS;

/** OTPC reload data structure definition **/
typedef struct
{
    uint32_t NRST_IWDG_OTPValue;    /* Corresponding to the value of SYS_CFG_CRST_SWDG_OTP in OTP */
    uint32_t BOR_OTPValue;          /* Corresponding to the value of BOR in OTP */
    uint32_t SEC_JTAG_OTPValue;     /* Corresponding to the value of SEC_JTAG in OTP */
    FlagStatus L2MODE;              /* Chip L2 mode*/
    FlagStatus L1MODE;              /* Chip L2 mode*/
    FlagStatus L0MODE;              /* Chip L2 mode*/
    FlagStatus ROOTMODE;            /* Chip ROOT mode*/
    FlagStatus INITMODE;            /* Chip INIT mode*/
    uint32_t TCM_SIZE_OTPValue;     /* Corresponding to the value of TCM_SIZE in OTP */
} OTPC_DBGReloadValue;    
    

/** OTPC Keys **/
#define OTPC_KEY1               ((uint32_t)0x45670123U)
#define OTPC_KEY2               ((uint32_t)0xCDEF89ABU)    

/** OTPC Operation **/
#define OTPC_CMD_WRITE          (OTPC_CTRL_PRMD)
#define OTPC_CMD_READ           (~OTPC_CTRL_PRMD)
#define OTPC_CMD_LOCK           (OTPC_CTRL_LOCK)   

/** OTPC us Counter Mask**/   
#define OTPC_USC_MASK           (OTPC_USC_VAL)    
    
/** OTPC Flags **/    
#define OTPC_FLAG_BE                                      ((uint32_t)OTPC_STS_BE  )         /* Bit[6] */
#define OTPC_FLAG_OORE                                    ((uint32_t)OTPC_STS_OORE)         /* Bit[5] */
#define OTPC_FLAG_RDPE                                    ((uint32_t)OTPC_STS_RDPE)         /* Bit[4] */
#define OTPC_FLAG_WRPE                                    ((uint32_t)OTPC_STS_WRPE)         /* Bit[3] */
#define OTPC_FLAG_PGE                                     ((uint32_t)OTPC_STS_PGE )         /* Bit[2] */
#define OTPC_FLAG_KEYE                                    ((uint32_t)OTPC_STS_KEYE)         /* Bit[1] */
#define OTPC_FLAG_BUSY                                    ((uint32_t)OTPC_STS_BUSY)         /* Bit[0] */
#define OTPC_ERROR_STS     (OTPC_FLAG_BE | OTPC_FLAG_OORE | OTPC_FLAG_RDPE | OTPC_FLAG_WRPE | OTPC_FLAG_PGE)
#define OTPC_ALLERROR_STS  (OTPC_FLAG_BE | OTPC_FLAG_OORE | OTPC_FLAG_RDPE | OTPC_FLAG_WRPE | OTPC_FLAG_PGE | OTPC_FLAG_KEYE)

/** OTPC Interrupt **/    
#define OTPC_INT_BE                                      ((uint32_t)OTPC_CTRL_BEEN)         /* Bit[6] */
#define OTPC_INT_OORE                                    ((uint32_t)OTPC_CTRL_OOREEN)         /* Bit[5] */
#define OTPC_INT_RDPE                                    ((uint32_t)OTPC_CTRL_RDPEEN)         /* Bit[4] */
#define OTPC_INT_WRPE                                    ((uint32_t)OTPC_CTRL_WRPEEN)         /* Bit[3] */
#define OTPC_INT_PGE                                     ((uint32_t)OTPC_CTRL_PGEEN)         /* Bit[2] */

/** User config register Offset Address **/
#define OTPC_VARIOUS_REG_MASK           ((uint32_t)0x0000FFFFU)
#define OTPC_SEC_JTAG_REG               ((uint32_t)0x00000020U)
#define OTPC_SEC_MODE_REG               ((uint32_t)0x00000028U)
#define OTPC_RDP2_REG                   ((uint32_t)0x00000030U)
#define OTPC_BTM_REG                    ((uint32_t)0x00000038U)
#define OTPC_BOR_REG                    ((uint32_t)0x0000003CU)
#define OTPC_IWDG_REG                   ((uint32_t)0x00000040U)
#define OTPC_TCM_SIZE_REG               ((uint32_t)0x00000044U)
#define OTPC_JTAG_KEY_REG               ((uint32_t)0x00000050U)
#define OTPC_REK_UNIT1_REG              ((uint32_t)0x00000054U)
#define OTPC_REK_UNIT2_REG              ((uint32_t)0x00000058U)
#define OTPC_REK_UNIT3_REG              ((uint32_t)0x0000005CU)
#define OTPC_REK_UNIT4_REG              ((uint32_t)0x00000060U)
#define OTPC_IDK_UNIT1_REG              ((uint32_t)0x00000064U)
#define OTPC_IDK_UNIT2_REG              ((uint32_t)0x00000068U)
#define OTPC_IDK_UNIT3_REG              ((uint32_t)0x0000006CU)
#define OTPC_IDK_UNIT4_REG              ((uint32_t)0x00000070U)
#define OTPC_UMUU_BASE_REG              ((uint32_t)0x00000074U)

/** OTPC CRLD1 register bit offset define **/
#define OTPC_CRLD1_NRIWDG_OFFSET               (REG_BIT16_OFFSET)  /* NRST IWDG OTP Value */
#define OTPC_CRLD1_BOR_OFFSET                  (REG_BIT12_OFFSET)  /* BOR OTP Value */

#define OTPC_TIME_OUT                  ((uint32_t)0xffffffffU)  /* timeout */

/* OTPC User unuse memory */
#define OTPC_UNUSE_STARTADDRESS        ((uint32_t)0x00000500U)  /* OTPC User unuse memory start address */
#define OTPC_UNUSE_PAGESIZE            ((uint32_t)0x00000020U)  /* OTPC User unuse memory page size */

/** OTPC Function **/
void OTPC_Unlock(void);
void OTPC_Lock(void);
FlagStatus OTPC_GetLockStatus(void);
void OTPC_SetUsCount(uint32_t us_count);
FlagStatus OTPC_GetFlagStatus(uint32_t optc_flag);
void OTPC_ClearFlag(uint32_t optc_flag);
FlagStatus OTPC_CheckError(void);
OTPC_STS OTPC_WaitForLastOperation(void);
void OTPC_ConfigInterrupt(uint32_t otpc_int, FunctionalState cmd);
void OTPC_WriteEnable(void);
void OTPC_ReadEnable(void);
void OTPC_SetAddr(uint32_t addr);
void OTPC_SetWriteData(uint32_t data);
OTPC_STS OTPC_ProgramWord(uint32_t addr, uint32_t data);
OTPC_STS OTPC_ReadWord(uint32_t addr, uint32_t *data);
uint32_t OTPC_GetUserCfgVaildNum(uint32_t ConfigUser);
FlagStatus OTPC_CheckUserMemoryUnused(uint32_t addr);
void OTPC_GetReload(OTPC_DBGReloadValue* DBGReloadValue);


#ifdef __cplusplus
}
#endif

#endif /* __X32M7XX_OTPC_H__ */
