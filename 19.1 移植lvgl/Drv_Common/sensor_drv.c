#include <stdint.h>
#include "gd32f30x.h"
#include "sensor_drv.h"
#include "ntc_drv.h"
#include "rh_drv.h"
/**
***********************************************************
* @brief ������������ʼ��
* @param
* @return 
***********************************************************
*/
void SensorDrvInit(void)
{
	TempDrvInit();
	HumiDrvInit();
}

/**
***********************************************************
* @brief ��������ת������������
* @param
* @return 
***********************************************************
*/
void SensorDrvProc(void)
{
	TempSensorProc();
	HumiSensorProc((uint8_t)GetTempData());
}

/**
***********************************************************
* @brief ��ȡ����������
* @param sensorData,��������������ݻ�д��ַ
* @return 
***********************************************************
*/
void GetSensorData(SensorData_t *sensorData)
{
	sensorData->temp = GetTempData();
	sensorData->humi = GetHumiData();
}
