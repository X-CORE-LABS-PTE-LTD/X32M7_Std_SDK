/**
 ******************************************************************************
 * @file      usbd_msc_bot.c
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

#include "usbd_msc_bot.h"
#include "usbd_msc_scsi.h"
#include "usbd_ioreq.h"
#include "usbd_msc_mem.h"

#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

uint16_t MSC_BOT_DataLen;
uint8_t MSC_BOT_State;
uint8_t MSC_BOT_Status;

#ifdef USB_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN uint8_t MSC_BOT_Data[MSC_MEDIA_PACKET] __ALIGN_END ;

#ifdef USB_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN MSC_BOT_CBW_TypeDef  MSC_BOT_cbw __ALIGN_END ;

#ifdef USB_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN MSC_BOT_CSW_TypeDef  MSC_BOT_csw __ALIGN_END ;

static void MSC_BOT_CBW_Decode (USB_CORE_MODULE  *pdev);

static void MSC_BOT_SendData (USB_CORE_MODULE  *pdev, uint8_t* pbuf, uint16_t len);

static void MSC_BOT_Abort(USB_CORE_MODULE  *pdev);

/**
*\*\name   MSC_BOT_Init.
*\*\fun    Initialize the BOT Process.
*\*\param  pdev : USB device
*\*\return none
**/
void MSC_BOT_Init (USB_CORE_MODULE  *pdev)
{
    MSC_BOT_State = BOT_IDLE;
    MSC_BOT_Status = BOT_STATE_NORMAL;
    USBD_STORAGE_fops->Init(0);

    USBDEV_EP_Flush(pdev, MSC_OUT_EP);
    USBDEV_EP_Flush(pdev, MSC_IN_EP);
    /* Prapare EP to Receive First BOT Cmd */
    USBDEV_EP_PrepareRx (pdev, MSC_OUT_EP, (uint8_t *)&MSC_BOT_cbw, BOT_CBW_LENGTH);
}

/**
*\*\name   MSC_BOT_Reset.
*\*\fun    Reset the BOT Machine.
*\*\param  pdev : USB device
*\*\return none
**/
void MSC_BOT_Reset (USB_CORE_MODULE  *pdev)
{
    MSC_BOT_State = BOT_IDLE;
    MSC_BOT_Status = BOT_STATE_RECOVERY;
    /* Prapare EP to Receive First BOT Cmd */
    USBDEV_EP_PrepareRx (pdev, MSC_OUT_EP, (uint8_t *)&MSC_BOT_cbw, BOT_CBW_LENGTH);
}

/**
*\*\name   MSC_BOT_DeInit.
*\*\fun    Uninitialize the BOT Machine.
*\*\param  pdev : USB device
*\*\return none
**/
void MSC_BOT_DeInit (USB_CORE_MODULE  *pdev)
{
    MSC_BOT_State = BOT_IDLE;
}

/**
*\*\name   MSC_BOT_DataIn.
*\*\fun    Handle BOT IN data stage.
*\*\param  pdev : USB device
*\*\param  epnum: endpoint index
*\*\return none
**/
void MSC_BOT_DataIn (USB_CORE_MODULE  *pdev, uint8_t epnum)
{
    switch (MSC_BOT_State)
    {
        case BOT_DATA_IN:
            if(SCSI_ProcessCmd(pdev, MSC_BOT_cbw.bLUN, &MSC_BOT_cbw.CB[0]) < 0)
            {
                MSC_BOT_SendCSW (pdev, CSW_CMD_FAILED);
            }
            break;

        case BOT_SEND_DATA:
        case BOT_LAST_DATA_IN:
            MSC_BOT_SendCSW (pdev, CSW_CMD_PASSED);
            break;

        default:
            break;
    }
}

/**
*\*\name   MSC_BOT_DataOut.
*\*\fun    Handle BOT OUT data stage.
*\*\param  pdev : USB device
*\*\param  epnum: endpoint index
*\*\return none
**/
void MSC_BOT_DataOut (USB_CORE_MODULE  *pdev, uint8_t epnum)
{
    switch (MSC_BOT_State)
    {
        case BOT_IDLE:
            MSC_BOT_CBW_Decode(pdev);
            break;

        case BOT_DATA_OUT:
            if(SCSI_ProcessCmd(pdev, MSC_BOT_cbw.bLUN, &MSC_BOT_cbw.CB[0]) < 0)
            {
                MSC_BOT_SendCSW (pdev, CSW_CMD_FAILED);
            }
            break;

        default:
            break;
    }
}

/**
*\*\name   MSC_BOT_CBW_Decode.
*\*\fun    Decode the CBW command and set the BOT state machine accordingtly.
*\*\param  pdev : USB device
*\*\return none
**/
static void  MSC_BOT_CBW_Decode (USB_CORE_MODULE  *pdev)
{

    MSC_BOT_csw.dTag = MSC_BOT_cbw.dTag;
    MSC_BOT_csw.dDataResidue = MSC_BOT_cbw.dDataLength;

    if ((USBD_GetRxCount (pdev ,MSC_OUT_EP) != BOT_CBW_LENGTH) ||
        (MSC_BOT_cbw.dSignature != BOT_CBW_SIGNATURE)||
        (MSC_BOT_cbw.bLUN > 1) || 
        (MSC_BOT_cbw.bCBLength < 1) || 
        (MSC_BOT_cbw.bCBLength > 16))
    {
        SCSI_SenseCode(MSC_BOT_cbw.bLUN, 
        ILLEGAL_REQUEST, 
        INVALID_CDB);
        MSC_BOT_Status = BOT_STATE_ERROR;   
        MSC_BOT_Abort(pdev);
    }
    else
    {
        if(SCSI_ProcessCmd(pdev, MSC_BOT_cbw.bLUN, &MSC_BOT_cbw.CB[0]) < 0)
        {
            MSC_BOT_Abort(pdev);
        }
        /*Burst xfer handled internally*/
        else if ((MSC_BOT_State != BOT_DATA_IN) && (MSC_BOT_State != BOT_DATA_OUT) && (MSC_BOT_State != BOT_LAST_DATA_IN)) 
        {
            if (MSC_BOT_DataLen > 0)
            {
                MSC_BOT_SendData(pdev, MSC_BOT_Data, MSC_BOT_DataLen);
            }
            else if (MSC_BOT_DataLen == 0) 
            {
                MSC_BOT_SendCSW (pdev, CSW_CMD_PASSED);
            }
        }
    }
}


/**
*\*\name   MSC_BOT_SendData.
*\*\fun    Send the requested data.
*\*\param  pdev : USB device
*\*\param  buf: pointer to data buffer
*\*\param  len: Data Length
*\*\return none
**/
static void  MSC_BOT_SendData(USB_CORE_MODULE  *pdev, uint8_t* buf, uint16_t len)
{
    len = MIN (MSC_BOT_cbw.dDataLength, len);
    MSC_BOT_csw.dDataResidue -= len;
    MSC_BOT_csw.bStatus = CSW_CMD_PASSED;
    MSC_BOT_State = BOT_SEND_DATA;

    USBDEV_EP_Tx(pdev, MSC_IN_EP, buf, len);
}

/**
*\*\name   MSC_BOT_SendCSW.
*\*\fun    Send the Command Status Wrapper.
*\*\param  pdev : USB device
*\*\param  CSW_Status : CSW status
*\*\return none
**/
void  MSC_BOT_SendCSW (USB_CORE_MODULE  *pdev, uint8_t CSW_Status)
{
    MSC_BOT_csw.dSignature = BOT_CSW_SIGNATURE;
    MSC_BOT_csw.bStatus = CSW_Status;
    MSC_BOT_State = BOT_IDLE;

    USBDEV_EP_Tx (pdev, MSC_IN_EP, (uint8_t *)&MSC_BOT_csw, BOT_CSW_LENGTH);

    /* Prepare EP to Receive next Cmd */
    USBDEV_EP_PrepareRx (pdev, MSC_OUT_EP, (uint8_t *)&MSC_BOT_cbw, BOT_CBW_LENGTH);  
  
}

/**
*\*\name   MSC_BOT_Abort.
*\*\fun    Abort the current transfer.
*\*\param  pdev : USB device
*\*\return none
**/
static void  MSC_BOT_Abort (USB_CORE_MODULE  *pdev)
{
    if ((MSC_BOT_cbw.bmFlags == 0) && (MSC_BOT_cbw.dDataLength != 0) && (MSC_BOT_Status == BOT_STATE_NORMAL))
    {
        USBDEV_EP_Stall(pdev, MSC_OUT_EP);
    }
    USBDEV_EP_Stall(pdev, MSC_IN_EP);

    if(MSC_BOT_Status == BOT_STATE_ERROR)
    {
        USBDEV_EP_PrepareRx (pdev, MSC_OUT_EP, (uint8_t *)&MSC_BOT_cbw, BOT_CBW_LENGTH);    
    }
}

/**
*\*\name   MSC_BOT_CplClrFeature.
*\*\fun    Complete the clear feature request.
*\*\param  pdev : USB device
*\*\param  epnum: endpoint index
*\*\return none
**/
void  MSC_BOT_CplClrFeature (USB_CORE_MODULE  *pdev, uint8_t epnum)
{
    if(MSC_BOT_Status == BOT_STATE_ERROR )/* Bad CBW Signature */
    {
        USBDEV_EP_Stall(pdev, MSC_IN_EP);
        MSC_BOT_Status = BOT_STATE_NORMAL;    
    }
    else if(((epnum & 0x80) == 0x80) && ( MSC_BOT_Status != BOT_STATE_RECOVERY))
    {
        MSC_BOT_SendCSW (pdev, CSW_CMD_FAILED);
    }
}

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
