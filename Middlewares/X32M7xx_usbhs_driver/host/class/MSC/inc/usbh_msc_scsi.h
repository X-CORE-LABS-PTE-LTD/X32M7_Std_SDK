/**
 ******************************************************************************
 * @file      usbh_msc_scsi.h
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

/* Define to prevent recursive  ----------------------------------------------*/
#ifndef __USBH_MSC_SCSI_H__
#define __USBH_MSC_SCSI_H__

/* Includes ------------------------------------------------------------------*/
#include "usbh_stdreq.h"


/** @addtogroup USBH_LIB
  * @{
  */

/** @addtogroup USBH_CLASS
  * @{
  */

/** @addtogroup USBH_MSC_CLASS
  * @{
  */
  
/** @defgroup USBH_MSC_SCSI
  * @brief This file is the Header file for usbh_msc_scsi.c
  * @{
  */ 


/** @defgroup USBH_MSC_SCSI_Exported_Types
  * @{
  */ 
typedef enum {
  USBH_MSC_OK = 0,
  USBH_MSC_FAIL = 1,
  USBH_MSC_PHASE_ERROR = 2,
  USBH_MSC_BUSY = 3
}USBH_MSC_Status_TypeDef;

typedef enum {
  CMD_UNINITIALIZED_STATE =0,
  CMD_SEND_STATE,
  CMD_WAIT_STATUS
} CMD_STATES_TypeDef;  



typedef struct __MassStorageParameter
{
  uint32_t MSCapacity;
  uint32_t MSSenseKey; 
  uint16_t MSPageLength;
  uint8_t MSBulkOutEp;
  uint8_t MSBulkInEp;
  uint8_t MSWriteProtect;
} MassStorageParameter_TypeDef;
/**
  * @}
  */ 



/** @defgroup USBH_MSC_SCSI_Exported_Defines
  * @{
  */ 



#define OPCODE_TEST_UNIT_READY            0X00
#define OPCODE_READ_CAPACITY10            0x25
#define OPCODE_MODE_SENSE6                0x1A
#define OPCODE_READ10                     0x28
#define OPCODE_WRITE10                    0x2A
#define OPCODE_REQUEST_SENSE              0x03

#define DESC_REQUEST_SENSE                0X00
#define ALLOCATION_LENGTH_REQUEST_SENSE   63 
#define XFER_LEN_READ_CAPACITY10           8
#define XFER_LEN_MODE_SENSE6              63

#define MASK_MODE_SENSE_WRITE_PROTECT     0x80
#define MODE_SENSE_PAGE_CONTROL_FIELD     0x00
#define MODE_SENSE_PAGE_CODE              0x3F
#define DISK_WRITE_PROTECTED              0x01
/**
  * @}
  */ 

/** @defgroup USBH_MSC_SCSI_Exported_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup _Exported_Variables
  * @{
  */ 
extern MassStorageParameter_TypeDef USBH_MSC_Param;
/**
  * @}
  */ 

/** @defgroup USBH_MSC_SCSI_Exported_FunctionsPrototype
  * @{
  */ 
uint8_t USBH_MSC_TestUnitReady(USB_CORE_MODULE *pdev);
uint8_t USBH_MSC_ReadCapacity10(USB_CORE_MODULE *pdev);
uint8_t USBH_MSC_ModeSense6(USB_CORE_MODULE *pdev);
uint8_t USBH_MSC_RequestSense(USB_CORE_MODULE *pdev);
uint8_t USBH_MSC_Write10(USB_CORE_MODULE *pdev, uint8_t *,uint32_t ,
                         uint32_t );
uint8_t USBH_MSC_Read10(USB_CORE_MODULE *pdev,
                        uint8_t *,
                        uint32_t ,
                        uint32_t );
void USBH_MSC_StateMachine(USB_CORE_MODULE *pdev);

/**
  * @}
  */ 

#endif  //__USBH_MSC_SCSI_H__
