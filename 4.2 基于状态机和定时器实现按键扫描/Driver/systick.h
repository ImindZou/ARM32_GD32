/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-09 22:44:35
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-09 23:03:21
 * @FilePath: \4.2 基于状态机和定时器实现按键扫描\Driver\systick.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __SYSTICK_H_
#define __SYSTICK_H_
#include <stdint.h>
#include "gd32f30x.h"

/**
***************************************
*@brief systick初始化
*@param 
*@return 
***************************************
*/
void SystickInit(void);

/**
***************************************
*@brief systick中断函数，1ms 产生1次
*@param 
*@return 
***************************************
*/
void SysTick_Handler(void);

/**
***************************************
*@brief 获取系统运行时间
*@param 
*@return 以1ms为单位
***************************************
*/
uint64_t GetSysRunTime(void);
#endif
