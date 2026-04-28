/**
 ******************************************************************************
 * @file      usbhs_core.h
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

#ifndef __USBHS_CORE_H__
#define __USBHS_CORE_H__

/* Includes ------------------------------------------------------------------*/
#include "usbhs_conf.h"
#include "usbhs_regs.h"
#include "usbhs_defines.h"


#define USB_EP0_IDLE              0
#define USB_EP0_SETUP             1
#define USB_EP0_DATA_IN           2
#define USB_EP0_DATA_OUT          3
#define USB_EP0_STATUS_IN         4
#define USB_EP0_STATUS_OUT        5
#define USB_EP0_STALL             6

#define USB_EP_TX_DIS             0x0000
#define USB_EP_TX_STALL           0x0010
#define USB_EP_TX_NAK             0x0020
#define USB_EP_TX_VALID           0x0030
      
#define USB_EP_RX_DIS             0x0000
#define USB_EP_RX_STALL           0x1000
#define USB_EP_RX_NAK             0x2000
#define USB_EP_RX_VALID           0x3000

#define MAX_DATA_LENGTH           0x200


typedef enum
{
    USB_OK = 0,
    USB_FAIL
}USB_STS;

typedef enum
{
    HCH_IDLE = 0,
    HCH_XFRC,
    HCH_HALTED,
    HCH_NAK,
    HCH_NYET,
    HCH_STALL,
    HCH_XACTERR,  
    HCH_BBLERR,   
    HCH_DATATGLERR,  
}HCH_STATUS; /* USB Host channel status */

typedef enum
{
    URB_IDLE = 0,
    URB_DONE,
    URB_NOTREADY,
    URB_ERROR,
    URB_STALL
}URB_STATE; /* USB Request Block */

typedef enum
{
    CTRL_START = 0,
    CTRL_XFRC,
    CTRL_HALTED,
    CTRL_NAK,
    CTRL_STALL,
    CTRL_XACTERR,  
    CTRL_BBLERR,   
    CTRL_DATATGLERR,  
    CTRL_FAIL
}CTRL_STATUS;


typedef struct
{
    uint8_t       dev_addr;
    uint8_t       ep_num;
    uint8_t       ep_is_in;
    uint8_t       speed;
    uint8_t       do_ping;  
    uint8_t       ep_type;
    uint16_t      max_packet;
    uint8_t       data_pid;
    uint8_t       *xfer_buff;
    uint32_t      xfer_len;
    uint32_t      xfer_count;  
    uint8_t       toggle_in;
    uint8_t       toggle_out;
    uint32_t      dma_addr;  
}USB_HCH;

typedef struct
{
    uint8_t        num;
    uint8_t        is_in;
    uint8_t        is_stall;  
    uint8_t        type;
    uint8_t        data_pid_start;
    uint8_t        even_odd_frame;
    uint16_t       tx_fifo_num;
    uint32_t       maxpacket;
    /* transaction level variables*/
    uint8_t        *xfer_buff;
    uint32_t       dma_addr;  
    uint32_t       xfer_len;
    uint32_t       xfer_count;
    /* Transfer level variables*/  
    uint32_t       rem_data_len;
    uint32_t       total_data_len;
    uint32_t       ctrl_data_len;  
}USB_EP;


typedef struct
{
    uint8_t       host_channels_num;
    uint8_t       dev_endpoints_num;
    uint8_t       speed;
    uint8_t       dma_enable;
    uint16_t      mps;
    uint16_t      TotalFifoSize;
    uint8_t       low_power;
    uint8_t       Reseved[3];
}USB_CORE_CFGS;


typedef struct
{
    uint8_t   bmRequest;
    uint8_t   bRequest;
    uint16_t  wValue;
    uint16_t  wIndex;
    uint16_t  wLength;
}USB_SETUP_REQ;

typedef struct
{
    uint8_t  *(*GetDeviceDescriptor)( uint8_t speed , uint16_t *length);  
    uint8_t  *(*GetLangIDStrDescriptor)( uint8_t speed , uint16_t *length); 
    uint8_t  *(*GetManufacturerStrDescriptor)( uint8_t speed , uint16_t *length);  
    uint8_t  *(*GetProductStrDescriptor)( uint8_t speed , uint16_t *length);  
    uint8_t  *(*GetSerialStrDescriptor)( uint8_t speed , uint16_t *length);  
    uint8_t  *(*GetConfigurationStrDescriptor)( uint8_t speed , uint16_t *length);  
    uint8_t  *(*GetInterfaceStrDescriptor)( uint8_t speed , uint16_t *length); 
} USBD_DEVICE_DESC;

typedef struct
{
    uint8_t  (*Init)         (void *USBx, uint8_t cfgidx);
    uint8_t  (*DeInit)       (void *USBx, uint8_t cfgidx);
    /* Control Endpoints*/
    uint8_t  (*Setup)        (void *USBx, USB_SETUP_REQ *req);  
    uint8_t  (*EP0_TxSent)   (void *USBx);    
    uint8_t  (*EP0_RxReady)  (void *USBx);  
    /* Class Specific Endpoints*/
    uint8_t  (*DataIn)       (void *USBx, uint8_t epnum);
    uint8_t  (*DataOut)      (void *USBx, uint8_t epnum);
    uint8_t  (*SOF)          (void *USBx); 
    uint8_t  (*IsoINIncomplete)  (void *USBx);
    uint8_t  (*IsoOUTIncomplete) (void *USBx);
    uint8_t  *(*GetConfigDescriptor)( uint8_t speed, uint16_t *length);
    uint8_t  *(*GetOtherConfigDescriptor)( uint8_t speed , uint16_t *length);  
    
#ifdef USB_SUPPORT_USER_STRING_DESC 
    uint8_t  *(*GetUserStrDescriptor)( uint8_t speed,uint8_t index,  uint16_t *length);   
#endif  
} USBD_Class_cb_TypeDef;


typedef struct
{
    void (*Init)(void);   
    void (*DeviceReset)(uint8_t speed); 
    void (*DeviceConfigured)(void);
    void (*DeviceSuspended)(void);
    void (*DeviceResumed)(void);  
    void (*DeviceConnected)(void);  
    void (*DeviceDisconnected)(void);    
}USBD_User_cb_TypeDef;

typedef struct
{
    uint8_t      device_config;
    uint8_t      device_state;
    uint8_t      device_status;
    uint8_t      device_old_status;
    uint8_t      device_address;
    uint8_t      connection_status;  
    uint8_t      test_mode;
    uint32_t     DevRemoteWakeup;
    USB_EP       in_ep[USB_MAX_TX_FIFOS];
    USB_EP       out_ep[USB_MAX_TX_FIFOS];
    uint8_t      setup_packet[24];
    USBD_Class_cb_TypeDef         *class_cb;
    USBD_User_cb_TypeDef           *user_cb;
    USBD_DEVICE_DESC              *user_device;  
    uint8_t        *pConfig_descriptor;
}USB_DEV;


typedef struct
{
    uint8_t                  Rx_Buffer [MAX_DATA_LENGTH];  
    __IO uint32_t            ConnSts;
    __IO uint32_t            PortEnabled;
    __IO uint32_t            ErrCnt[USB_MAX_TX_FIFOS];
    __IO uint32_t            XferCnt[USB_MAX_TX_FIFOS];
    __IO HCH_STATUS          HCH_Status[USB_MAX_TX_FIFOS];  
    __IO URB_STATE           URB_State[USB_MAX_TX_FIFOS];
    USB_HCH                  hch[USB_MAX_TX_FIFOS];
    uint16_t                 channel[USB_MAX_TX_FIFOS];
}USB_HOST;


typedef struct
{
    USB_CORE_CFGS   cfg;
    USB_Register    regs;
    
#ifdef USE_DEVICE_MODE
    USB_DEV         dev;
#endif
#ifdef USE_HOST_MODE
    USB_HOST        host;
#endif
}USB_CORE_MODULE;


USB_STS USB_BasicInit(USB_CORE_MODULE *USBx, USB_CORE_ID_TypeDef coreID);
USB_STS USB_CoreInit(USB_CORE_MODULE *USBx);

USB_STS USB_EnableGlobalInt(USB_CORE_MODULE *USBx, FunctionalState cmd);

void* USB_ReadPacket(USB_CORE_MODULE *USBx , uint8_t *dest, uint16_t len);
USB_STS USB_WritePacket(USB_CORE_MODULE *USBx, uint8_t *src, uint8_t ch_ep_num, uint16_t len);
USB_STS USB_FlushTxFifo(USB_CORE_MODULE *USBx , uint32_t num);
USB_STS USB_FlushRxFifo(USB_CORE_MODULE *USBx);

uint32_t USB_ReadCoreItr(USB_CORE_MODULE *USBx);
uint8_t USB_IsHostMode(USB_CORE_MODULE *USBx);
uint8_t USB_IsDeviceMode(USB_CORE_MODULE *USBx);
uint32_t USB_GetMode(USB_CORE_MODULE *USBx);
USB_STS USB_PhyInit(USB_CORE_MODULE *USBx);
USB_STS USB_SetCurrentMode(USB_CORE_MODULE *USBx, uint8_t mode);

/*********************** HOST APIs ********************************************/
#ifdef USE_HOST_MODE
USB_STS USB_CoreInitHost(USB_CORE_MODULE *USBx);
USB_STS USB_EnableHostInt(USB_CORE_MODULE *USBx);
USB_STS USB_HCH_Init(USB_CORE_MODULE *USBx, uint8_t hch_num);
USB_STS USB_HCH_Halt(USB_CORE_MODULE *USBx, uint8_t hch_num);
USB_STS USB_HCH_StartXfer(USB_CORE_MODULE *USBx, uint8_t hch_num);
USB_STS USB_HCH_DoPing(USB_CORE_MODULE *USBx , uint8_t hch_num);

uint32_t USB_ReadHostAllChannels_intr(USB_CORE_MODULE *USBx);
uint32_t USB_ResetPort(USB_CORE_MODULE *USBx);
uint32_t USB_ReadHPCS(USB_CORE_MODULE *USBx);
void USB_DriveVbus(USB_CORE_MODULE *USBx, uint8_t state);
void USB_InitFSLSPClkSel(USB_CORE_MODULE *USBx ,uint8_t freq);
uint8_t USB_IsEvenFrame(USB_CORE_MODULE *USBx) ;
void USB_StopHost(USB_CORE_MODULE *USBx);
#endif

/********************* DEVICE APIs ********************************************/
#ifdef USE_DEVICE_MODE
USB_STS USB_CoreInitDev(USB_CORE_MODULE *USBx);
USB_STS USB_EnableDevInt(USB_CORE_MODULE *USBx);
uint32_t USB_ReadDevAllInEPItr(USB_CORE_MODULE *USBx);
enum USB_SPEED USB_GetDeviceSpeed(USB_CORE_MODULE *USBx);
USB_STS USB_EP0Activate (USB_CORE_MODULE *USBx);
USB_STS USB_EPActivate  (USB_CORE_MODULE *USBx, USB_EP *ep);
USB_STS USB_EPDeactivate(USB_CORE_MODULE *USBx, USB_EP *ep);
USB_STS USB_EPStartXfer (USB_CORE_MODULE *USBx, USB_EP *ep);
USB_STS USB_EP0StartXfer(USB_CORE_MODULE *USBx, USB_EP *ep);
USB_STS USB_EPSetStall(USB_CORE_MODULE *USBx, USB_EP *ep);
USB_STS USB_EPClearStall(USB_CORE_MODULE *USBx, USB_EP *ep);
uint32_t USB_ReadDevAllOutEp_itr(USB_CORE_MODULE *USBx);
uint32_t USB_ReadDevOutEP_itr(USB_CORE_MODULE *USBx, uint8_t epnum);
uint32_t USB_ReadDevAllInEPItr(USB_CORE_MODULE *USBx);
uint32_t USB_ReadDevEachInEPItr(USB_CORE_MODULE *USBx);
uint32_t USB_ReadDevEachOutEPItr(USB_CORE_MODULE *USBx);
void USB_InitDevSpeed(USB_CORE_MODULE *USBx, uint8_t speed);
uint8_t USBH_IsEvenFrame(USB_CORE_MODULE *USBx);
void USB_EP0_OutStart(USB_CORE_MODULE *USBx);
void USB_ActiveRemoteWakeup(USB_CORE_MODULE *USBx);
void USB_UngateClock(USB_CORE_MODULE *USBx);
void USB_StopDevice(USB_CORE_MODULE *USBx);
void USB_SetEPStatus(USB_CORE_MODULE *USBx, USB_EP *ep, uint32_t Status);
uint32_t USB_GetEPStatus(USB_CORE_MODULE *USBx ,USB_EP *ep);
#endif

#endif  /* __USBHS_CORE_H__ */
