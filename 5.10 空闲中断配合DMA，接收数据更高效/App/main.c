#include <stdint.h>
#include <stdio.h>
#include "gd32f30x.h"
#include "led_drv.h"
#include "key_drv.h"
#include "systick.h"
#include "usb2com_drv.h"

int main(void)
{	
	SystickInit();
	LedDrvInit();
	KeyDrvInit();
	Usb2ComDrvInit();
	
	while (1)
	{
		Usb2ComTask();
	}
}
