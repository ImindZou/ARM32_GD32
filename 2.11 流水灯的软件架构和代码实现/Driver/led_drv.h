#ifndef __LED_DRV_H
#define __LED_DRV_H

#include <stdint.h>

#define LED1 0
#define LED2 1
#define LED3 2

/**
***************************************
*@brief GPIO初始化
*@param 
*@return 
***************************************
*/
void LedDrvInit(void);

/**
***************************************
*@brief 点亮LED 
*@param ledNo,LED标号，0~2
*@return 
***************************************
*/
void TurnOnLed(uint8_t ledNo);

/**
***************************************
*@brief 熄灭LED 
*@param ledNo,LED标号，0~2 
*@return 
***************************************
*/
void TurnOffLed(uint8_t ledNo);

#endif
