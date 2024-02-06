#include <stdint.h>
#include <stdio.h>
#include "rtc_drv.h"
#include "sensor_drv.h"
#include "led_drv.h"
#include "key_drv.h"
#include "fatfs_app.h"

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
			PrintSensorFile();
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
