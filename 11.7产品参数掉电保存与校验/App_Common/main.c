/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-02-02 11:36:54
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-02-02 12:50:11
 * @FilePath: \11.7²úÆ·²ÎÊýµôµç±£´æÓëÐ£Ñé\App_Common\main.c
 * @Description: ÕâÊÇÄ¬ÈÏÉèÖÃ,ÇëÉèÖÃ`customMade`, ´ò¿ªkoroFileHeader²é¿´ÅäÖÃ ½øÐÐÉèÖÃ: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdint.h>
#include <stdio.h>
#include "led_drv.h"
#include "key_drv.h"
#include "systick.h"
#include "usb2com_drv.h"
#include "rtc_drv.h"
#include "delay.h"
#include "sensor_drv.h"
#include "eeprom_drv.h"
#include "hmi_app.h"
#include "sensor_app.h"
#include "modbus_app.h"
#include "store_app.h"

typedef struct
{
	uint8_t run;                // ï¿½ï¿½ï¿½È±ï¿½Ö¾ï¿½ï¿½1ï¿½ï¿½ï¿½ï¿½ï¿½È£ï¿½0ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	uint16_t timCount;          // Ê±ï¿½ï¿½Æ¬ï¿½ï¿½ï¿½ï¿½Öµ
	uint16_t timRload;          // Ê±ï¿½ï¿½Æ¬ï¿½ï¿½ï¿½ï¿½Öµ
	void (*pTaskFuncCb)(void);  // ï¿½ï¿½ï¿½ï¿½Ö¸ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Òµï¿½ï¿½ï¿½ï¿½Ä£ï¿½éº?ï¿½ï¿½ï¿½ï¿½Ö·
} TaskComps_t;

static TaskComps_t g_taskComps[] = 
{
	{0, 5,  5,   HmiTask},
	{0, 1000, 1000,  SensorTask},
	{0, 1,    1,     ModbusTask},
	/* ï¿½ï¿½ï¿½ï¿½Òµï¿½ï¿½ï¿½ï¿½Ä£ï¿½ï¿½ */
};

#define TASK_NUM_MAX   (sizeof(g_taskComps) / sizeof(g_taskComps[0]))

static void TaskHandler(void)
{
	for (uint8_t i = 0; i < TASK_NUM_MAX; i++)
	{
		if (g_taskComps[i].run)                  // ï¿½Ð¶ï¿½Ê±ï¿½ï¿½Æ¬ï¿½ï¿½Ö¾
		{
			g_taskComps[i].run = 0;              // ï¿½ï¿½Ö¾ï¿½ï¿½ï¿½ï¿½
			g_taskComps[i].pTaskFuncCb();        // Ö´ï¿½Ðµï¿½ï¿½ï¿½Òµï¿½ï¿½ï¿½ï¿½Ä£ï¿½ï¿½
		}
	}
}

/**
***********************************************************
* @brief ï¿½Ú¶ï¿½Ê±ï¿½ï¿½ï¿½Ð¶Ï·ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ð±ï¿½ï¿½ï¿½Óµï¿½ï¿½Ã£ï¿½ï¿½ï¿½ï¿½ï¿½Ê±ï¿½ï¿½Æ?ï¿½ï¿½Ç£ï¿?
         ï¿½ï¿½Òªï¿½ï¿½Ê±ï¿½ï¿½1msï¿½ï¿½ï¿½ï¿½1ï¿½ï¿½ï¿½Ð¶ï¿½
* @param
* @return 
***********************************************************
*/
static void TaskScheduleCb(void)
{
	for (uint8_t i = 0; i < TASK_NUM_MAX; i++)
	{
		if (g_taskComps[i].timCount)
		{
			g_taskComps[i].timCount--;
			if (g_taskComps[i].timCount == 0)
			{
				g_taskComps[i].run = 1;
				g_taskComps[i].timCount = g_taskComps[i].timRload;
			}
		}
	}
}

static void DrvInit(void)
{
	DelayInit();
	LedDrvInit();
	KeyDrvInit();
	Usb2ComDrvInit();
	RtcDrvInit();
	SensorDrvInit();
	EepromDrvInit();
	SystickInit();
}
static void AppInit(void)
{
	TaskScheduleCbReg(TaskScheduleCb);
	ModbusAppInit();
	InitSysParam();
}


int main(void)
{	
	DrvInit();
	AppInit();

	while (1)
	{
		TaskHandler();

	}
}
