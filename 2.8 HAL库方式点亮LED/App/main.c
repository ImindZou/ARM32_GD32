#include <stdint.h>
#include "gd32f30x.h"


//#define  GPIOA_CTL1 (*(uint32_t *)(0x40010800 + 0x04))
//#define  GPIOA_OCTL (*(uint32_t *)(0x40010800 + 0x0C))
//#define  RCU_APB2EN (*(uint32_t *)(0x40021000 + 0x18))

static void Delay(uint32_t count)
{
	while (count--);
}

int main(void)
{
	/*使能GPIO的时钟*/
	//RCU_APB2EN |= 1 << 2;
	rcu_periph_clock_enable(RCU_GPIOA);
	/*配置为推挽输出模式*/
	//GPIOA_CTL1 = 0x44444442;
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_8);


	while (1)
	{
		#if 1
		/*配置为输出高电平*/
		//GPIOA_OCTL |= 1 << 8;  //00000000 00000000 00000001 00000000	
		//gpio_bit_set(GPIOA, GPIO_PIN_8);
		gpio_bit_write(GPIOA, GPIO_PIN_8, SET);
		Delay(1000000);
		/*配置为输出低电平*/
		//GPIOA_OCTL &= ~(1 << 8); //11111111 11111111 11111110 11111111
		//gpio_bit_reset(GPIOA, GPIO_PIN_8);
		gpio_bit_write(GPIOA, GPIO_PIN_8, RESET);
		Delay(1000000);
		#endif
		while (gpio_input_bit_get(GPIOA, GPIO_PIN_0) == RESET)
		{
			gpio_bit_write(GPIOA, GPIO_PIN_8, SET);
		}
		gpio_bit_write(GPIOA, GPIO_PIN_8, RESET);
	}
}
