#include <stdint.h>
#include <stdio.h>
#include "rtc_drv.h"
#include "sensor_drv.h"
/**
***********************************************************
* @brief �˻�������������
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
