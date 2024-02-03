#ifndef __INFLASH_DRV_H_
#define __INFLASH_DRV_H_

#include <stdbool.h>
#include <stdint.h>

/**
***********************************************************
* @brief ָ����ַ��ʼ����ָ������������
* @param readAddr����ȡ��ַ
* @param pBuffer�������׵�ַ
* @param numToRead��Ҫ���������ݸ���
* @return 
***********************************************************
*/
bool FlashRead(uint32_t readAddr, uint8_t *pBuffer, uint32_t numToRead);

/**
***********************************************************
* @brief ָ����ַ��ʼд��ָ������������
* @param writeAddr����д��ַ
* @param pBuffer�������׵�ַ
* @param numToRead��Ҫд������ݸ���
* @return 
***********************************************************
*/
bool FlashWrite(uint32_t writeAddr, uint8_t *pBuffer, uint32_t numToWrite);

/**
***********************************************************
* @brief ������eraseAddr��ʼ��eraseAddr + numToErase��ҳ
* @param eraseAddr����ַ
* @param numToErase����Ӧд������ʱ�ĸ���
* @return 
***********************************************************
*/
bool FlashErase(uint32_t eraseAddr, uint32_t numToErase);

void FlashDrvTest(void);

#endif
