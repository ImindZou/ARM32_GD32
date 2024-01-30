#include <stdint.h>
#include <stdio.h>
#include "rtc_drv.h"
#include "sensor_drv.h"
/**
***********************************************************
* @brief 人机交互任务处理函数
* @param 
* @return 
***********************************************************
*/
void HmiTask(void)
{
	SensorData_t sensorData;
	GetSensorData(&sensorData);
	printf("\n temp is %.1f, humi is %d.\n", sensorData.temp, sensorData.humi);
}
