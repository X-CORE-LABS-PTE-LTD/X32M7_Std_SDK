/**
 ******************************************************************************
 * @file      usbd_dfu_core.h
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

#ifndef __USB_DFU_CORE_H_
#define __USB_DFU_CORE_H_

#include  "usbd_ioreq.h"
#include  "usbd_dfu_mal.h"

#define USB_DFU_CONFIG_DESC_SIZ       (18 + (9 * USBD_ITF_MAX_NUM))
#define USB_DFU_DESC_SIZ              9

#define DFU_DESCRIPTOR_TYPE           0x21

/**************************************************/
/* DFU Requests  DFU states                       */
/**************************************************/

#define STATE_appIDLE                 0
#define STATE_appDETACH               1
#define STATE_dfuIDLE                 2
#define STATE_dfuDNLOAD_SYNC          3
#define STATE_dfuDNBUSY               4
#define STATE_dfuDNLOAD_IDLE          5
#define STATE_dfuMANIFEST_SYNC        6
#define STATE_dfuMANIFEST             7
#define STATE_dfuMANIFEST_WAIT_RESET  8
#define STATE_dfuUPLOAD_IDLE          9
#define STATE_dfuERROR                10

/**************************************************/
/* DFU Requests  DFU status                       */
/**************************************************/

#define STATUS_OK                   0x00
#define STATUS_ERRTARGET            0x01
#define STATUS_ERRFILE              0x02
#define STATUS_ERRWRITE             0x03
#define STATUS_ERRERASE             0x04
#define STATUS_ERRCHECK_ERASED      0x05
#define STATUS_ERRPROG              0x06
#define STATUS_ERRVERIFY            0x07
#define STATUS_ERRADDRESS           0x08
#define STATUS_ERRNOTDONE           0x09
#define STATUS_ERRFIRMWARE          0x0A
#define STATUS_ERRVENDOR            0x0B
#define STATUS_ERRUSBR              0x0C
#define STATUS_ERRPOR               0x0D
#define STATUS_ERRUNKNOWN           0x0E
#define STATUS_ERRSTALLEDPKT        0x0F

/**************************************************/
/* DFU Requests  DFU states Manifestation State   */
/**************************************************/

#define Manifest_complete           0x00
#define Manifest_In_Progress        0x01


/**************************************************/
/* Special Commands  with Download Request        */
/**************************************************/

#define CMD_GETCOMMANDS              0x00
#define CMD_SETADDRESSPOINTER        0x21
#define CMD_ERASE                    0x41

/**************************************************/
/* Other defines                                  */
/**************************************************/
/* Bit Detach capable = bit 3 in bmAttributes field */
#define DFU_DETACH_MASK              (uint8_t)(1 << 4) 
/**
  * @}
  */ 


/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */
/**************************************************/
/* DFU Requests                                   */
/**************************************************/

typedef enum _DFU_REQUESTS {
  DFU_DETACH = 0,
  DFU_DNLOAD = 1,
  DFU_UPLOAD,
  DFU_GETSTATUS,
  DFU_CLRSTATUS,
  DFU_GETSTATE,
  DFU_ABORT
} DFU_REQUESTS;

typedef  void (*pFunction)(void);
/**
  * @}
  */ 



/** @defgroup USBD_CORE_Exported_Macros
  * @{
  */ 
/**********  Descriptor of DFU interface 0 Alternate setting n ****************/  
#define USBD_DFU_IF_DESC(n)   0x09,   /* bLength: Interface Descriptor size */ \
                              USB_INTERFACE_DESCRIPTOR_TYPE,   /* bDescriptorType */ \
                              0x00,   /* bInterfaceNumber: Number of Interface */ \
                              (n),      /* bAlternateSetting: Alternate setting */ \
                              0x00,   /* bNumEndpoints*/ \
                              0xFE,   /* bInterfaceClass: Application Specific Class Code */ \
                              0x01,   /* bInterfaceSubClass : Device Firmware Upgrade Code */ \
                              0x02,   /* nInterfaceProtocol: DFU mode protocol */ \
                              USBD_IDX_INTERFACE_STR + (n) + 1 /* iInterface: Index of string descriptor */ \
                              /* 18 */
  
/**
  * @}
  */ 

/** @defgroup USBD_CORE_Exported_Variables
  * @{
  */ 

extern USBD_Class_cb_TypeDef  USBD_DFU_cb;
/**
  * @}
  */ 

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */
/**
  * @}
  */ 

#endif /* __USB_DFU_CORE_H_ */
