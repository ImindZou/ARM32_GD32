#include <stdint.h>
#include "gd32f30x.h"
#include "sensor_drv.h"
#include "ntc_drv.h"

/**
***********************************************************
* @brief 传感器驱动初始化
* @param
* @return 
***********************************************************
*/
void SensorDrvInit(void)
{
	TempDrvInit();
}

/**
***********************************************************
* @brief 触发驱动转换传感器数据
* @param
* @return 
***********************************************************
*/
void SensorDrvProc(void)
{
	TempSensorProc();
}

/**
***********************************************************
* @brief 获取传感器数据
* @param sensorData,输出，传感器数据回写地址
* @return 
***********************************************************
*/
void GetSensorData(SensorData_t *sensorData)
{
	sensorData->temp = GetTempData();
}
