/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-25 16:01:59
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-25 16:19:37
 * @FilePath: \9.7 �����������ܴ\Driver\sensor_drv.h
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __SENSOR_DRV_H_
#define __SENSOR_DRV_H_
#include <stdint.h>

typedef struct 
{   
    float temp;
    uint8_t humi;
} SensorData_t;
/**
***************************************
*@brief ������������ʼ��
*@param 
*@return 
***************************************
*/
void SensorDrvInit(void);


/**
***************************************
*@brief ��ȡ����������
*@param sensorData,��������������ݻ�д��ַ
*@return 
***************************************
*/
void GetSensorData(SensorData_t *sensorData);

/**
***************************************
*@brief ��������ת������������
*@param 
*@return 
***************************************
*/
void SensorDrvProc(void);



#endif
