/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-18 14:45:37
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-18 15:13:10
 * @FilePath: \8.4 RTC���������ʵ��d:\Study\ARM32��Ƭ��ʵս�γ�\06�Լ�����\8.5 �������Ź�\App\hmi_app.c
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdint.h>
#include <stdio.h>
#include "led_drv.h"
#include "key_drv.h"

/**
***********************************************************
* @brief �˻�������������
* @param 
* @return 
***********************************************************
*/
void HmiTask(void)
{
	uint8_t keyVal;
	keyVal = GetKeyVal();
	switch (keyVal)
		{
		case KEY1_SHORT_PRESS:
			TurnOnLed(LED1);
			printf("************������ѭ��**************\n");
			while (1);
			break;
		case KEY1_LONG_PRESS:
			TurnOffLed(LED1);
			break;
		case KEY2_SHORT_PRESS:
			TurnOnLed(LED2);
			break;
		case KEY2_LONG_PRESS:
			TurnOffLed(LED2);
			break;
		case KEY3_SHORT_PRESS:
			TurnOnLed(LED3);
			break;
		case KEY3_LONG_PRESS:
			TurnOffLed(LED3);
			break;
		case KEY4_SHORT_PRESS:
			TurnOnLed(LED1);
			TurnOnLed(LED2);
			TurnOnLed(LED3);
			break;
		case KEY4_LONG_PRESS:
			TurnOffLed(LED1);
			TurnOffLed(LED2);
			TurnOffLed(LED3);
			break;
		default:
			break;
		}
}

