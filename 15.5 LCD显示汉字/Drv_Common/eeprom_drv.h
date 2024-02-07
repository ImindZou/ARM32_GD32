#ifndef _EEPROM_DRV_H_
#define _EEPROM_DRV_H_

#include <stdint.h>
#include <stdbool.h>

void EepromDrvInit(void);
void EepromDrvTest(void);

/**
*******************************************************************
* @function ָ����ַ��ʼд��ָ������������
* @param    writeAddr,д���ַ��0~255
* @param    pBuffer,�����׵�ַ
* @param    numToWrite,Ҫд������ݸ���,������256
* @return                                                         
*******************************************************************
*/
bool WriteEepromData(uint8_t writeAddr, uint8_t *pBuffer, uint16_t numToWrite);

/**
*******************************************************************
* @function ָ����ַ��ʼ����ָ������������
* @param    readAddr,��ȡ��ַ��0~255
* @param    pBuffer,�����׵�ַ
* @param    numToRead,Ҫ���������ݸ���,������256
* @return   
*******************************************************************
*/
bool ReadEepromData(uint8_t readAddr, uint8_t *pBuffer, uint16_t numToRead);

#endif
