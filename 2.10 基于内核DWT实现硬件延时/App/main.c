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
	while (1)
	{
#if 1
		/*����Ϊ����ߵ�ƽ*/
		// GPIOA_OCTL |= 1 << 8;  //00000000 00000000 00000001 00000000
		// gpio_bit_set(GPIOA, GPIO_PIN_8);
		gpio_bit_write(GPIOA, GPIO_PIN_8, SET);

		DelayNms(50);
		/*����Ϊ����͵�ƽ*/
		// GPIOA_OCTL &= ~(1 << 8); //11111111 11111111 11111110 11111111
		// gpio_bit_reset(GPIOA, GPIO_PIN_8);
		gpio_bit_write(GPIOA, GPIO_PIN_8, RESET);
		DelayNms(50);
#endif

	}
}
