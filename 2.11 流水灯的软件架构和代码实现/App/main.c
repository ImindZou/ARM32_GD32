/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-09 16:57:50
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-09 18:20:32
 * @FilePath: \2.11 ��ˮ�Ƶ�����ܹ��ʹ���ʵ��\App\main.c
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/**
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-09 15:38:06
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-09 16:38:54
 * @FilePath: \2.10 �����ں�DWTʵ��Ӳ����ʱ\App\main.c
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdint.h>
#include "gd32f30x.h"
#include "delay.h"
#include "led_drv.h"

// #define  GPIOA_CTL1 (*(uint32_t *)(0x40010800 + 0x04))
// #define  GPIOA_OCTL (*(uint32_t *)(0x40010800 + 0x0C))
// #define  RCU_APB2EN (*(uint32_t *)(0x40021000 + 0x18))

int main(void)
{
	/*ʹ��GPIO��ʱ��*/
	// RCU_APB2EN |= 1 << 2;
	rcu_periph_clock_enable(RCU_GPIOA);
	/*����Ϊ�������ģʽ*/
	// GPIOA_CTL1 = 0x44444442;
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_8);
	Delay_Init();
	LedDrvInit();
	
	while (1)
	{
#if 1
		TurnOnLed(LED1);
		DelayNms(200);
		TurnOffLed(LED1);
		TurnOnLed(LED2);
		DelayNms(200);
		TurnOffLed(LED2);
		TurnOnLed(LED3);
		DelayNms(200);
		TurnOffLed(LED3);
		DelayNms(200);
#endif
	}
}
