/**
 ******************************************************************************
 * @file      usbh_core.h
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

#ifndef __USBH_CORE_H__
#define __USBH_CORE_H__

/* Includes ------------------------------------------------------------------*/
#include "usbhs_hcd.h"
#include "usbh_def.h"
#include "usbh_conf.h"
#include "usbhs_core.h"


#define MSC_CLASS                             0x08 /* MSC */
#define HID_CLASS                             0x03 /* HID */

#define USB_AUDIO                             0x01 /* Audio */
#define USB_CDCC                              0x02  /* Communications and CDC Control */
#define USB_HID                               0x03  /* HID (Human Interface Device) */
#define USB_PRINTER                           0x07  /* Printer */
#define USB_MSC                               0x08  /* Mass Storage */
#define USB_HUB                               0x09  /* Hub */
#define USB_CDCD                              0x0A  /* CDC-Data */
#define USB_SMARTCARD                         0x0B  /* Smart Card */
#define USB_VIDEO                             0x0E  /* Video */
#define USB_AVD                               0x10  /* Audio/Video Devices */



#define MSC_PROTOCOL                      0x50
#define CBI_PROTOCOL                      0x01


#define USBH_MAX_ERROR_COUNT                            2
#define USBH_DEVICE_ADDRESS_DEFAULT                     0
#define USBH_DEVICE_ADDRESS                             1
#define CFG_DESC_MAX_SIZE                               512


typedef enum 
{
    USBH_OK   = 0,
    USBH_BUSY,
    USBH_FAIL,
    USBH_NOT_SUPPORTED,
    USBH_UNRECOVERED_ERROR,
    USBH_ERROR_SPEED_UNKNOWN,
    USBH_APPLY_DEINIT
}USBH_Status;

/* Following states are used for gState */
typedef enum
{
    HOST_IDLE =0,
    HOST_WAIT_PRT_ENABLED,
    HOST_DEV_ATTACHED,
    HOST_DEV_DISCONNECTED,  
    HOST_DETECT_DEVICE_SPEED,
    HOST_ENUMERATION,
    HOST_CLASS_REQUEST,  
    HOST_CLASS,
    HOST_CTRL_XFER,
    HOST_USR_INPUT,
    HOST_SUSPENDED,
    HOST_WAKEUP,
    HOST_ERROR_STATE
}HOST_State;  
    

/* Following states are used for EnumerationState */
typedef enum
{
    ENUM_IDLE = 0,
    ENUM_GET_FULL_DEV_DESC,
    ENUM_SET_ADDR,
    ENUM_GET_CFG_DESC,
    ENUM_GET_FULL_CFG_DESC,
    ENUM_GET_MFC_STRING_DESC,
    ENUM_GET_PRODUCT_STRING_DESC,
    ENUM_GET_SERIALNUM_STRING_DESC,
    ENUM_SET_CONFIGURATION,
    ENUM_DEV_CONFIGURED
}ENUM_State;  



/* Following states are used for CtrlXferStateMachine */
typedef enum
{
    CTRL_IDLE =0,
    CTRL_SETUP,
    CTRL_SETUP_WAIT,
    CTRL_DATA_IN,
    CTRL_DATA_IN_WAIT,
    CTRL_DATA_OUT,
    CTRL_DATA_OUT_WAIT,
    CTRL_STATUS_IN,
    CTRL_STATUS_IN_WAIT,
    CTRL_STATUS_OUT,
    CTRL_STATUS_OUT_WAIT,
    CTRL_ERROR,
    CTRL_STALLED,
    CTRL_COMPLETE    
}CTRL_State;  

typedef enum
{
    USBH_USER_NO_RESP   = 0,
    USBH_USER_RESP_OK = 1,
}USBH_USER_Status;

/* Following states are used for RequestState */
typedef enum
{
    CMD_IDLE =0,
    CMD_SEND,
    CMD_WAIT
}CMD_State;  

typedef struct
{
    uint8_t               hch_num_in; 
    uint8_t               hch_num_out; 
    uint8_t               ep0size;  
    uint8_t               *buff;
    uint16_t              length;
    uint8_t               errorcount;
    uint16_t              timer;  
    CTRL_STATUS           status;
    USB_Setup_TypeDef     setup;
    CTRL_State            state;  

}USBH_Ctrl_TypeDef;

typedef struct
{
    uint8_t                           address;
    uint8_t                           speed;
    USBH_DevDesc_TypeDef              Dev_Desc;
    USBH_CfgDesc_TypeDef              Cfg_Desc;  
    USBH_InterfaceDesc_TypeDef        Itf_Desc[USBH_MAX_NUM_INTERFACES];
    USBH_EpDesc_TypeDef               Ep_Desc[USBH_MAX_NUM_INTERFACES][USBH_MAX_NUM_ENDPOINTS];
    USBH_HIDDesc_TypeDef              HID_Desc;
}USBH_Device_TypeDef;

typedef struct
{
    USBH_Status  (*Init)(USB_CORE_MODULE *USBx , void *phost);
    void         (*DeInit)(USB_CORE_MODULE *USBx , void *phost);
    USBH_Status  (*Requests)(USB_CORE_MODULE *USBx ,void *phost);  
    USBH_Status  (*Machine)(USB_CORE_MODULE *USBx, void *phost);     
}USBH_Class_cb_TypeDef;


typedef struct
{
    void (*Init)(void);       /* HostLibInitialized */
    void (*DeInit)(void);       /* HostLibInitialized */  
    void (*DeviceAttached)(void);           /* DeviceAttached */
    void (*ResetDevice)(void);
    void (*DeviceDisconnected)(void); 
    void (*OverCurrentDetected)(void);  
    void (*DeviceSpeedDetected)(uint8_t DeviceSpeed);          /* DeviceSpeed */
    void (*DeviceDescAvailable)(void *);    /* DeviceDescriptor is available */
    void (*DeviceAddressAssigned)(void);  /* Address is assigned to USB Device */
    void (*ConfigurationDescAvailable)(USBH_CfgDesc_TypeDef *,
                                     USBH_InterfaceDesc_TypeDef *,
                                     USBH_EpDesc_TypeDef *); 
    /* Configuration Descriptor available */
    void (*ManufacturerString)(void *);     /* ManufacturerString*/
    void (*ProductString)(void *);          /* ProductString*/
    void (*SerialNumString)(void *);        /* SerialNubString*/
    void (*EnumerationDone)(void);           /* Enumeration finished */
    USBH_USER_Status (*UserInput)(void);
    int  (*UserApplication) (void);
    void (*DeviceNotSupported)(void); /* Device is not supported*/
    void (*UnrecoveredError)(void);
}USBH_User_cb_TypeDef;

typedef struct
{
    HOST_State            gState;       /*  Host State Machine Value */
    HOST_State            gStateBkp;    /* backup of previous State machine value */
    ENUM_State            EnumState;    /* Enumeration state Machine */
    CMD_State             RequestState;       
    USBH_Ctrl_TypeDef     Control;

    USBH_Device_TypeDef   device_prop; 

    USBH_Class_cb_TypeDef *class_cb;  
    USBH_User_cb_TypeDef   *usr_cb;
} USBH_HOST;

void USBH_Init(USB_CORE_MODULE *USBx, USB_CORE_ID_TypeDef coreID, USBH_HOST *host, USBH_Class_cb_TypeDef *class_cb, USBH_User_cb_TypeDef *user_cb);
               
USBH_Status USBH_DeInit(USB_CORE_MODULE *USBx, USBH_HOST *phost);
void USBH_Main_Process(USB_CORE_MODULE *USBx, USB_CORE_ID_TypeDef coreID, USBH_HOST *phost);
void USBH_ErrorHandle(USBH_HOST *phost, USBH_Status errType);

#endif /* __USBH_CORE_H__ */
