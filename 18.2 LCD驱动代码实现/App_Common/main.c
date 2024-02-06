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

extern const unsigned char g_Image_picture2[307200];
extern const unsigned char gImage_QQ[3200];

int main(void)
{	
	DrvInit();
	AppInit();
	
	LcdClear(RED);
	
	LcdFillMultiColor(0, 0, LCD_PIXEL_WIDTH - 1, LCD_PIXEL_HEIGHT - 1, (uint16_t *)g_Image_picture2);
	
	LcdFillMultiColor(0, 0, 40 - 1, 40 - 1, (uint16_t *)gImage_QQ);

	while (1)
	{
		TaskHandler();

	}
}
