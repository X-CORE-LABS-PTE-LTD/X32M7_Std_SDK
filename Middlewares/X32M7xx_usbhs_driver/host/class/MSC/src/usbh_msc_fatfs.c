/**
 ******************************************************************************
 * @file      usbh_msc_fatfs.c
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

#include "usbhs_conf.h"
#include "usbh_core.h"
#include "diskio.h"
#include "usbh_msc_scsi.h"
#include "usbh_msc_bot.h"

DWORD get_fattime(void);

static volatile DSTATUS Stat = STA_NOINIT;	/* Disk status */

extern USB_CORE_MODULE USBHS_Core;
extern USBH_HOST USB_Host;

/**
*\*\name    disk_initialize.
*\*\fun     Initialize Disk Drive.
*\*\param   drv: Physical drive number.
*\*\return  Stat.
*\*\
**/
DSTATUS disk_initialize(BYTE drv)/* Physical drive number (0) */
{
    if(HOST_IsDeviceConnected(&USBHS_Core))
    {  
        Stat &= ~STA_NOINIT;
    }

    return Stat;
}

/**
*\*\name    disk_status.
*\*\fun     Get Disk Status.
*\*\param   drv: Physical drive number.
*\*\return  Stat.
*\*\
**/
DSTATUS disk_status(BYTE drv) /* Physical drive number (0) */
{
    if (drv) return STA_NOINIT;   /* Supports only single drive */
    return Stat;
}

/**
*\*\name    disk_read.
*\*\fun     Read Sector(s).
*\*\param   pdrv: Physical drive number.
*\*\param   buff: Pointer to the data buffer to store read data.
*\*\param   sector: Start sector number.
*\*\param   count: Sector count.
*\*\return  Stat.
*\*\
**/
DRESULT disk_read (BYTE pdrv, BYTE *buff, DWORD sector, UINT count)
{
    BYTE status = USBH_MSC_OK;

    if (pdrv || !count) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;


    if(HOST_IsDeviceConnected(&USBHS_Core))
    {
        do
        {
            status = USBH_MSC_Read10(&USBHS_Core, buff,sector,512 * count);
            USBH_MSC_HandleBOTXfer(&USBHS_Core ,&USB_Host);
          
            if(!HOST_IsDeviceConnected(&USBHS_Core))
            { 
                return RES_ERROR;
            }
        }
        while(status == USBH_MSC_BUSY );
    }
    if(status == USBH_MSC_OK)
    return RES_OK;
    return RES_ERROR;
}

#if _READONLY == 0

/**
*\*\name    disk_write.
*\*\fun     Write Sector(s).
*\*\param   pdrv: Physical drive number.
*\*\param   buff: Pointer to the data buffer to be written.
*\*\param   sector: Start sector number.
*\*\param   count: Sector count.
*\*\return  Stat.
*\*\
**/
DRESULT disk_write (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count)
{
    BYTE status = USBH_MSC_OK;
    if (pdrv || !count) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;
    if (Stat & STA_PROTECT) return RES_WRPRT;


    if(HOST_IsDeviceConnected(&USBHS_Core))
    {  
        do
        {
            status = USBH_MSC_Write10(&USBHS_Core,(BYTE*)buff,sector,512 * count);
            USBH_MSC_HandleBOTXfer(&USBHS_Core, &USB_Host);

            if(!HOST_IsDeviceConnected(&USBHS_Core))
            { 
                return RES_ERROR;
            }
        }
        while(status == USBH_MSC_BUSY );
    }

    if(status == USBH_MSC_OK)
    return RES_OK;
    return RES_ERROR;
}
#endif /* _READONLY == 0 */

#if _USE_IOCTL != 0
/**
*\*\name    disk_ioctl.
*\*\fun     Miscellaneous Functions.
*\*\param   pdrv: Physical drive number.
*\*\param   ctrl: Control code.
*\*\param   buff: Buffer to send/receive control data.
*\*\return  res.
*\*\
**/
DRESULT disk_ioctl (BYTE drv, BYTE ctrl, void *buff)
{
    DRESULT res = RES_OK;

    if (drv) return RES_PARERR;

    res = RES_ERROR;

    if (Stat & STA_NOINIT) return RES_NOTRDY;

    switch (ctrl) 
    {
        case CTRL_SYNC :        /* Make sure that no pending write process */
            res = RES_OK;
            break;

        case GET_SECTOR_COUNT : /* Get number of sectors on the disk (DWORD) */

            *(DWORD*)buff = (DWORD) USBH_MSC_Param.MSCapacity;
            res = RES_OK;
            break;

        case GET_SECTOR_SIZE :  /* Get R/W sector size (WORD) */
            *(WORD*)buff = 512;
            res = RES_OK;
            break;

        case GET_BLOCK_SIZE :   /* Get erase block size in unit of sector (DWORD) */
            *(DWORD*)buff = 512;
            break;

        default:
        res = RES_PARERR;
    }
    return res;
}

/**
*\*\name    get_fattime.
*\*\fun     Gets Time from RTC .
*\*\param   none.
*\*\return  Time in DWORD.
*\*\
**/
DWORD get_fattime(void)
{
    return 0;
}

#endif /* _USE_IOCTL != 0 */
