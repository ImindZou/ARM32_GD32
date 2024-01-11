/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-10 09:43:06
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-10 09:51:15
 * @FilePath: \ARM32单片机实战课程\06自己动手\4.2 基于状态机和定时器实现按键扫描\Driver\systick.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#include <stdint.h>
#include "gd32f30x.h"

static uint64_t g_sysRunTime = 0;

/**
***************************************
*@brief systick???
*@param 
*@return 
***************************************
*/
void SystickInit(void)
{
    /*1ms ??????*/
    if(SysTick_Config(rcu_clock_freq_get(CK_AHB) / 1000))
    {
        while (1);
    }
}

/**
***************************************
*@brief systick?????1ms ??1?
*@param 
*@return 
***************************************
*/
void SysTick_Handler(void)
{
    g_sysRunTime++;
}

/**
***************************************
*@brief ????????
*@param 
*@return ?1ms???
***************************************
*/
uint64_t GetSysRunTime(void)
{
    return g_sysRunTime;
}
