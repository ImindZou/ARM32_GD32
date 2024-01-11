#ifndef _USB2COM_DRV_H_
#define _USB2COM_DRV_H_
#include <stdint.h>


static void Usb2ComGpioInit(void);

static void UsbComUartInit(uint32_t baudRate);

void Usb2ComTest(void);

/**
***********************************************************
* @brief USB转串口硬件初始化
* @param
* @return 
***********************************************************
*/
void Usb2ComDrvInit(void);




#endif

