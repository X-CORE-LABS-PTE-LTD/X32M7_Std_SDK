/**
 ******************************************************************************
 * @file      usbh_hid_core.h
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
#ifndef __USBH_HID_CORE_H
#define __USBH_HID_CORE_H

/* Includes ------------------------------------------------------------------*/
#include "usbh_core.h"
#include "usbh_stdreq.h"
#include "usbhs_bsp.h"
#include "usbh_ioreq.h"
#include "usbh_hcs.h"
 
/** @addtogroup USBH_LIB
  * @{
  */

/** @addtogroup USBH_CLASS
  * @{
  */

/** @addtogroup USBH_HID_CLASS
  * @{
  */
  
/** @defgroup USBH_HID_CORE
  * @brief This file is the Header file for USBH_HID_CORE.c
  * @{
  */ 


/** @defgroup USBH_HID_CORE_Exported_Types
  * @{
  */ 

#define HID_MIN_POLL          10

/* States for HID State Machine */
typedef enum
{
  HID_IDLE= 0,
  HID_SEND_DATA,
  HID_BUSY,
  HID_GET_DATA,   
  HID_SYNC,     
  HID_POLL,
  HID_ERROR,
}
HID_State;

typedef enum
{
  HID_REQ_IDLE = 0,
  HID_REQ_GET_REPORT_DESC,
  HID_REQ_GET_HID_DESC,
  HID_REQ_SET_IDLE,
  HID_REQ_SET_PROTOCOL,
  HID_REQ_SET_REPORT,

}
HID_CtlState;

typedef struct HID_cb
{
  void  (*Init)   (void);             
  void  (*Decode) (uint8_t *data);       
  
} HID_cb_TypeDef;

typedef  struct  _HID_Report 
{
    uint8_t   ReportID;    
    uint8_t   ReportType;  
    uint16_t  UsagePage;   
    uint32_t  Usage[2]; 
    uint32_t  NbrUsage;                      
    uint32_t  UsageMin;                      
    uint32_t  UsageMax;                      
    int32_t   LogMin;                        
    int32_t   LogMax;                        
    int32_t   PhyMin;                        
    int32_t   PhyMax;                        
    int32_t   UnitExp;                       
    uint32_t  Unit;                          
    uint32_t  ReportSize;                    
    uint32_t  ReportCnt;                     
    uint32_t  Flag;                          
    uint32_t  PhyUsage;                      
    uint32_t  AppUsage;                      
    uint32_t  LogUsage;   
} 
HID_Report_TypeDef;

/* Structure for HID process */
typedef struct _HID_Process
{
  uint8_t              buff[64];
  uint8_t              hc_num_in; 
  uint8_t              hc_num_out; 
  HID_State            state; 
  uint8_t              HIDIntOutEp;
  uint8_t              HIDIntInEp;
  HID_CtlState         ctl_state;
  uint16_t             length;
  uint8_t              ep_addr;
  uint16_t             poll; 
  __IO uint16_t        timer; 
  HID_cb_TypeDef             *cb;
}
HID_Machine_TypeDef;

/**
  * @}
  */ 

/** @defgroup USBH_HID_CORE_Exported_Defines
  * @{
  */ 

#define USB_HID_REQ_GET_REPORT       0x01
#define USB_HID_GET_IDLE             0x02
#define USB_HID_GET_PROTOCOL         0x03
#define USB_HID_SET_REPORT           0x09
#define USB_HID_SET_IDLE             0x0A
#define USB_HID_SET_PROTOCOL         0x0B    
/**
  * @}
  */ 

/** @defgroup USBH_HID_CORE_Exported_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USBH_HID_CORE_Exported_Variables
  * @{
  */ 
extern USBH_Class_cb_TypeDef  USBH_HID_cb;
/**
  * @}
  */ 

/** @defgroup USBH_HID_CORE_Exported_FunctionsPrototype
  * @{
  */ 

USBH_Status USBH_Set_Report (USB_CORE_MODULE *pdev,
                             USBH_HOST *phost,
                                  uint8_t reportType,
                                  uint8_t reportId,
                                  uint8_t reportLen,
                                  uint8_t* reportBuff);
/**
  * @}
  */ 


#endif /* __USBH_HID_CORE_H */
