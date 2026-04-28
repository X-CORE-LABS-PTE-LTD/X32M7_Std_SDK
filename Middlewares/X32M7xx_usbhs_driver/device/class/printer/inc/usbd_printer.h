/**
 ******************************************************************************
 * @file      usbd_printer.h
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

#ifndef __USB_PRINTER_H_
#define __USB_PRINTER_H_

#include  "usbd_ioreq.h"

#define USB_PRNT_CONFIG_DESC_SIZ                   (32U)


#define USB_PRNT_BIDIRECTIONAL                     (0x02U)


#define PRINTER_DATA_IN_PACKET_SIZE                PRINTER_DATA_MAX_PACKET_SIZE
#define PRINTER_DATA_OUT_PACKET_SIZE               PRINTER_DATA_MAX_PACKET_SIZE

#define PRINTER_STATUS_PAPER_EMPTY                 0x10U
#define PRINTER_STATUS_SELECTED                    0x08U
#define PRINTER_STATUS_NO_ERROR                    0x00U

/* printing device specific-class request */
#define PRINTER_GET_DEVICE_ID                      0x00U            /*!< get device id request */
#define PRINTER_GET_PORT_STATUS                    0x01U            /*!< get port status request */
#define PRINTER_SOFT_RESET                         0x02U            /*!< soft reset request */


typedef struct _USBD_PRNT_Itf
{
  int8_t (* Init)(void);
  int8_t (* DeInit)(void);
  int8_t (* Control_req)(uint8_t req, uint8_t *pbuf, uint16_t *length);
  int8_t (* Receive)(uint8_t *Buf, uint16_t *Len);

} USBD_PRNT_ItfTypeDef;

extern USBD_Class_cb_TypeDef  USBD_PRINTER_cb;

#endif /* __USB_PRINTER_H_ */
