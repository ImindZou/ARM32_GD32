/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-14 00:34:45
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-18 20:30:47
 * @FilePath: \6.2 ???????????????????\App\main.c
 * @Description: ???????????,??????`customMade`, ??koroFileHeader?????? ????????: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
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
#include "vres_drv.h"

typedef struct
{
	uint8_t run;                // 调度标志，1：调度，0：挂起
	uint16_t timCount;          // 时间片计数值
	uint16_t timRload;          // 时间片重载值
	void (*pTaskFuncCb)(void);  // 函数指针变量，用来保存业务功能模块函数地址
} TaskComps_t;

static TaskComps_t g_taskComps[] = 
{
	{0, 5,   5,   HmiTask},
	{0, 200, 200, Usb2ComTask},
	/* 添加业务功能模块 */
};

#define TASK_NUM_MAX   (sizeof(g_taskComps) / sizeof(g_taskComps[0]))

static void TaskHandler(void)
{
	for (uint8_t i = 0; i < TASK_NUM_MAX; i++)
	{
		if (g_taskComps[i].run)                  // 判断时间片标志
		{
			g_taskComps[i].run = 0;              // 标志清零
			g_taskComps[i].pTaskFuncCb();        // 执行调度业务功能模块
		}
	}
}

/**
***********************************************************
* @brief 在定时器中断服务函数中被间接调用，设置时间片标记，
         需要定时器1ms产生1次中断
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
	VresDrvInit();
}
static void AppInit(void)
{
	Usb2ComAppInit();
	TaskScheduleCbReg(TaskScheduleCb);
}

#include <stdlib.h>

typedef struct
{
	uint16_t id;
	uint8_t score;
} StuInfo_t;


//int32_t CmpCb(const void *_a, const void *_b)
//{
//	uint16_t *a = (uint16_t *)_a;
//	uint16_t *b = (uint16_t *)_b;
//	int32_t val = 0;
//	if(*a > *b)
//	{
//		val = -1;
//	}
//	else if (*a < *b)
//	{
//		val = 1;
//	}
//	else
//	{
//		val = 0;
//	}
//	return val;
//}

int32_t StuCmpCb(const void *_a, const void *_b)
{
	StuInfo_t *a = (StuInfo_t *)_a;
	StuInfo_t *b = (StuInfo_t *)_b;
	int32_t val = 0;
	if(a->score > b->score)
	{
		val = -1;
	}
	else if (a->score < b->score)
	{
		val = 1;
	}
	else
	{
		val = 0;
	}
	return val;
}

int main(void)
{	
	DrvInit();
	uint16_t num[10] = {5,7,8,1,26,52,9,3,14,56};
	uint16_t sz = sizeof(num) / sizeof(num[0]);
//	qsort(num,sz,sizeof(num[0]),CmpCb);
//	for(uint8_t i = 0; i < 10; i++)
//	{
//		printf("num[%d] = %d.\n",i,num[i]);
//	}
	
	StuInfo_t stuInfo[5] = {
		{1,90},
		{2,65},
		{3,80},
		{4,75},
		{5,25}
	};
	qsort(stuInfo,5,sizeof(stuInfo[0]),StuCmpCb);
	for(uint8_t i = 0; i < 5; i++)
	{
		printf("student id = %d, score = %d\n",stuInfo[i].id,stuInfo[i].score);
	} 
	
	// AppInit();

	while (1)
	{
		// TaskHandler();
		VresDrvTest();
	}
}

