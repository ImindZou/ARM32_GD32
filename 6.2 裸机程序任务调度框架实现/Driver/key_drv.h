/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-09 19:42:30
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-10 00:20:58
 * @FilePath: \2.11 ��ˮ�Ƶ�����ܹ��ʹ���ʵ��d:\Study\ARM32��Ƭ��ʵս�γ�\06�Լ�����\3.5 �ж���ռ���ȼ���ʲô����\Driver\key_drv.h
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __KEY_DRV_H_
#define __KEY_DRV_H_
#include <stdint.h>
#include "led_drv.h"

#define KEY1_SHORT_PRESS  0x01
#define KEY1_LONG_PRESS   0x81
#define KEY2_SHORT_PRESS  0x02
#define KEY2_LONG_PRESS   0x82
#define KEY3_SHORT_PRESS  0x03
#define KEY3_LONG_PRESS   0x83
#define KEY4_SHORT_PRESS  0x04
#define KEY4_LONG_PRESS   0x84

/**
***************************************
*@brief ����Ӳ����ʼ��
*@param 
*@return 
***************************************
*/
//void KeyDrvInit(void);
void KeyDrvInit(void);

/**
***************************************
*@brief ��ȡ������ֵ
*@param 
*@return �ĸ�������ֵ���̰�0x01 0x02 0x03 0x04������0x81 0x82 0x83 0x84
***************************************
*/
uint8_t GetKeyVal(void);

/*����״̬��*/
static uint8_t KeyScan(uint8_t KeyIndex);
#endif
