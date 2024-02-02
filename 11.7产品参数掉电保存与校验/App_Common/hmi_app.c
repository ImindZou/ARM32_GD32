/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-02-02 11:36:54
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-02-02 12:43:14
 * @FilePath: \11.7产品参数掉电保存与校验\App_Common\hmi_app.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdint.h>
#include <stdio.h>
#include "rtc_drv.h"
#include "sensor_drv.h"
#include "led_drv.h"
#include "key_drv.h"
#include "store_app.h"

/**
***********************************************************
* @brief 人机交互任务处理函数
* @param 
* @return 
***********************************************************
*/
void HmiTask(void)
{
//	SensorData_t sensorData;
//	GetSensorData(&sensorData);
//	printf("\n temp is %.1f, humi is %d.\n", sensorData.temp, sensorData.humi);
	
	uint8_t keyVal;
	keyVal = GetKeyVal();
	switch (keyVal)
	{
		case KEY1_SHORT_PRESS:
			TurnOnLed(LED1);
			if(SetModbusParam(2))
			{
				printf("SetModbusParam sucess\n");
			}
			else
			{
				printf("SetModbusParam fail\r\n");
			}
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
		default:
			break;
	}
}
