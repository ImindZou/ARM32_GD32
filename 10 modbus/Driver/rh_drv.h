#ifndef _RH_DRV_H_
#define _RH_DRV_H_

#include <stdint.h>
/**
***********************************************************
* @brief ʪ�ȴ�����������ʼ��
* @param
* @return 
***********************************************************
*/
void HumiDrvInit(void);

/**
***********************************************************
* @brief ��������ת��ʪ�ȴ���������
* @param
* @return 
***********************************************************
*/
void HumiSensorProc(uint8_t temp);

/**
***********************************************************
* @brief ��ȡʪ�ȴ���������
* @param
* @return ʪ�����ݣ�����
***********************************************************
*/
float GetHumiData(void);
#endif