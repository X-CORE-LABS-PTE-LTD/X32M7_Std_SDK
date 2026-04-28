/**
 ******************************************************************************
 * @file      usbhs_defines.h
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

#ifndef __USBHS_DEF_H__
#define __USBHS_DEF_H__

#include  "usbhs_conf.h"


#define USB_SPEED_PARAM_HIGH 0
#define USB_SPEED_PARAM_HIGH_IN_FULL 1

#define USB_EMBEDDED_PHY    1

#define GAHBCFG_TXFEMPTYLVL_EMPTY              1
#define GAHBCFG_TXFEMPTYLVL_HALFEMPTY          0
#define GAHBCFG_GLBINT_ENABLE                  1
#define GAHBCFG_INT_DMA_BURST_SINGLE           0
#define GAHBCFG_INT_DMA_BURST_INCR             1
#define GAHBCFG_INT_DMA_BURST_INCR4            3
#define GAHBCFG_INT_DMA_BURST_INCR8            5
#define GAHBCFG_INT_DMA_BURST_INCR16           7
#define GAHBCFG_DMAENABLE                      1
#define GAHBCFG_TXFEMPTYLVL_EMPTY              1
#define GAHBCFG_TXFEMPTYLVL_HALFEMPTY          0
#define GRXSTS_PKTSTS_IN                       2
#define GRXSTS_PKTSTS_IN_XFER_COMP             3
#define GRXSTS_PKTSTS_DATA_TOGGLE_ERR          5
#define GRXSTS_PKTSTS_CH_HALTED                7


#define A_HOST                                 1
#define A_SUSPEND                              2
#define A_PERIPHERAL                           3
#define B_PERIPHERAL                           4
#define B_HOST                                 5
#define DEVICE_MODE                            0
#define HOST_MODE                              1
#define OTG_MODE                               2

#define DSTS_ENUMSPD_HS_PHY_30MHZ_OR_60MHZ     0
#define DSTS_ENUMSPD_FS_PHY_30MHZ_OR_60MHZ     1
#define DSTS_ENUMSPD_LS_PHY_6MHZ               2
#define DSTS_ENUMSPD_FS_PHY_48MHZ              3

#define DCFG_FRAME_INTERVAL_80                 0
#define DCFG_FRAME_INTERVAL_85                 1
#define DCFG_FRAME_INTERVAL_90                 2
#define DCFG_FRAME_INTERVAL_95                 3

#define DEP0CTRL_MPS_64                         0
#define DEP0CTRL_MPS_32                         1
#define DEP0CTRL_MPS_16                         2
#define DEP0CTRL_MPS_8                          3

#define EP_SPEED_LOW                           0
#define EP_SPEED_FULL                          1
#define EP_SPEED_HIGH                          2

#define EP_TYPE_CTRL                           0
#define EP_TYPE_ISOC                           1
#define EP_TYPE_BULK                           2
#define EP_TYPE_INTR                           3
#define EP_TYPE_MSK                            3

#define STS_GOUT_NAK                           1
#define STS_DATA_UPDT                          2
#define STS_XFER_COMP                          3
#define STS_SETUP_COMP                         4
#define STS_SETUP_UPDT                         6

#define HCH_PID_DATA0                          0
#define HCH_PID_DATA2                          1
#define HCH_PID_DATA1                          2
#define HCH_PID_SETUP                          3

#define HPCS_PRTSPD_HIGH_SPEED                 0
#define HPCS_PRTSPD_FULL_SPEED                 1
#define HPCS_PRTSPD_LOW_SPEED                  2

#define HCFG_30_60_MHZ                         0
#define HCFG_48_MHZ                            1
#define HCFG_6_MHZ                             2

#define HCHCTRL_CTRL                           0
#define HCHCTRL_ISOC                           1
#define HCHCTRL_BULK                           2
#define HCHCTRL_INTR                           3

#ifndef MIN
#define MIN(a, b)      (((a) < (b)) ? (a) : (b))
#endif

typedef enum
{
  USBHS1_CORE_ID = 0,
  USBHS2_CORE_ID = 1
}USB_CORE_ID_TypeDef;

typedef enum
{
    USB_ENUM_HS = 0,
    USB_ENUM_FS = 1
}USB_ENUM_CORE;


#define USB_READ_REG32(REG) (*(__IO uint32_t *)(REG))
#define USB_WRITE_REG32(REG, VAL) (*(__IO uint32_t *)(REG) = (VAL))
#define USB_MODIFY_REG32(REG, clear_mask, set_mask)  USB_WRITE_REG32((REG), (((USB_READ_REG32(REG)) & ~(clear_mask)) | (set_mask)))

#define USB_SET_REG32_BIT(REG, BIT) ((*(__IO uint32_t *)(REG)) |= ((uint32_t)(BIT)))
#define USB_READ_REG32_BIT(REG, BIT) ((*(__IO uint32_t *)(REG)) &= ((uint32_t)(BIT)))
#define USB_CLR_REG32_BIT(REG, BIT) ((*(__IO uint32_t *)(REG)) &= ((uint32_t)(~((uint32_t)(BIT)))))

#define USB_CLR_REG32(REG) (*(__IO uint32_t *)(REG) = ((uint32_t)(0UL)))

/** ENUMERATION TYPE **/
enum USB_SPEED
{
    USB_SPEED_UNKNOWN = 0,
    USB_SPEED_LOW,
    USB_SPEED_FULL,
    USB_SPEED_HIGH
};

#endif /* __USB_DEFINES__H__ */
