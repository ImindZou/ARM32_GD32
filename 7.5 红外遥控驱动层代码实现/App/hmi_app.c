#include <stdint.h>
#include "led_drv.h"
#include "ir_drv.h"
#include <stdio.h>

/**
***********************************************************
* @brief 人机交互任务处理函数
* @param 
* @return 
***********************************************************
*/
void HmiTask(void)
{
	uint8_t keyVal;
	if(!GetIrCode(&keyVal))
	{
		return;
	}
	printf("ir keyVal is 0x%x.\n",keyVal);
	switch (keyVal)
	{
	case KEY1_CODE:
		TurnOnLed(LED1);
		break;
	case KEY2_CODE:
		TurnOffLed(LED1);
		break;
	default:
		break;
	}
}

