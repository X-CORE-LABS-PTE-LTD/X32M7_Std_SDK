/**
 ******************************************************************************
 * @file      x32m7xx_vrefbuf.h
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

#ifndef __X32M7XX_VREFBUF_H__
#define __X32M7XX_VREFBUF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "x32m7xx.h"

/** VREFBUF Register list**/
#define VREFBUF_STS_REG_ADDR                     ((uint32_t)AFEC_BASE + 0x34U)
#define VREFBUF_CTRL1_REG_ADDR                   ((uint32_t)AFEC_BASE + 0x48U)
#define VREFBUF_CTRL2_REG_ADDR                   ((uint32_t)AFEC_BASE + 0xDCU)
#define VREFBUF_TRIM1_REG_ADDR                   ((uint32_t)AFEC_BASE + 0x28U)
#define VREFBUF_TRIM2_REG_ADDR                   ((uint32_t)AFEC_BASE + 0xE8U)

#define VREFBUF_EN_CTRL                          ((uint32_t)AFEC_BASE + 0x3CU)  

/** VREFBUF_Exported_Constants **/
#define VREFBUF_EN_MASK                           (VREFBUF_CTRL1_EN)
#define VREFBUF_HIM_EN_MASK                       (VREFBUF_CTRL1_HIM)

#define VREFBUF_VOLTAGE_SCALE_MASK                (VREFBUF_CTRL2_VLSEL)
#define VREFBUF_VOLTAGE_SCALE_2_5V                ((uint32_t)0x00000000U)
#define VREFBUF_VOLTAGE_SCALE_2_048V              (VREFBUF_CTRL2_VLSEL_0)
#define VREFBUF_VOLTAGE_SCALE_1_8V                (VREFBUF_CTRL2_VLSEL_1)
#define VREFBUF_VOLTAGE_SCALE_1_5V                (VREFBUF_CTRL2_VLSEL_1 | VREFBUF_CTRL2_VLSEL_0)

#define VREFBUF_READY_MASK                        (VREFBUF_STS_RDY)

/** VREFBUF_Trimming_Constants **/
#define VREFBUF_TRIMING_2_5V_POS                  ((uint8_t)16U)
#define VREFBUF_TRIMING_2_5V_MASK                 (VREFBUF_TRIM1_2_5V_MASK)
#define VREFBUF_TRIMING_2_0V_POS                  ((uint8_t)22U)
#define VREFBUF_TRIMING_2_0V_MASK                 (VREFBUF_TRIM1_2_048V_MASK)
#define VREFBUF_TRIMING_1_8V_POS                  ((uint8_t)0U)
#define VREFBUF_TRIMING_1_8V_MASK                 (VREFBUF_TRIM1_1_8V_MASK)
#define VREFBUF_TRIMING_1_5V_POS                  ((uint8_t)8U)
#define VREFBUF_TRIMING_1_5V_MASK                 (VREFBUF_TRIM1_1_5V_MASK)
/** VREFBUF_Exported_Functions **/

void VREFBUF_Enable(FunctionalState Cmd);
void VREFBUF_EnableHIM(FunctionalState Cmd);
void VREFBUF_SetVoltageScale(uint32_t Scale);
uint32_t VREFBUF_GetVoltageScale(void);
FlagStatus VREFBUF_IsVREFReady(void);


void VREFBUF_SetTrimming(uint32_t Value);

#ifdef __cplusplus
}
#endif

#endif /*__X32M7XX_VREFBUF_H__ */


