/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-14 00:34:45
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-18 10:55:44
 * @FilePath: \6.2 裸机程序任务调度框架实现\App\main.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
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
}
static void AppInit(void)
{
	Usb2ComAppInit();
	TaskScheduleCbReg(TaskScheduleCb);
}

#include <time.h>
#include <stdlib.h>

//测试malloc调用堆跟static的区别
struct tm *Test(void)
{
	return (struct tm *)malloc(sizeof(struct tm));
}


int main(void)
{	
	DrvInit();
	AppInit();

	time_t timeStamp = 1000000000;
	struct  tm *timeInfo = NULL;
	// timeInfo = Test();
	// printf("address of timeinfo is 0x%p\n",timeInfo);	//malloc是分配的动态内存，需要用free()进行释放内存，否则就会内存溢出，导致内存分配的地址不一样。这会导致程序出现意想不到的错误。
	// free(Test);	//free()是一次性释放的
	// timeInfo = Test();
	
	printf("111111\n");
	// printf("address of timeinfo is 0x%p\n",timeInfo);
	timeInfo = gmtime(&timeStamp);
//	printf("gmitme, %d-%d-%d %d:%d:%d\n",timeInfo->tm_yday + 1900,timeInfo->tm_mon + 1,timeInfo->tm_mday + 1,timeInfo->tm_hour,timeInfo->tm_min,timeInfo->tm_sec);
	timeInfo = localtime(&timeStamp);
	// printf("localtime, %d-%d-%d %d:%d:%d\n",timeInfo->tm_yday + 1900,timeInfo->tm_mon + 1,timeInfo->tm_mday,timeInfo->tm_hour,timeInfo->tm_min,timeInfo->tm_sec);
//  内存不会释放的原因是localtime是用const修饰的字符串，在localtime里保存了一个static变量，放回的地址就是对应函数的static变量的地址，在函数出栈时不会像局部变量一样直接销毁。
//	这就是指针函数的用法，以后会用上
//	函数原形
	/**
	 *	struct tm *localtime(const time_t *ptr)
	 *	{
	 *		static struct tm ret;
	 *		//在这里计算的到的ret的值
	 *	return &ret;
	 *	}
	 */
	printf("address of timeinfo is 0x%p\n", timeInfo);	
	timeInfo = localtime(&timeStamp);
	printf("address of timeinfo is 0x%p\n", timeInfo);

	printf("%s\n",asctime(timeInfo));
	char timeArr[80];
	strftime(timeArr,80,"%Y-%m-%d %H:%M:%S",timeInfo);
	printf("%s\n",timeArr);
	while (1)
	{
		TaskHandler();
	}
}

