/**
 ******************************************************************************
 * @file      usbd_printer.c
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

#include "usbd_printer.h"
#include "usbd_desc.h"
#include "usbd_req.h"
#include "usbhs_bsp.h"

/* Printer Device library callbacks */
static uint8_t USBD_PRNTER_Init(void *pdev, uint8_t cfgidx);
static uint8_t USBD_PRNTER_DeInit(void *pdev, uint8_t cfgidx);
static uint8_t USBD_PRNTER_Setup(void *pdev, USB_SETUP_REQ *req);
static uint8_t USBD_PRNTER_DataIn(void *pdev, uint8_t epnum);
static uint8_t USBD_PRNTER_DataOut(void *pdev, uint8_t epnum);

/* Printer specific management functions */
static uint8_t *USBD_PRNTER_GetCfgDesc(uint8_t speed, uint16_t *length);
static uint8_t *USBD_PRNTER_GetOtherCfgDesc(uint8_t speed, uint16_t *length);

extern USBD_PRNT_ItfTypeDef  APP_FOPS;
static uint8_t PRNTInEpAdd  = PRINTER_IN_EP;
static uint8_t PRNTOutEpAdd = PRINTER_OUT_EP;

/* Printer interface class callbacks structure */
USBD_Class_cb_TypeDef  USBD_PRINTER_cb = 
{
    USBD_PRNTER_Init,
    USBD_PRNTER_DeInit,
    USBD_PRNTER_Setup,
    NULL,                 
    NULL,
    USBD_PRNTER_DataIn,
    USBD_PRNTER_DataOut,
    NULL,
    NULL,
    NULL,     
    USBD_PRNTER_GetCfgDesc,
    USBD_PRNTER_GetOtherCfgDesc, 
};

#ifdef USB_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN uint8_t USBD_PRNT_CfgDesc  [USB_PRNT_CONFIG_DESC_SIZ] __ALIGN_END ;

#ifdef USB_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN static __IO uint32_t  USBD_PRINTER_AltSet  __ALIGN_END = 0;


#ifdef USB_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN uint8_t USB_Rx_Buffer   [PRINTER_DATA_MAX_PACKET_SIZE] __ALIGN_END ;


#ifdef USB_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN uint8_t CmdBuff[PRINTER_CMD_PACKET_SIZE] __ALIGN_END ;


#ifdef USB_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_INTERNAL_DMA_ENABLED */
/* USB PRNT device Configuration Descriptor */
__ALIGN_BEGIN uint8_t USBD_PRNT_CfgDesc[] __ALIGN_END =
{
	/*Configuration Descriptor*/
	0x09,                                /* bLength: Configuration Descriptor size */
	USB_DESC_TYPE_CONFIGURATION,         /* bDescriptorType: Configuration */
	USB_PRNT_CONFIG_DESC_SIZ,            /* wTotalLength:no of returned bytes */
	0x00,
	0x01,                                /* bNumInterfaces: 1 interface */
	0x01,                                /* bConfigurationValue: Configuration value */
	0x00,                                /* iConfiguration: Index of string descriptor describing the configuration */
	0xA0,                                /* bmAttributes: Self Powered according to user configuration */
	0x32,                                /* MaxPower 100 mA */
	
	/*Interface Descriptor */
	0x09,                                /* bLength: Interface Descriptor size */
	USB_DESC_TYPE_INTERFACE,             /* bDescriptorType: Interface */
	0x00,                                /* bInterfaceNumber: Number of Interface */
	0x00,                                /* bAlternateSetting: Alternate setting */
	0x02,                                /* bNumEndpoints: 2 endpoints used */
	0x07,                                /* bInterfaceClass: Communication Interface Class */
	0x01,                                /* bInterfaceSubClass: Abstract Control Model */
	USB_PRNT_BIDIRECTIONAL,              /* bDeviceProtocol */
	0x00,                                /* iInterface */
	
	/*Endpoint IN Descriptor*/
	0x07,                                /* bLength: Endpoint Descriptor size */
	USB_DESC_TYPE_ENDPOINT,              /* bDescriptorType: Endpoint */
	PRINTER_IN_EP,                       /* bEndpointAddress */
	0x02,                                /* bmAttributes: Bulk */
	LOBYTE(PRINTER_DATA_MAX_PACKET_SIZE), /* wMaxPacketSize */
	HIBYTE(PRINTER_DATA_MAX_PACKET_SIZE),
	0x00,                                /* bInterval */
	
	/*Endpoint OUT Descriptor*/
	0x07,                                /* bLength: Endpoint Descriptor size */
	USB_DESC_TYPE_ENDPOINT,              /* bDescriptorType: Endpoint */
	PRINTER_OUT_EP,                      /* bEndpointAddress */
	0x02,                                /* bmAttributes: Bulk */
	LOBYTE(PRINTER_DATA_MAX_PACKET_SIZE),/* wMaxPacketSize */
	HIBYTE(PRINTER_DATA_MAX_PACKET_SIZE),
	0x00                                 /* bInterval */
};


/**
*\*\name    USBD_PRNTER_Init.
*\*\fun     Initialize the printer interface.
*\*\param   pdev: device instance.
*\*\param   cfgidx: Configuration index
*\*\return  USBD_OK.
*\*\
**/
uint8_t USBD_PRNTER_Init (void  *pdev, uint8_t cfgidx)
{
	/* Open EP IN */
	USBDEV_EP_Open(pdev, PRNTInEpAdd, PRINTER_DATA_IN_PACKET_SIZE, USB_EP_BULK);
	
	/* Open EP OUT */
    USBDEV_EP_Open(pdev, PRNTOutEpAdd, PRINTER_DATA_OUT_PACKET_SIZE, USB_EP_BULK);
	
	/* Initialize the Interface physical components */
    APP_FOPS.Init();
	
	/* Prepare Out endpoint to receive next packet */
    USBDEV_EP_PrepareRx(pdev, PRNTOutEpAdd, (uint8_t*)(USB_Rx_Buffer), PRINTER_DATA_OUT_PACKET_SIZE);
	
	return USBD_OK;
}


/**
*\*\name    USBD_PRNTER_DeInit.
*\*\fun     DeInitialize the printer layer.
*\*\param   pdev: device instance.
*\*\param   cfgidx: Configuration index
*\*\return  USBD_OK.
*\*\
**/
uint8_t  USBD_PRNTER_DeInit (void  *pdev, uint8_t cfgidx)
{
	/* Open EP IN */
    USBDEV_EP_Close(pdev, PRNTInEpAdd);

    /* Open EP OUT */
    USBDEV_EP_Close(pdev, PRNTOutEpAdd);
	
	/* Restore default state of the Interface physical components */
    APP_FOPS.DeInit();
	
	return USBD_OK;
}

/**
*\*\name    USBD_PRNTER_Setup
*\*\fun     Handle the printer specific requests.
*\*\param   pdev: device instance.
*\*\param   req: usb requests
*\*\return  USBD_OK.
**/
uint8_t USBD_PRNTER_Setup (void  *pdev, USB_SETUP_REQ *req)
{
	USBD_Status ret = USBD_OK;
	uint16_t status_info = 0U;
	uint16_t data_length;
	
	switch (req->bmRequest & USB_REQ_TYPE_MASK)
	{
		/* PRNTER Class Requests -------------------------------*/
		case USB_REQ_TYPE_CLASS :
		{
			/* Check if the request is a data setup packet */
			if (req->wLength != 0U)
			{
				data_length = MIN(req->wLength, PRINTER_DATA_MAX_PACKET_SIZE);
				
				if ((req->bmRequest & 0x80U) != 0U)
				{
					/* Call the User class interface function to process the command */
					APP_FOPS.Control_req(req->bRequest, CmdBuff, &data_length);
					
					/* Return the answer to host */
					USBD_CtrlSendData(pdev, (uint8_t *)CmdBuff, data_length);
				}
				else /* Host-to-Device request */
				{
					/* Prepare for control data reception */
					USBD_CtrlPrepareRx(pdev, (uint8_t *)CmdBuff, data_length);
				}
			}
			else /* No Data request */
			{
				data_length = 0U;
				APP_FOPS.Control_req(req->bRequest, (uint8_t *)req, &data_length);
			}
			
			break;
		}
		
		/* Standard Requests -------------------------------*/
		case USB_REQ_TYPE_STANDARD :
		{
			switch (req->bRequest)
			{
				case USB_REQ_GET_STATUS :
				{
					if (((USB_CORE_MODULE *)pdev)->dev.device_status == USB_CONFIGURED)
					{
						USBD_CtrlSendData(pdev, (uint8_t *)&status_info, 2U);
					}
					else
					{
						USBD_CtrlError (pdev, req);
						ret = USBD_FAIL;
					}
					
					break;
				}
				
				case USB_REQ_GET_INTERFACE :
				{
					if (((USB_CORE_MODULE *)pdev)->dev.device_status == USB_CONFIGURED)
					{
						USBD_CtrlSendData(pdev, (uint8_t *)&USBD_PRINTER_AltSet, 1U);
					}
					else
					{
						USBD_CtrlError (pdev, req);
						ret = USBD_FAIL;
					}
					
					break;
				}
				
				case USB_REQ_SET_INTERFACE :
				{
					if (((USB_CORE_MODULE *)pdev)->dev.device_status != USB_CONFIGURED)
					{
						USBD_CtrlError (pdev, req);
						ret = USBD_FAIL;
					}
					
					break;
				}
				
				case USB_REQ_CLEAR_FEATURE :
					break;
				
				default :
					USBD_CtrlError(pdev, req);
					ret = USBD_FAIL;
			}
			
			break;
		}
		
		default :
			USBD_CtrlError (pdev, req);
            ret = USBD_FAIL;
	}
	
	return (uint8_t)ret;
}

/**
*\*\name    USBD_PRNTER_DataIn.
*\*\fun     handle data IN Stage.
*\*\param   pdev: device instance.
*\*\param   epnum: endpoint index.
*\*\return  USBD_OK.
*\*\
**/
uint8_t  USBD_PRNTER_DataIn (void *pdev, uint8_t epnum)
{
	if ((((USB_CORE_MODULE *)pdev)->dev.in_ep[epnum].total_data_len  > 0U) &&
		(((USB_CORE_MODULE *)pdev)->dev.in_ep[epnum].total_data_len % ((USB_CORE_MODULE *)pdev)->dev.in_ep[epnum].maxpacket == 0U))
	{
		/* Update the packet total length */
		((USB_CORE_MODULE *)pdev)->dev.in_ep[epnum].total_data_len = 0U;
		
		USBDEV_EP_Tx(pdev, epnum, NULL, 0U);
	}
	
	return USBD_OK;
}


/**
*\*\name    USBD_PRNTER_DataOut.
*\*\fun     handle data OUT Stage.
*\*\param   pdev: device instance.
*\*\param   epnum: endpoint index.
*\*\return  USBD_OK.
*\*\
**/
uint8_t  USBD_PRNTER_DataOut (void *pdev, uint8_t epnum)
{
	uint16_t recv_length;
	
	/* Get the received data length */
	recv_length = USBD_GetRxCount(pdev, epnum);
	
	APP_FOPS.Receive(USB_Rx_Buffer, &recv_length);
	
	return USBD_OK;
}

/**
*\*\name    USBD_PRNTER_GetCfgDesc.
*\*\fun     Return configuration descriptor.
*\*\param   speed : current device speed
*\*\param   length : pointer data length
*\*\return  pointer to descriptor buffer.
*\*\
**/
static uint8_t *USBD_PRNTER_GetCfgDesc (uint8_t speed, uint16_t *length)
{
    *length = sizeof (USBD_PRNT_CfgDesc);
    return USBD_PRNT_CfgDesc;
}

/**
*\*\name    USBD_PRNTER_GetOtherCfgDesc.
*\*\fun     Return configuration descriptor.
*\*\param   speed : current device speed
*\*\param   length : pointer data length
*\*\return  pointer to descriptor buffer.
*\*\
**/
static uint8_t *USBD_PRNTER_GetOtherCfgDesc (uint8_t speed, uint16_t *length)
{
	*length = sizeof (USBD_PRNT_CfgDesc);
	return USBD_PRNT_CfgDesc;
}
