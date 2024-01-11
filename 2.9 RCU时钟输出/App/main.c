#include <stdint.h>
#include "gd32f30x.h"


int main(void)
{
	/*使能GPIO的时钟*/
	//RCU_APB2EN |= 1 << 2;
	rcu_periph_clock_enable(RCU_GPIOA);
	/*配置为推挽输出模式*/
	//GPIOA_CTL1 = 0x44444442;
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_8);
  rcu_ckout0_config(RCU_CKOUT0SRC_CKSYS);
  while(1);
}
