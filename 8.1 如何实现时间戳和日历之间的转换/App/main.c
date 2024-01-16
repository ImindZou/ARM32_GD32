/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-14 00:34:45
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-15 23:38:40
 * @FilePath: \6.2 �������������ȿ��ʵ��\App\main.c
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdint.h>
#include <stdio.h>
#include "led_drv.h"
#include "key_drv.h"
#include "systick.h"
#include "usb2com_drv.h"
#include "delay.h"
#include "usb2com_app.h"
#include "hmi_app.h"

typedef struct
{
	uint8_t run;                // ���ȱ�־��1�����ȣ�0������
	uint16_t timCount;          // ʱ��Ƭ����ֵ
	uint16_t timRload;          // ʱ��Ƭ����ֵ
	void (*pTaskFuncCb)(void);  // ����ָ���������������ҵ����ģ�麯����ַ
} TaskComps_t;

static TaskComps_t g_taskComps[] = 
{
	{0, 5,   5,   HmiTask},
	{0, 200, 200, Usb2ComTask},
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
	SystickInit();
	LedDrvInit();
	KeyDrvInit();
	DelayInit();
	Usb2ComDrvInit();
}
static void AppInit(void)
{
	Usb2ComAppInit();
	TaskScheduleCbReg(TaskScheduleCb);
}

#include <time.h>

int main(void)
{	
	DrvInit();
	AppInit();

	time_t timeStamp = 1000000000;
	struct  tm *timeInfo = NULL;
//	timeInfo = gmtime(&timeStamp);
////	printf("gmitme, %d-%d-%d %d:%d:%d\n",timeInfo->tm_yday + 1900,timeInfo->tm_mon + 1,timeInfo->tm_mday + 1,timeInfo->tm_hour,timeInfo->tm_min,timeInfo->tm_sec);
	timeInfo = localtime(&timeStamp);
	printf("localtime, %d-%d-%d %d:%d:%d\n",timeInfo->tm_yday + 1900,timeInfo->tm_mon + 1,
	timeInfo->tm_mday,timeInfo->tm_hour,timeInfo->tm_min,timeInfo->tm_sec);
	
	printf("%s\n",asctime(timeInfo));
	char timeArr[80];
	strftime(timeArr,80,"%Y-%m-%d %H:%M:%S",timeInfo);
	printf("%s\n",timeArr);
	while (1)
	{
		TaskHandler();
	}
}

