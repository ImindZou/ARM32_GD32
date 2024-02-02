/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-02-02 11:36:54
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-02-02 12:05:57
 * @FilePath: \11.7产品参数掉电保存与校验\Drv_Common\eeprom_drv.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef _EEPROM_DRV_H_
#define _EEPROM_DRV_H_
#include <stdbool.h>

void EepromDrvInit(void);
void EepromDrvTest(void);
bool ReadEepromData(uint8_t readAddr, uint8_t *pBuffer, uint16_t numToRead);
bool WriteEepromData(uint8_t writeAddr, uint8_t *pBuffer, uint16_t numToWrite);

#endif
