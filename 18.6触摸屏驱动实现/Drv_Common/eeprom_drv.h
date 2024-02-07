#ifndef _EEPROM_DRV_H_
#define _EEPROM_DRV_H_

#include <stdint.h>
#include <stdbool.h>

void EepromDrvInit(void);
void EepromDrvTest(void);

/**
*******************************************************************
* @function 指定地址开始写入指定个数的数据
* @param    writeAddr,写入地址，0~255
* @param    pBuffer,数组首地址
* @param    numToWrite,要写入的数据个数,不大于256
* @return                                                         
*******************************************************************
*/
bool WriteEepromData(uint8_t writeAddr, uint8_t *pBuffer, uint16_t numToWrite);

/**
*******************************************************************
* @function 指定地址开始读出指定个数的数据
* @param    readAddr,读取地址，0~255
* @param    pBuffer,数组首地址
* @param    numToRead,要读出的数据个数,不大于256
* @return   
*******************************************************************
*/
bool ReadEepromData(uint8_t readAddr, uint8_t *pBuffer, uint16_t numToRead);

#endif
