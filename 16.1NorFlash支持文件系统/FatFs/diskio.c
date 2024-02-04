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
#include "norflash_drv.h"

/* Definitions of physical drive number for each drive */
#define DEV_NORFLASH		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */


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
	uint8_t mid = 0;
	uint16_t did = 0;

	switch (pdrv) {
	case DEV_NORFLASH :
		NorflashDrvInit();
		ReadNorflashID(&mid, &did);
		if (((uint32_t)mid << 16 | did) == NORFLASH_ID)
		{
			return RES_OK;
		}
	}
	return STA_NOINIT;
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

	switch (pdrv) {
	case DEV_NORFLASH :
		ReadNorflashData(sector * NORFLASH_SECTOR_SIZE, buff, count * NORFLASH_SECTOR_SIZE);
		return RES_OK;
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
	switch (pdrv) {
	case DEV_NORFLASH :
		EraseNorflashForWrite(sector * NORFLASH_SECTOR_SIZE, count * NORFLASH_SECTOR_SIZE);
		WriteNorflashData(sector * NORFLASH_SECTOR_SIZE, (uint8_t *)buff, count * NORFLASH_SECTOR_SIZE);
		return RES_OK;
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
		case DEV_NORFLASH :
			switch (cmd)
			{
				case GET_SECTOR_COUNT :
					*(DWORD *)buff = NORFLASH_SECTOR_COUNT;
					break;
				case GET_SECTOR_SIZE :
					*(WORD *)buff = NORFLASH_SECTOR_SIZE;
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
