#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "rtc_drv.h"
#include "ff.h"
#include "sensor_drv.h"

#define DEV_PATH_STR				"0:"
#define SENSOR_FILE_PATH_STR		"0:�¶ȴ�������¼.txt"

static FATFS g_fs;
static FIL g_sensorFile;
static bool g_fsInitStat;

/**
* Pointer to the filesystem object to be registered (NULL:unmount)
* Logical drive number to be mounted/unmounted 
* Mount option: 0=Do not mount (delayed mount), 1=Mount immediately 
**/
void FatfsInit(void)
{
	FRESULT res;
	uint8_t workBuffer[FF_MAX_SS];	//4096
	
	res = f_mount(&g_fs, DEV_PATH_STR, 1);
	if (res == FR_NO_FILESYSTEM)
	{
		res = f_mkfs(DEV_PATH_STR, 0, workBuffer,  FF_MAX_SS);
		if (res == FR_OK)
		{
			/* ��ʽ������ȡ������ */
			f_mount(NULL,DEV_PATH_STR, 1);
			/* ���¹��� */
			res = f_mount(&g_fs, DEV_PATH_STR, 1);
		}
	}
	if (res != FR_OK)
	{
		printf("�ļ�ϵͳ��ʼ��ʧ�ܣ������룺%d.\n", res);
		g_fsInitStat = false;
		return;
	}
	g_fsInitStat = true;
}

void FatfsTask(void)
{
	if (!g_fsInitStat)
	{
		return;
	}
	FRESULT res;
	res = f_open(&g_sensorFile, SENSOR_FILE_PATH_STR, FA_OPEN_APPEND | FA_WRITE);
	if (res != FR_OK)
	{
		printf("��%sʧ�ܣ������룺%d.\n", SENSOR_FILE_PATH_STR, res);
		return;
	}
	RtcTime_t rtcTime;
	GetRtcTime(&rtcTime);
	SensorData_t sensorData;
	GetSensorData(&sensorData); 
	if (f_printf(&g_sensorFile, "%d-%02d-%-2d %-2d:%02d:%02d, temp is %.1f, humi is %d\n",
			rtcTime.year, rtcTime.month, rtcTime.day, rtcTime.hour, rtcTime.minute, rtcTime.second,
			sensorData.temp, sensorData.humi) < 0)
	{
		printf("д������ʧ��\n");
	}
	f_close(&g_sensorFile);
}
 
void PrintSensorFile(void)
{
	if (!g_fsInitStat)
	{
		return;
	}
	
	FRESULT res;
	res = f_open(&g_sensorFile, SENSOR_FILE_PATH_STR, FA_OPEN_EXISTING | FA_READ);
	if (res != FR_OK)
	{
		printf("��%sʧ�ܣ������룺%d.\n", SENSOR_FILE_PATH_STR, res);
		return;
	}
	
	uint8_t readBuf[50];
	memset(readBuf, 0, sizeof(readBuf));
	while (NULL != f_gets((TCHAR *)readBuf, sizeof(readBuf), &g_sensorFile))
	{
		printf("%s",readBuf);
		memset(readBuf, 0, sizeof(readBuf));
	}
	f_unlink(SENSOR_FILE_PATH_STR);
}

