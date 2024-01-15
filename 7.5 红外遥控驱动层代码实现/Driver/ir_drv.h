#ifndef __IR_DRV_H_
#define __IR_DRV_H_

#include <stdint.h>
#include <stdbool.h>

#define KEY1_CODE 0x45
#define KEY2_CODE 0x46

/**
***********************************************************
* @brief ��ȡң�ؼ���ֵ
* @param code,�������������ֵ
* @return �Ƿ�ɹ���ȡ��������ֵ
***********************************************************
*/
bool GetIrCode(uint8_t *code);

/**
***********************************************************
* @brief �������Ӳ����ʼ������
* @param 
* @return 
***********************************************************
*/
void IrDrvInit(void);
#endif
