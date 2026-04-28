/**
 ******************************************************************************
 * @file      x32m7xx_fmac.h
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

#ifndef __X32M7XX_FMAC_H
#define __X32M7XX_FMAC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "x32m7xx.h"

/**  FMAC Init structure definition **/

typedef struct
{
    uint8_t  InputBufBaseAddress;     /* Base address of the input buffer (X1) within the internal memory (0x00 to 0xFF) */
      
    uint8_t  InputBufSize;            /* Number of 16-bit words allocated to the input buffer (including the optional "headroom")*/
      
    uint8_t  CoeffBufBaseAddress;     /* Base address of the coefficient buffer (X2) within the internal memory (0x00 to 0xFF)*/
      
    uint8_t  CoeffBufSize;            /* Number of 16-bit words allocated to the coefficient buffer */
      
    uint8_t  OutBufBaseAddress;       /* Base address of the output buffer (Y) within the internal memory (0x00 to 0xFF). */
      
    uint8_t  OutBufSize;              /* Number of 16-bit words allocated to the output buffer (including the optional "headroom") */
      
    uint8_t  P;                       /* Parameter P (vector length, number of filter taps, etc.) */
      
    uint8_t  Q;                       /* Parameter Q (vector length, etc.). Ignored if not needed */
      
    uint8_t  R;                       /* Parameter R (gain, etc.). Ignored if not needed. */

    uint32_t InputBufThreshold;       /* Input threshold: the buffer full flag will be set if the number of free spaces in the buffer is lower than this threshold*/

    uint32_t OutBufThreshold;         /* Output threshold: the buffer empty flag will be set if the number of unread values */
      
    uint32_t Limit;                   /* Enable or disable the limit feature */

    uint32_t Func;                    /* Filter type */
} FMAC_InitType;

/**  FMAC preload buffer parameters structure definition **/
typedef struct
{
    int16_t *pInputData;        /* content of the input data */
    uint8_t InputDataSize;      /* Size of the input data */

    int16_t *pCoeffA;           /* [IIR only] Initialization of the coefficient vector A. If not needed, it should be set to NULL. */
    uint8_t CoeffASize;         /* Size of the coefficient vector A. */
    int16_t *pCoeffB;           /* Initialization of the coefficient vector B. If not needed (re-use of a previously loaded buffer), it should be set to NULL. */
    uint8_t CoeffBSize;         /* Size of the coefficient vector B. */
    int16_t *pOutputData;       /* content of the output data */
    uint8_t OutputDataSize;     /* size of the output data */
} FMAC_PreLoadType;

/* Function define  */
#define FMAC_FUNC_LOADX1                ((uint32_t)0x01000000U) /* Load X1 buffer */
#define FMAC_FUNC_LOADX2                ((uint32_t)0x02000000U) /* Load X2 buffer */
#define FMAC_FUNC_LOADY                 ((uint32_t)0x03000000U) /* Load Y buffer  */
#define FMAC_FUNC_CONVO_FIR             ((uint32_t)0x08000000U) /* Convolution (FIR filter)   */
#define FMAC_FUNC_IIR_DIRECT_FORM_1     ((uint32_t)0x09000000U) /* IIR filter (direct form 1)  */

/* Buffer threshold define, used for X1 buffer and Y buffer */
#define FMAC_THRESHOLD1                 ((uint32_t)0x00000000U) /* Input: Buffer full flag set if the number of free spaces in the buffer is less than 1.
                                                                   Output: Buffer empty flag set if the number of unread values in the buffer is less than 1. */
#define FMAC_THRESHOLD2                 ((uint32_t)0x01000000U) /* Input: Buffer full flag set if the number of free spaces in the buffer is less than 2.
                                                                   Output: Buffer empty flag set if the number of unread values in the buffer is less than 2. */
#define FMAC_THRESHOLD4                 ((uint32_t)0x02000000U) /* Input: Buffer full flag set if the number of free spaces in the buffer is less than 4.
                                                                   Output: Buffer empty flag set if the number of unread values in the buffer is less than 4. */
#define FMAC_THRESHOLD8                 ((uint32_t)0x03000000U) /* Input: Buffer full flag set if the number of free spaces in the buffer is less than 8.
                                                                   Output: Buffer empty flag set if the number of unread values in the buffer is less than 8. */

/* Data limit define */
#define FMAC_LIMIT_DISABLE              ((uint32_t)0x00000000U) /* Limit disabled */
#define FMAC_LIMIT_ENABLE               FMAC_CTRL_LIMITEN       /* Limit enabled */

/* FMAC flag definitions */  
#define FMAC_FLAG_YBUFEF                FMAC_STS_YBUFEF     /* Y buffer empty flag */
#define FMAC_FLAG_X1BUFFF               FMAC_STS_X1BUFFF    /* X1 buffer full flag */
#define FMAC_FLAG_OVF                   FMAC_STS_OVF        /* Overflow error flag */
#define FMAC_FLAG_UNF                   FMAC_STS_UNF        /* Underflow error flag */
#define FMAC_FLAG_SATF                  FMAC_STS_SATF       /* Saturation error flag */

/* FMAC interrupt definitions */  
#define FMAC_INT_READ                   FMAC_CTRL_RINTEN    /* Enable read interrupt */
#define FMAC_INT_WRITE                  FMAC_CTRL_WIINEN    /* Enable write interrupt */
#define FMAC_INT_OV                     FMAC_CTRL_OVINTEN   /* Enable overflow error interrupts */
#define FMAC_INT_UN                     FMAC_CTRL_UNINTEN   /* Enable underflow error interrupts */
#define FMAC_INT_SAT                    FMAC_CTRL_SATINTEN  /* Enable saturation error interrupts */  

/* FMAC DMA mdoe definitions */          
#define FMAC_DMA_READ                   FMAC_CTRL_DMAREN    /* Enable DMA read channel requests */
#define FMAC_DMA_WRITE                  FMAC_CTRL_DMAWEN    /* Enable DMA write channel requests */

/* FMAC interrupt flag definitions */
#define FMAC_INT_FLAG_YBUFEF            ((FMAC_INT_READ  << 16U) | FMAC_FLAG_YBUFEF)    /* Y buffer empty interrupt flag */
#define FMAC_INT_FLAG_X1BUFFF           ((FMAC_INT_WRITE << 16U) | FMAC_FLAG_X1BUFFF)   /* X1 buffer full interrupt flag */
#define FMAC_INT_FLAG_OVF               ((FMAC_INT_OV    << 16U) | FMAC_FLAG_OVF)       /* Overflow error interrupt flag */
#define FMAC_INT_FLAG_UNF               ((FMAC_INT_UN    << 16U) | FMAC_FLAG_UNF)       /* underflow error interrupt flag */
#define FMAC_INT_FLAG_SATF              ((FMAC_INT_SAT   << 16U) | FMAC_FLAG_SATF)      /* saturation error interrupt flag */
#define FMAC_INT_FLAG_MASK              ((uint32_t)0x0000FFFF)

/* FMAC polling-based communications time-out value */
#define FMAC_TIMEOUT                    (400000000U)
/* FMAC reset time-out value */
#define FMAC_RESET_TIMEOUT_VALUE        (200000000U)

void FMAC_DeInit(void);
void FMAC_StructInit(FMAC_InitType * FMAC_InitStruct);
void FMAC_PreLoadStructInit(FMAC_PreLoadType * FMAC_PreloadStruct);
void FMAC_Init(FMAC_InitType * FMAC_InitStruct);
void FMAC_PreLoadData(uint32_t size, int16_t array[]);
ErrorStatus FMAC_PreloadBufferData(FMAC_PreLoadType * FMAC_PreloadStruct);
void FMAC_ConfigParam(FMAC_InitType* FMAC_ParamStruct);
void FMAC_FilterStart(FMAC_InitType* FMAC_ParamStruct);
ErrorStatus FMAC_Reset(void);
void FMAC_Enable(FunctionalState Cmd);
void FMAC_ConfigX1(uint8_t Baseaddr, uint8_t Bufsize, uint32_t Threshold);
void FMAC_ConfigX2(uint8_t Baseaddr, uint8_t Bufsize);
void FMAC_ConfigY(uint8_t Baseaddr, uint8_t Bufsize, uint32_t Threshold);
void FMAC_WriteData(int16_t data);
int16_t FMAC_ReadData(void);
void FMAC_EnableLimit(FunctionalState Cmd);
void FMAC_EnableDMA(uint32_t dma_req, FunctionalState Cmd);
ErrorStatus FMAC_FinishCalculate(void);
void FMAC_ConfigInt(uint32_t interrupt, FunctionalState Cmd);
FlagStatus FMAC_GetIntStatus(uint32_t FMAC_INT);
FlagStatus FMAC_GetFlagStatus(uint32_t FMAC_FLAG);


#ifdef __cplusplus
}
#endif

#endif /* __X32M7XX_FMAC_H */


