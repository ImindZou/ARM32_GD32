/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-18 14:45:37
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-18 15:13:10
 * @FilePath: \8.4 RTC驱动层代码实现d:\Study\ARM32单片机实战课程\06自己动手\8.5 独立看门狗\App\hmi_app.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdint.h>
#include <stdio.h>
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
			printf("************进入死循环**************\n");
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

