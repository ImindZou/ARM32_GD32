/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-14 00:34:46
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-14 01:00:15
 * @FilePath: \6.2 裸机程序任务调度框架实现\Driver\systick.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef _SYSTICK_H_
#define _SYSTICK_H_

#include <stdint.h>
/**
***********************************************************
* @brief systick Init
* @param
* @return 
***********************************************************
*/
void SystickInit(void);

/**
***********************************************************
* @brief acquire the system runing time 
* @param
* @return in units of 1ms
***********************************************************
*/
uint64_t GetSysRunTime(void);

/**
***********************************************************
* @brief register the callback function
* @param pFunc，introduce callback func address
* @return 
***********************************************************
*/
void TaskScheduleCbReg(void (*pFunc)(void));

#endif
