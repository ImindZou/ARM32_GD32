/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-12 01:12:32
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-12 02:13:28
 * @FilePath: \5.4 如何使用串口�?�?接收数据\App\main.c
 * @Description: 这是默�?��?�置,请�?�置`customMade`, 打开koroFileHeader查看配置 进�?��?�置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include <stdint.h>
#include "gd32f30x.h"
#include "led_drv.h"
#include "key_drv.h"
#include "usb2Com_drv.h"
#include "usb2Com_app.h"
#include "systick.h"
#include "delay.h"

static void DrvInit(void)
{
	SystickInit();
	DelayInit();
	LedDrvInit();
	KeyDrvInit();
	Usb2ComDrvInit();
}

static void AppInit(void)
{
	Usb2ComAppInit();
}

int main(void)
{
	DrvInit();
	AppInit();
	
	while (1)
	{
		Usb2ComTask();
		DelayNms(500);
	}
}
