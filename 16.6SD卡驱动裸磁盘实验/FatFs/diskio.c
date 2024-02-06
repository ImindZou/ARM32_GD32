/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include <stdint.h>
#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "rtc_drv.h"
#include "sdcard.h"

/* Definitions of physical drive number for each drive */
#define DEV_SD		0	/* Example: Map Ramdisk to physical drive 0 */

static sd_card_info_struct g_SdInfo;

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
//	uint8_t mid = 0;
//	uint16_t did = 0;

	switch (pdrv) {
	case DEV_SD :
		if (SD_OK != SdcardDrvInit())
		{
			return RES_OK;
		}
		sd_card_information_get(&g_SdInfo);
		return RES_OK;
	}
	return RES_NOTRDY;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	sd_error_enum status = SD_ERROR;
	switch (pdrv) {
	case DEV_SD :
		if (1 == count)
		{
			/* single sector read */
			status = sd_block_read((uint32_t *)buff, (uint32_t)(sector << 9), g_SdInfo.card_blocksize);
		}
		else
		{
			/* multiple sectors read */
			status = sd_multiblocks_read((uint32_t *)buff, (uint32_t)(sector << 9), g_SdInfo.card_blocksize, (uint32_t)count);
		}
		if(SD_OK == status)
		{
			return RES_OK;
		}
	}
	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	sd_error_enum status = SD_ERROR;
	switch (pdrv) {
	case DEV_SD :
		if (1 == count)
		{
			/* single sector write */
			status = sd_block_write((uint32_t *)buff, sector << 9, g_SdInfo.card_blocksize);
		}
		else
		{
			/* muliple sectors write */
			status = sd_multiblocks_write((uint32_t *)buff, sector << 9, g_SdInfo.card_blocksize, (uint32_t)count);		}
		if (SD_OK == status)
		{
			return RES_OK;
		}
	}
	
	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	switch (pdrv) {
		case DEV_SD :
			switch (cmd)
			{
				case GET_SECTOR_COUNT :
					*(DWORD *)buff = (uint32_t)(g_SdInfo.card_capacity / g_SdInfo.card_blocksize);
					break;
				case GET_SECTOR_SIZE :
					*(WORD *)buff = g_SdInfo.card_blocksize;
					break;
			}
			return RES_OK;
	}
	return RES_PARERR;
}
/*
User defined function to qive a current time to fatfs module
31-25:rear(0-127 org.1980),24-21:Month(1-12),20-16:Day(1-31)
15-11:Hour(0-23)£¬10-5:Minute(0-59)£¬4-0:second(0-29 *2)
*/
DWORD get_fattime (void)
{
//	time_t t;   ¹Ù·½Ô´Âë
//    struct tm *stm;


//    t = time(0);
//    stm = localtime(&t);

//    return (DWORD)(stm->tm_year - 80) << 25 |
//           (DWORD)(stm->tm_mon + 1) << 21 |
//           (DWORD)stm->tm_mday << 16 |
//           (DWORD)stm->tm_hour << 11 |
//           (DWORD)stm->tm_min << 5 |
//           (DWORD)stm->tm_sec >> 1;
	RtcTime_t  rtcTime;
	GetRtcTime(&rtcTime);
	return ((DWORD)(rtcTime.year - 1980) << 25)
		  |	((DWORD)(rtcTime.month) << 21)
		  | ((DWORD)(rtcTime.day) << 16)
		  | ((DWORD)(rtcTime.hour) << 11)
		  | ((DWORD)(rtcTime.minute) << 5)
		  | ((DWORD)(rtcTime.second) >> 5);
}
