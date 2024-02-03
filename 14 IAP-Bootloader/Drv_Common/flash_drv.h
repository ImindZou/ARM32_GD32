#ifndef _FLASH_DRV_H_
#define _FLASH_DRV_H_

#include <stdint.h>
#include <stdbool.h>
/**
*******************************************************************
* @function ָ����ַ��ʼ����ָ������������
* @param    readAddr,��ȡ��ַ
* @param    pBuffer,�����׵�ַ
* @param    numToRead,Ҫ���������ݸ���
* @return   
*******************************************************************
*/
bool FlashRead(uint32_t readAddr, uint8_t *pBuffer, uint32_t numToRead);

/**
*******************************************************************
* @function ָ����ַ��ʼд��ָ������������
* @param    writeAddr,д���ַ
* @param    pBuffer,�����׵�ַ
* @param    numToWrite,Ҫд������ݸ���
* @return                                                         
*******************************************************************
*/
bool FlashWrite(uint32_t writeAddr, uint8_t *pBuffer, uint32_t numToWrite);

/**
*******************************************************************
* @function ������eraseAddr��ʼ��eraseAddr + numToErase��ҳ
* @param    eraseAddr,��ַ
* @param    numToErase,��Ӧд������ʱ�ĸ���
* @return                                                         
*******************************************************************
*/
bool FlashErase(uint32_t eraseAddr, uint32_t numToErase);

void FlashDrvTest(void);

#endif
