#ifndef _USB2COM_DRV_H_
#define _USB2COM_DRV_H_

#include <stdint.h>

/**
***********************************************************
* @brief USB转串口硬件初始化
* @param
* @return 
***********************************************************
*/
void Usb2ComDrvInit(void);

/**
***********************************************************
* @brief 注册回调函数
* @param pFunc，函数指针变量，接收传入的回调函数地址
* @return 
***********************************************************
*/
void regUsb2ComCb(void (*pFunc)(uint8_t data));

#endif
