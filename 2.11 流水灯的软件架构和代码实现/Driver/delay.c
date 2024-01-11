/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-09 15:55:29
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-09 16:25:17
 * @FilePath: \2.10 基于内核DWT实现硬件延时\Driver\delay.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdint.h>
#include "gd32f30x.h"
/**
***************************************
*@brief DWT初始化配置
*@param
*@return 
***************************************
*/
void Delay_Init(void) 
{
    /*关闭TRC*/
    CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk;
    /*打开TRC*/
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    /*关闭计数功能*/
    DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk;
    /*打开计数功能*/
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    /*计数清零*/
    DWT -> CYCCNT = 0;
}

/**
***************************************
*@brief 微秒级别延时函数
*@param nUs,最大延时时间(2^32 / 内核主频) * 10……6 us
*@return 
***************************************
*/
void DelayNus(uint32_t nUs)
{
    uint32_t tickstar = DWT->CYCCNT;

    /*转换为nUs对应的时钟跳动次数*/
    nUs *= (rcu_clock_freq_get(CK_AHB) / 1000000);

    /*延时等待*/
    while((DWT->CYCCNT - tickstar) < nUs);
}

/**
***************************************
*@brief 毫秒级延时函数
*@param nMs,延时时间n毫秒
*@return 
***************************************
*/
void DelayNms(uint32_t nMs)
{
    for(uint32_t i = 0; i < nMs; i++)
    {
        DelayNus(1000);
    }
}
