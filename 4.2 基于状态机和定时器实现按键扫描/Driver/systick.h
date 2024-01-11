/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-09 22:44:35
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-09 23:03:21
 * @FilePath: \4.2 ����״̬���Ͷ�ʱ��ʵ�ְ���ɨ��\Driver\systick.h
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __SYSTICK_H_
#define __SYSTICK_H_
#include <stdint.h>
#include "gd32f30x.h"

/**
***************************************
*@brief systick��ʼ��
*@param 
*@return 
***************************************
*/
void SystickInit(void);

/**
***************************************
*@brief systick�жϺ�����1ms ����1��
*@param 
*@return 
***************************************
*/
void SysTick_Handler(void);

/**
***************************************
*@brief ��ȡϵͳ����ʱ��
*@param 
*@return ��1msΪ��λ
***************************************
*/
uint64_t GetSysRunTime(void);
#endif
