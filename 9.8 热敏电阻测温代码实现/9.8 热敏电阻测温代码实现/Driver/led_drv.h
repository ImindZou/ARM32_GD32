#ifndef _LED_DRV_H_
#define _LED_DRV_H_

#include <stdint.h>

#define  LED1  0
#define  LED2  1
#define  LED3  2

/**
***********************************************************
* @brief LED硬件初始化
* @param
* @return 
***********************************************************
*/
void LedDrvInit(void);

/**
***********************************************************
* @brief 点亮LED
* @param ledNo，LED标号，0~2
* @return 
***********************************************************
*/
void TurnOnLed(uint8_t ledNo);

/**
***********************************************************
* @brief 熄灭LED
* @param ledNo，LED标号，0~2
* @return 
***********************************************************
*/
void TurnOffLed(uint8_t ledNo);

/**
***********************************************************
* @brief LED状态取反
* @param ledNo，LED标号，0~2
* @return 
***********************************************************
*/
void ToggleLed(uint8_t ledNo);
#endif
