/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-14 00:34:46
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-14 00:59:30
 * @FilePath: \6.2 裸机程序任务调度框架实现\Driver\systick.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdint.h>
#include "gd32f30x.h"

static uint64_t g_sysRunTime = 0;

static void (*g_pTaskScheduleFunc)(void);	//函数指针变量，保存任务调度的函数地址

/**
***********************************************************
* @brief 注册任务调度回调函数
* @param pFunc，传入回调函数地址
* @return 
***********************************************************
*/
void TaskScheduleCbReg(void (*pFunc)(void))
{
	g_pTaskScheduleFunc = pFunc;
}

/**
***********************************************************
* @brief systick初始化
* @param
* @return 
***********************************************************
*/
void SystickInit(void)
{
	/*1ms 产生一次定时中断*/
	if (SysTick_Config(rcu_clock_freq_get(CK_AHB) / 1000))
	{
		while (1);
	}
}

/**
***********************************************************
* @brief 定时中断服务函数，1ms产生一次中断
* @param
* @return 
***********************************************************
*/
void SysTick_Handler(void)
{
	g_sysRunTime++;
	g_pTaskScheduleFunc();
}

/**
***********************************************************
* @brief 获取系统运行时间
* @param
* @return 以1ms为单位
***********************************************************
*/
uint64_t GetSysRunTime(void)
{
	return g_sysRunTime;
}
