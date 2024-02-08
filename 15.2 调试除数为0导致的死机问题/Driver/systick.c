#include <stdint.h>
#include "gd32f30x.h"

static volatile uint64_t g_sysRunTime = 0;

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
