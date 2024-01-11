/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-09 15:55:29
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-09 16:45:13
 * @FilePath: \2.10 基于内核DWT实现硬件延时\Driver\delay.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __DELAY_H_
#define __DELAY_H
#include <stdint.h>

/**
***************************************
*@brief DWT初始化配置
*@param
*@return 
***************************************
*/
void Delay_Init(void);

/**
***************************************
*@brief 微秒级别延时函数
*@param nUs,最大延时时间(2^32 / 内核主频) * 10……6 us
*@return 
***************************************
*/
void DelayNus(uint32_t nUs);

/**
***************************************
*@brief 毫秒级延时函数
*@param nMs,延时时间n毫秒
*@return 
***************************************
*/
void DelayNms(uint32_t nMs);

#endif
