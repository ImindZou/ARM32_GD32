#ifndef _NORFLASH_DRV_H_
#define _NORFLASH_DRV_H_

#include <stdint.h>

void NorflashDrvInit(void);

/**
*******************************************************************
* @function ָ����ַ��ʼ����ָ������������
* @param    readAddr,��ȡ��ַ
* @param    pBuffer,�����׵�ַ
* @param    len,Ҫ���������ݸ���
* @return   
*******************************************************************
*/
void ReadNorflashData(uint32_t readAddr, uint8_t *pBuffer, uint32_t len);

/**
*******************************************************************
* @function ָ����ַ��ʼд��ָ�����������ݣ�����ǰ��Ҫ����flash
* @param    writeAddr,д���ַ
* @param    pBuffer,�����׵�ַ
* @param    len,Ҫд������ݸ���
* @return                                                         
*******************************************************************
*/
void WriteNorflashData(uint32_t writeAddr, uint8_t *pBuffer, uint32_t len);

/**
*******************************************************************
* @function ������eraseAddr��ʼ��eraseAddr + len����Ӧ������
* @param    eraseAddr,��ַ
* @param    len,��Ӧд�����ݵĸ���
* @return                                                         
*******************************************************************
*/
void EraseNorflashForWrite(uint32_t eraseAddr, uint32_t len);

#endif
