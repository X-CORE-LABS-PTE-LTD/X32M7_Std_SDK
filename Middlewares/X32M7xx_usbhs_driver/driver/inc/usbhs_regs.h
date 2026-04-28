/**
 ******************************************************************************
 * @file      usbhs_regs.h
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

#ifndef __USBHS_REGS_H__
#define __USBHS_REGS_H__

#include "x32m7xx.h"
#include "usbhs_conf.h"


#define USBHS1_BASE_ADDR                  (0x40100000)
#define USBHS1_WARP_BASE_ADDR             (0x40140000)
#define USBHS2_BASE_ADDR                  (0x40060000)
#define USBHS2_WARP_BASE_ADDR             (0x400A0000)

#define USB_GLOBAL_REGS_OFFSET            (0x000)
#define USB_DEV_GLOBAL_REG_OFFSET         (0x800)
#define USB_DEV_IN_EP_REG_OFFSET          (0x900)
#define USB_EP_REG_OFFSET                 (0x20)
#define USB_DEV_OUT_EP_REG_OFFSET         (0xB00)
#define USB_HOST_GLOBAL_REG_OFFSET        (0x400)
#define USB_HOST_PORT_REGS_OFFSET         (0x440)
#define USB_HOST_CHANNEL_REGS_OFFSET      (0x500)
#define USB_CHANNEL_REGS_OFFSET           (0x20)
#define USB_PWRCTRL_OFFSET                (0xE00)
#define USB_PWRCTRL1_OFFSET               (0xE04)
#define USB_DATA_FIFO_OFFSET              (0x1000)
#define USB_DATA_FIFO_SIZE                (0x1000)

#define USB_MAX_TX_FIFOS                  (16U)
#define USB_MAX_CH_NUM                    (USB_MAX_TX_FIFOS)
#define USB_MAX_EP_NUM                    (16U)
#define USB_MAX_EP0_SIZE                  (64U)

#define USB_HS_MAX_PACKET_SIZE            (1024U)
#define USB_FS_MAX_PACKET_SIZE            (64U)
#define USB_MAX_CHANNEL_NUM               (16U)           /* USBHS host channel count */
#define USB_MAX_FIFO_WORDLEN              (1280U)         /* USBHS max fifo size in words */


typedef struct 
{
    USB_Global_Registers   *GCSR;
    USB_Device_Registers   *DCSR;
    USB_Host_Registers     *HCSR;
    USB_INEP_Registers     *INEPCSR[USB_MAX_EP_NUM];
    USB_OUTEP_Registers    *OUTEPCSR[USB_MAX_EP_NUM];
    USB_HostCH_Registers   *HCHCSR[USB_MAX_CH_NUM];
    __IO uint32_t          *HPCS;
    __IO uint32_t          *DFIFO[USB_MAX_TX_FIFOS];
    __IO uint32_t          *PWRCTRL;
    __IO uint32_t          *PWRCTRL1;
    
    USB_Wrapper_Registers  *WRAPCSR;
}USB_Register;

#endif /* __USBHS_REGS_H__ */
