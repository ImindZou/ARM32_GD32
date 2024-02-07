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
#include "lcd_drv.h"
#include "touch_drv.h"
#include "hmi_app.h"
#include "sensor_app.h"
#include "modbus_app.h"
#include "store_app.h"

typedef struct
{
	uint8_t run;                // ���ȱ�־��1�����ȣ�0������
	uint16_t timCount;          // ʱ��Ƭ����ֵ
	uint16_t timRload;          // ʱ��Ƭ����ֵ
	void (*pTaskFuncCb)(void);  // ����ָ���������������ҵ����ģ�麯����ַ
} TaskComps_t;

static TaskComps_t g_taskComps[] = 
{
//	{0, 5,  5,   HmiTask},
	{0, 1000, 1000,  SensorTask},
	{0, 1,    1,     ModbusTask},
	/* ���ҵ����ģ�� */
};

#define TASK_NUM_MAX   (sizeof(g_taskComps) / sizeof(g_taskComps[0]))

static void TaskHandler(void)
{
	for (uint8_t i = 0; i < TASK_NUM_MAX; i++)
	{
		if (g_taskComps[i].run)                  // �ж�ʱ��Ƭ��־
		{
			g_taskComps[i].run = 0;              // ��־����
			g_taskComps[i].pTaskFuncCb();        // ִ�е���ҵ����ģ��
		}
	}
}

/**
***********************************************************
* @brief �ڶ�ʱ���жϷ������б���ӵ��ã�����ʱ��Ƭ��ǣ�
         ��Ҫ��ʱ��1ms����1���ж�
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
	LcdDrvInit();
	TouchDrvInit();
	SystickInit();
}
static void AppInit(void)
{
	TaskScheduleCbReg(TaskScheduleCb);
	ModbusAppInit();
	InitSysParam();
}
extern const uint8_t ilearning16x16[8][32];
extern const uint8_t ilearning24x24[8][72];
int main(void)
{	
	DrvInit();
	AppInit();
	
	LcdClear(BLACK);
	
//	LcdShowChar(10, 10, 'A', ASCII_FONT_SIZE_24, RED, BLUE);
//	LcdShowString(10, 10, "I am learning ARM! I am learning ARM! I am learning ARM! I am learning ARM!", ASCII_FONT_SIZE_24, RED, BLUE);
	for (uint16_t i = 0; i < 8; i++)
	{
	LcdShowChinese(10 + CHINESE_FONT_SIZE_16 * i, 10, (uint8_t *)&ilearning16x16[i][0], CHINESE_FONT_SIZE_16, RED, BLUE);
	}
	LcdShowChinese(10, 40, (uint8_t *)&ilearning24x24[0][0], CHINESE_FONT_SIZE_24, RED, BLUE);
	while (1)
	{
		TaskHandler();

	}
}
