#include <stdint.h>
#include "led_drv.h"
#include "key_drv.h"

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
	keyVal = GetKeyVal();
	switch (keyVal)
		{
		case KEY1_SHORT_PRESS:
			TurnOnLed(LED1);
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

