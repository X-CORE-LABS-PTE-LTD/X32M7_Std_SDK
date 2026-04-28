/**
 ******************************************************************************
 * @file      usbd_otp_if.c
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

/* Includes ------------------------------------------------------------------*/
#include "usbd_otp_if.h"
#include "usbd_dfu_mal.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
uint16_t OTP_If_Write (uint32_t Add, uint32_t Len);
uint8_t *OTP_If_Read  (uint32_t Add, uint32_t Len);
uint16_t OTP_If_CheckAdd(uint32_t Add);


/* Private variables ---------------------------------------------------------*/
DFU_MAL_Prop_TypeDef DFU_Otp_cb =
  {
    OTP_IF_STRING,
    NULL, /* Init not supported*/
    NULL, /* DeInit not supported */
    NULL, /* Erase not supported */
    OTP_If_Write,
    OTP_If_Read,
    OTP_If_CheckAdd,
    1,  /* Erase Time in ms */
    10  /* Programming Time in ms */
  };
  
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  OTP_If_Write
  *         Memory write routine.
  * @param  Add: Address to be written to.
  * @param  Len: Number of data to be written (in bytes).
  * @retval MAL_OK if operation is successful, MAL_FAIL else.
  */
uint16_t OTP_If_Write(uint32_t Add, uint32_t Len)
{
    uint32_t idx = 0;

    if  (Len & 0x3) /* Not an aligned data */
    {
        for (idx = Len; idx < ((Len & 0xFFFC) + 4); idx++)
        {
            MAL_Buffer[idx] = 0xFF;
        }
    }

    /* Data received are Word multiple */
    for (idx = 0; idx <  Len; idx = idx + 4)
    {
//        FLASH_ProgramWord(Add, *(uint32_t *)(MAL_Buffer + idx));
        Add += 4;
    }
    return MAL_OK;
}

/**
  * @brief  OTP_If_Read
  *         Memory read routine.
  * @param  Add: Address to be read from.
  * @param  Len: Number of data to be read (in bytes).
  * @retval Pointer to the physical address where data should be read.
  */
uint8_t *OTP_If_Read (uint32_t Add, uint32_t Len)
{
#ifdef USB_INTERNAL_DMA_ENABLED
    uint32_t idx = 0;
    for (idx = 0; idx < Len; idx += 4)
    {
        *(uint32_t*)(MAL_Buffer + idx) = *(uint32_t *)(Add + idx);
    }
    return (uint8_t*)(MAL_Buffer);
#else
    return  (uint8_t*)(Add);
#endif /* USB_INTERNAL_DMA_ENABLED */
}

/**
  * @brief  OTP_If_CheckAdd
  *         Check if the address is an allowed address for this memory.
  * @param  Add: Address to be checked.
  * @param  Len: Number of data to be read (in bytes).
  * @retval MAL_OK if the address is allowed, MAL_FAIL else.
  */
uint16_t OTP_If_CheckAdd(uint32_t Add)
{
    if ((Add >= OTP_START_ADD) && (Add < OTP_END_ADD))
    {
        return MAL_OK;
    }
    else
    {
        return MAL_FAIL;
    }
}
