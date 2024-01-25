/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-25 16:01:59
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-25 16:19:37
 * @FilePath: \9.7 传感器任务框架搭建\Driver\sensor_drv.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __SENSOR_DRV_H_
#define __SENSOR_DRV_H_
#include <stdint.h>

/**
***************************************
*@brief 传感器驱动初始化
*@param 
*@return 
***************************************
*/
void SensorDrvInit(void);


typedef struct 
{   
    float temp;
    uint8_t humi;
} SensorData_t;

/**
***************************************
*@brief 获取传感器数据
*@param sensorData,输出，传感器数据回写地址
*@return 
***************************************
*/
void GetSensorData(SensorData_t *sensorData);

/**
***************************************
*@brief 触发驱动转换传感器数据
*@param 
*@return 
***************************************
*/
void SensorDrvProc(void);



#endif
