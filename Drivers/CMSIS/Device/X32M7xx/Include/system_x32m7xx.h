/**
 ******************************************************************************
 * @file      system_x32m7xx.h
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

#ifndef __SYSTEM_X32M7XX_H__
#define __SYSTEM_X32M7XX_H__

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

/** X32M7XX_System */

 /** Power supply source configuration **/
#define PWR_SUPPLY_MODE_MASK               (PWR_SYSCTRL4_MLDOEN | PWR_SYSCTRL4_DCDCEN | PWR_SYSCTRL4_VCORESRC | PWR_SYSCTRL4_DCDCFRCEN)    
#define PWR_LDO_SUPPLY                     (PWR_SYSCTRL4_MLDOEN)             /* Core domains are supplied from the LDO  */
#define PWR_DIRECT_SMPS_SUPPLY             (PWR_SYSCTRL4_DCDCEN)             /* Core domains are supplied from the SMPS */ 
#define PWR_EXTERNAL_SOURCE_SUPPLY         (PWR_SYSCTRL4_VCORESRC)           /* The SMPS and the LDO are Bypassed. The Core domains are supplied from an external source */

 /** NRST Analog and Digital Filter configuration **/
#define PWR_RST_AGFBPEN_MAST              (PWR_SYSCTRL1_AGF_ARSTOBP)
#define PWR_RST_DGFBPEN_MAST              (PWR_SYSCTRL1_NRST_DGFBP)
#define PWR_RST_DGF_CNT_MAST              (PWR_SYSCTRL1_NRST_DGFCNT)
#define PWR_RST_DGF_CNT_DEFAULT           ((uint32_t)0x200000U)

extern uint32_t SystemCoreClock; /* System Clock Frequency (Core Clock) */

extern void SystemInit(void);
#ifdef __cplusplus
}
#endif

#endif /*__SYSTEM_X32M7XX_H__ */
