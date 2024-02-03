#ifndef __INFLASH_DRV_H_
#define __INFLASH_DRV_H_

#include <stdbool.h>
#include <stdint.h>

/**
***********************************************************
* @brief 指定地址开始读出指定个数的数据
* @param readAddr，读取地址
* @param pBuffer，数组首地址
* @param numToRead，要读出的数据个数
* @return 
***********************************************************
*/
bool FlashRead(uint32_t readAddr, uint8_t *pBuffer, uint32_t numToRead);

/**
***********************************************************
* @brief 指定地址开始写入指定个数的数据
* @param writeAddr，读写入址
* @param pBuffer，数组首地址
* @param numToRead，要写入的数据个数
* @return 
***********************************************************
*/
bool FlashWrite(uint32_t writeAddr, uint8_t *pBuffer, uint32_t numToWrite);

/**
***********************************************************
* @brief 擦除从eraseAddr开始到eraseAddr + numToErase的页
* @param eraseAddr，地址
* @param numToErase，对应写入数据时的个数
* @return 
***********************************************************
*/
bool FlashErase(uint32_t eraseAddr, uint32_t numToErase);

void FlashDrvTest(void);

#endif
