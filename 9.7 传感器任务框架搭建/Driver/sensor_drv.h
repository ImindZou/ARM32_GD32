#ifndef _SENSOR_DRV_H_
#define _SENSOR_DRV_H_

#include <stdint.h>

typedef struct
{
	float temp;
	uint8_t humi;
} SensorData_t;

/**
***********************************************************
* @brief 传感器驱动初始化
* @param
* @return 
***********************************************************
*/
void SensorDrvInit(void);

/**
***********************************************************
* @brief 获取传感器数据
* @param sensorData,输出，传感器数据回写地址
* @return 
***********************************************************
*/
void GetSensorData(SensorData_t *sensorData);

/**
***********************************************************
* @brief 触发驱动转换传感器数据
* @param
* @return 
***********************************************************
*/
void SensorDrvProc(void);

#endif
