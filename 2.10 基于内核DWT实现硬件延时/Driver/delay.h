/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-09 15:55:29
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-09 16:45:13
 * @FilePath: \2.10 �����ں�DWTʵ��Ӳ����ʱ\Driver\delay.h
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __DELAY_H_
#define __DELAY_H
#include <stdint.h>

/**
***************************************
*@brief DWT��ʼ������
*@param
*@return 
***************************************
*/
void Delay_Init(void);

/**
***************************************
*@brief ΢�뼶����ʱ����
*@param nUs,�����ʱʱ��(2^32 / �ں���Ƶ) * 10����6 us
*@return 
***************************************
*/
void DelayNus(uint32_t nUs);

/**
***************************************
*@brief ���뼶��ʱ����
*@param nMs,��ʱʱ��n����
*@return 
***************************************
*/
void DelayNms(uint32_t nMs);

#endif
