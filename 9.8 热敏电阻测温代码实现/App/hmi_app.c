/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-25 16:01:59
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-25 17:07:46
 * @FilePath: \9.7 传感器任务框架搭建\App\hmi_app.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdint.h>
#include <stdio.h>
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
	printf("\n temp is %.1f.\n");
}

