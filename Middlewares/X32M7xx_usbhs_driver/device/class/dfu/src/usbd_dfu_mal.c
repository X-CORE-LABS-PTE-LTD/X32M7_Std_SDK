/**
 ******************************************************************************
 * @file      usbd_dfu_mal.c
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
#include "usbd_dfu_mal.h"

#include "usbd_flash_if.h"
#include "x32m7xx_crc.h"

#ifdef DFU_MAL_SUPPORT_OTP
 #include "usbd_otp_if.h"
#endif

#ifdef DFU_MAL_SUPPORT_MEM
 #include "usbd_mem_if_template.h"
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Global Memories callback and string descriptors reference tables.
   To add a new memory, modify the value of MAX_USED_MEDIA in usbd_dfu_mal.h
   and add the pointer to the callback structure in this table.
   Then add the pointer to the memory string descriptor in usbd_dfu_StringDesc table.
   No other operation is required. */
DFU_MAL_Prop_TypeDef* tMALTab[MAX_USED_MEDIA] = 
{
    &DFU_Flash_cb
#ifdef DFU_MAL_SUPPORT_OTP
  , &DFU_Otp_cb
#endif
#ifdef DFU_MAL_SUPPORT_MEM
  , &DFU_Mem_cb
#endif
};

#ifdef USB_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_INTERNAL_DMA_ENABLED */

__ALIGN_BEGIN const uint8_t* usbd_dfu_StringDesc[MAX_USED_MEDIA] __ALIGN_END  = {
    FLASH_IF_STRING
#ifdef DFU_MAL_SUPPORT_OTP
  , OTP_IF_STRING
#endif
#ifdef DFU_MAL_SUPPORT_MEM
  , MEM_IF_STRING
#endif
};

#ifdef USB_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_INTERNAL_DMA_ENABLED */
/* RAM Buffer for Downloaded Data */
__ALIGN_BEGIN uint8_t  MAL_Buffer[XFERSIZE] __ALIGN_END ; 

/* Private function prototypes -----------------------------------------------*/
static uint8_t  MAL_CheckAdd  (uint32_t Add);
/* Private functions ---------------------------------------------------------*/


BOOTLOADER_CMD_Type bl_cmd;
BOOTLOADER_IC_CMD_Type ic_cmd;

#define P_UCID                    ((uint8_t *) 0x15000000)//((uint8_t *) 0x1FFFF7C0)
#define P_CHIP_ID                 ((uint8_t *) 0x15000010)//((uint8_t *) 0x1FFFF7F0)
#define P_IDCODE                  ((uint8_t *) 0x15000020)//((uint8_t *) 0x1FFFF7FC)
#define P_CHIP_SERIES             ((uint8_t *) 0x15000030)//((uint8_t *) 0x1FFFF7E0)

uint8_t cmd_return_par;
uint8_t Return_Par[6][2] = 
{
    {0xA0,0xB0},{0xE0,0x10},{0xE0,0x11},{0xE0,0x12},{0xE0,0x13},{0xBB,0xCC}
};

//uint8_t Return_Info_CR[25][2] __attribute__((at(0x1FFFF780)));

/**
  * @brief  MAL_Init
  *         Initializes the Media on the X32
  * @param  None
  * @retval Result of the operation (MAL_OK in all cases)
  */
uint16_t MAL_Init(void)
{
    uint32_t memIdx = 0;

    /* Init all supported memories */
    for(memIdx = 0; memIdx < MAX_USED_MEDIA; memIdx++)
    {
        /* If the check addres is positive, exit with the memory index */
        if (tMALTab[memIdx]->pMAL_Init != NULL)
        {
            tMALTab[memIdx]->pMAL_Init();
        }
    }

    return MAL_OK;
}

/**
  * @brief  MAL_DeInit
  *         DeInitializes the Media on the X32
  * @param  None
  * @retval Result of the operation (MAL_OK in all cases)
  */
uint16_t MAL_DeInit(void)
{
    uint32_t memIdx = 0;

    /* Init all supported memories */
    for(memIdx = 0; memIdx < MAX_USED_MEDIA; memIdx++)
    {
        /* Check if the command is supported */
        if (tMALTab[memIdx]->pMAL_DeInit != NULL)
        {
            tMALTab[memIdx]->pMAL_DeInit();
        }
    }

    return MAL_OK;
}

/**
  * @brief  MAL_Erase
  *         Erase a sector of memory.
  * @param  Add: Sector address/code
  * @retval Result of the operation: MAL_OK if all operations are OK else MAL_FAIL
  */
uint16_t MAL_Erase(uint32_t Add)
{
    uint32_t memIdx = MAL_CheckAdd(Add);

    /* Check if the area is protected */
    if (DFU_MAL_IS_PROTECTED_AREA(Add))
    {
        return MAL_FAIL;
    }    

    if (memIdx < MAX_USED_MEDIA)
    {
        /* Check if the command is supported */
        if (tMALTab[memIdx]->pMAL_Erase != NULL)
        {
            return tMALTab[memIdx]->pMAL_Erase(Add);
        }
        else
        {
            return MAL_FAIL;
        }
    }
    else
    {
        return MAL_FAIL;
    }
}

/**
  * @brief  MAL_Write
  *         Write sectors of memory.
  * @param  Add: Sector address/code
  * @param  Len: Number of data to be written (in bytes)
  * @retval Result of the operation: MAL_OK if all operations are OK else MAL_FAIL
  */
uint16_t MAL_Write (uint32_t Add, uint32_t Len)
{
    uint32_t memIdx = MAL_CheckAdd(Add);

    /* Check if the area is protected */
    if (DFU_MAL_IS_PROTECTED_AREA(Add))
    {
        return MAL_FAIL;
    }
    if (memIdx < MAX_USED_MEDIA)
    {
        /* Check if the command is supported */
        if (tMALTab[memIdx]->pMAL_Write != NULL)
        {
            return tMALTab[memIdx]->pMAL_Write(Add, Len);
        }
        else
        {
            return MAL_FAIL;
        }    
    }
    else
    {
        return MAL_FAIL;
    }
}

/**
  * @brief  MAL_Read
  *         Read sectors of memory.
  * @param  Add: Sector address/code
  * @param  Len: Number of data to be written (in bytes)
  * @retval Buffer pointer
  */
uint8_t *MAL_Read (uint32_t Add, uint32_t Len)
{
    uint32_t memIdx = MAL_CheckAdd(Add);

    if (memIdx < MAX_USED_MEDIA)
    {
        /* Check if the command is supported */
        if (tMALTab[memIdx]->pMAL_Read != NULL)
        {
            return tMALTab[memIdx]->pMAL_Read(Add, Len);
        }
        else
        {
            return MAL_Buffer;
        }     
    }
    else
    {
        return MAL_Buffer;
    }
}

/**
  * @brief  MAL_GetStatus
  *         Get the status of a given memory.
  * @param  Add: Sector address/code (allow to determine which memory will be addressed)
  * @param  Cmd: 0 for erase and 1 for write
  * @param  buffer: pointer to the buffer where the status data will be stored.
  * @retval Buffer pointer
  */
uint16_t MAL_GetStatus(uint32_t Add , uint8_t Cmd, uint8_t *buffer)
{
    uint32_t memIdx = MAL_CheckAdd(Add);

    if (memIdx < MAX_USED_MEDIA)
    {
        if (Cmd & 0x01)
        {
            SET_POLLING_TIMING(tMALTab[memIdx]->EraseTiming);
        }
        else
        {
            SET_POLLING_TIMING(tMALTab[memIdx]->WriteTiming);
        }

        return MAL_OK;
    }
    else
    {
        return MAL_FAIL;
    }
}

/**
  * @brief  MAL_CheckAdd
  *         Determine which memory should be managed.
  * @param  Add: Sector address/code (allow to determine which memory will be addressed)
  * @retval Index of the addressed memory.
  */
static uint8_t  MAL_CheckAdd(uint32_t Add)
{
    uint32_t memIdx = 0;

    /* Check with all supported memories */
    for(memIdx = 0; memIdx < MAX_USED_MEDIA; memIdx++)
    {
        /* If the check addres is positive, exit with the memory index */
        if (tMALTab[memIdx]->pMAL_CheckAdd(Add) == MAL_OK)
        {
            return memIdx;
        }
    }
    /* If no memory found, return MAX_USED_MEDIA */
    return (MAX_USED_MEDIA);
}
