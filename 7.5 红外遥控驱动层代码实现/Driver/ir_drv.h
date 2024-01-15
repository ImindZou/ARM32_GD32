#ifndef __IR_DRV_H_
#define __IR_DRV_H_

#include <stdint.h>
#include <stdbool.h>

#define KEY1_CODE 0x45
#define KEY2_CODE 0x46

/**
***********************************************************
* @brief 获取遥控键码值
* @param code,输出，按键键码值
* @return 是否成功获取到按键码值
***********************************************************
*/
bool GetIrCode(uint8_t *code);

/**
***********************************************************
* @brief 红外接收硬件初始化函数
* @param 
* @return 
***********************************************************
*/
void IrDrvInit(void);
#endif
