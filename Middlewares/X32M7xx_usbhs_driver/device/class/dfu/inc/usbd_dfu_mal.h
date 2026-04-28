/**
 ******************************************************************************
 * @file      usbd_dfu_mal.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DFU_MAL_H
#define __DFU_MAL_H

/* Includes ------------------------------------------------------------------*/
#include "usbhs_conf.h"

#include "usbd_conf.h"
#include "usbd_dfu_core.h"

/* Exported types ------------------------------------------------------------*/
typedef struct _DFU_MAL_PROP
{
  const uint8_t* pStrDesc;
  uint16_t (*pMAL_Init)     (void);   
  uint16_t (*pMAL_DeInit)   (void);   
  uint16_t (*pMAL_Erase)    (uint32_t Add);
  uint16_t (*pMAL_Write)    (uint32_t Add, uint32_t Len);
  uint8_t  *(*pMAL_Read)    (uint32_t Add, uint32_t Len);
  uint16_t (*pMAL_CheckAdd) (uint32_t Add);
  const uint32_t EraseTiming;
  const uint32_t WriteTiming;
}
DFU_MAL_Prop_TypeDef;


/* Exported constants --------------------------------------------------------*/
#define MAL_OK                          0
#define MAL_FAIL                        1

#define FLASH_2KB                     ((uint32_t)0x800)

/* useful macro ---------------------------------------------------------------*/
#define _1st_BYTE(x)  (uint8_t)((x)&0xFF)             /* 1st addressing cycle */
#define _2nd_BYTE(x)  (uint8_t)(((x)&0xFF00)>>8)      /* 2nd addressing cycle */
#define _3rd_BYTE(x)  (uint8_t)(((x)&0xFF0000)>>16)   /* 3rd addressing cycle */
#define _4th_BYTE(x)  (uint8_t)(((x)&0xFF000000)>>24) /* 4th addressing cycle */

/* Exported macro ------------------------------------------------------------*/
#define SET_POLLING_TIMING(x)   buffer[1] = _1st_BYTE(x);\
                                buffer[2] = _2nd_BYTE(x);\
                                buffer[3] = _3rd_BYTE(x);  

typedef enum 
{
    CMD_GET_INF             = 0x10,
    CMD_FLASH_ERASE         = 0x30, 
    CMD_FLASH_DWNLD         = 0x31, 
    CMD_DATA_CRC_CHECK      = 0x32, 
    CMD_SYS_RESET           = 0x50, 
    CMD_APP_GO              = 0x51
} CMD_H_NUM;

typedef enum 
{
    CMD_PAR_SUCCESS      = 0x00,
    CMD_PAR_FAILED       = 0x01,
    CMD_PAR_CRCERR       = 0x10,
    CMD_PAR_FLASHLIMET   = 0x0C,
    CMD_PAR_ADDERR       = 0x0D,
    CMD_PAR_LENERR       = 0x0E,
    CMD_PAR_PROGRAMERR   = 0x0F,
} CMD_RETURN_PAR;

typedef struct
{
    uint8_t CMD_H;
    uint8_t CMD_L;
    uint8_t LEN[2];
    uint8_t Par[4];
    uint8_t Data[256];
    uint8_t Flag;
} BOOTLOADER_CMD_Type;

typedef struct
{
    uint8_t CMD_H;
    uint8_t CMD_L;
    uint8_t LEN[2];
    uint8_t Data[256];
    uint8_t CR1;
    uint8_t CR2;
} BOOTLOADER_IC_CMD_Type;

/* Exported functions ------------------------------------------------------- */
void Bootloader_CMD(void);
void Bootloader_Return_CMD(void);

uint16_t MAL_Init (void);
uint16_t MAL_DeInit (void);
uint16_t MAL_Erase (uint32_t SectorAddress);
uint16_t MAL_Write (uint32_t SectorAddress, uint32_t DataLength);
uint8_t *MAL_Read  (uint32_t SectorAddress, uint32_t DataLength);
uint16_t MAL_GetStatus(uint32_t SectorAddress ,uint8_t Cmd, uint8_t *buffer);

extern uint8_t  MAL_Buffer[XFERSIZE]; /* RAM Buffer for Downloaded Data */
#endif /* __DFU_MAL_H */

