/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-25 16:01:59
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-25 17:07:46
 * @FilePath: \9.7 �����������ܴ\App\hmi_app.c
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdint.h>
#include <stdio.h>
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
	printf("\n temp is %.1f.\n");
}

