#include <stdio.h>
#include <stdint.h>
#include "gd32f30x.h"
#include "led_drv.h"
#include "key_drv.h"
#include "usb2Com_drv.h"

int main(void)
{
	LedDrvInit();
	KeyDrvInit();
	Usb2ComDrvInit();
	
//			Usb2ComTest();
	printf("Hello world! \n");
	printf("×öÄà\r\n");
	while (1)
	{
		
	}
}
