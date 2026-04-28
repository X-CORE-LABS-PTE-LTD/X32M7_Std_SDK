/**
 ******************************************************************************
 * @file      usbh_cdc_core.h
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

#ifndef __USBH_CDC_CORE_H
#define __USBH_CDC_CORE_H


#include "usbh_cdc_funct.h"



/*Comuncation Calss codes*/
#define COMMUNICATION_DEVICE_CLASS_CODE                         0x02
#define COMMUNICATION_INTERFACE_CLASS_CODE                      0x02

/*Data Interface Class Codes*/
#define DATA_INTERFACE_CLASS_CODE                               0x0A

/*Communcation sub class codes*/
#define RESERVED                                                0x00
#define DIRECT_LINE_CONTROL_MODEL                               0x01
#define ABSTRACT_CONTROL_MODEL                                  0x02
#define TELEPHONE_CONTROL_MODEL                                 0x03
#define MULTICHANNEL_CONTROL_MODEL                              0x04   
#define CAPI_CONTROL_MODEL                                      0x05
#define ETHERNET_NETWORKING_CONTROL_MODEL                       0x06
#define ATM_NETWORKING_CONTROL_MODEL                            0x07


/*Communication Interface Class Control Protocol Codes*/
#define NO_CLASS_SPECIFIC_PROTOCOL_CODE                         0x00
#define COMMON_AT_COMMAND                                       0x01
#define VENDOR_SPECIFIC                                         0xFF


#define CS_INTERFACE                                            0x24
#define CDC_PAGE_SIZE_64                                        0x40


/* States for CDC State Machine */
typedef enum
{
    CDC_IDLE= 0,
    CDC_READ_DATA,
    CDC_SEND_DATA,
    CDC_DATA_SENT,
    CDC_BUSY,
    CDC_GET_DATA,   
    CDC_POLL,
    CDC_CTRL_STATE
}
CDC_State;

/* CDC Transfer State */
typedef struct _CDCXfer
{
    volatile CDC_State CDCState;
    uint8_t* pRxTxBuff;
    uint8_t* pFillBuff;
    uint8_t* pEmptyBuff;
    uint32_t BufferLen;
    uint16_t DataLength;
} CDC_Xfer_TypeDef;

typedef struct CDC_UserCb
{
    void  (*Send)       (uint8_t  *);             
    void  (*Receive)    (uint8_t *);       

} CDC_Usercb_TypeDef;

/* Structure for CDC process */
typedef struct _CDC_CommInterface
{
    uint8_t              hc_num_in; 
    uint8_t              hc_num_out;
    uint8_t              notificationEp;
    CDC_State            state; 
    uint8_t              buff[8];
    uint16_t             length;
    uint8_t              ep_addr;  
}
CDC_CommInterface_Typedef ;

typedef struct _CDC_DataInterface
{
    uint8_t              hc_num_in; 
    uint8_t              hc_num_out;
    uint8_t              cdcOutEp;
    uint8_t              cdcInEp;
    CDC_State            state; 
    uint8_t              buff[8];
    uint16_t             length;
    uint8_t              ep_addr;
}
CDC_DataInterface_Typedef ;

/* Structure for CDC process */
typedef struct _CDC_Process
{
    CDC_CommInterface_Typedef CDC_CommItf;
    CDC_DataInterface_Typedef CDC_DataItf;
}
CDC_Machine_TypeDef;

extern USBH_Class_cb_TypeDef  USBH_CDC_cb;

void  CDC_SendData(uint8_t *data, uint16_t length);
void  CDC_StartReception( USB_CORE_MODULE *pdev);
void  CDC_StopReception( USB_CORE_MODULE *pdev);


#endif /* __USBH_CDC_CORE_H */
