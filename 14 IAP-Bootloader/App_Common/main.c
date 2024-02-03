#include <stdint.h>
#include <stdio.h>
#include "systick.h"
#include "rs485_drv.h"
#include "delay.h"
#include "update.h"
#include "gd32f30x.h"

#define BOOT_DELAY_COUNT                    20000  // 单位ms

typedef void (*pFunction)(void);

static void DrvInit(void)
{
	RS485DrvInit();
	DelayInit();
	SystickInit();
}

#define RAM_START_ADDRESS    0x20000000
#define RAM_SIZE             0x10000

static void BootToApp(void)
{
	uint32_t stackTopAddr = *(volatile uint32_t*)APP_ADDR_IN_FLASH; 
	if (stackTopAddr > RAM_START_ADDRESS && stackTopAddr < (RAM_START_ADDRESS + RAM_SIZE)) //判断栈顶地址是否在合法范围内
	{
		__disable_irq();
		
		__set_MSP(stackTopAddr);
		uint32_t resetHandlerAddr = *(volatile uint32_t*) (APP_ADDR_IN_FLASH + 4);
		/* Jump to user application */
		pFunction Jump_To_Application = (pFunction) resetHandlerAddr; // int *p = (int *)0x8003145
		/* Initialize user application's Stack Pointer */
		Jump_To_Application();
	}
	NVIC_SystemReset();
}

#define DOWNLOAD_KEY_VALUE                 0x31
#define EXECUTE_KEY_VALUE                  0x32
static void MainMenuCmd(void)
{
	printf("\rHit any key to stop autoboot:  ");
	
	uint8_t serialKey;
	uint32_t timCount = (uint32_t)GetSysRunTime();
	uint8_t bootDelayNow = 0;
	uint8_t bootDelayLast = 0;
	while ((timCount < BOOT_DELAY_COUNT) && !GetKeyPressed(&serialKey))
	{
		timCount = (uint32_t)GetSysRunTime();
		bootDelayNow = (BOOT_DELAY_COUNT - timCount) / 1000;
		if (bootDelayNow != bootDelayLast)
		{
			printf("\b\b%2d", bootDelayNow);
			bootDelayLast = bootDelayNow; 
		}
		
	}
	
	if (timCount >= BOOT_DELAY_COUNT)
	{
		BootToApp();  // 跳转到APP执行
	}

	while (1)
	{
	
		printf("\r\n\n======================= Main Menu ============================\r\n\n");
		printf("************[1].Download Image To Internal Flash*************\r\n\n");
		printf("************[2].Execute The APP******************************\r\n\n");
		printf("\r\n==============================================================\r\n\n");

		while (!GetKeyPressed(&serialKey));

		if (serialKey == DOWNLOAD_KEY_VALUE)
		{
			UpdateApp();
			
		}
		if (serialKey == EXECUTE_KEY_VALUE)
		{
			BootToApp();
		}
	}
}

int main(void)
{	
	DrvInit();

	while (1)
	{
		MainMenuCmd();
	}
}
