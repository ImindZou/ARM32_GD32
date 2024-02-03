#include "gd32f30x.h"

void InitIrqAfterBoot(void)
{
	nvic_vector_table_set(NVIC_VECTTAB_FLASH, 0x3000);
	__enable_irq();
}

void ResetToBoot(void)
{
	__disable_irq();    //关闭所有中断
	NVIC_SystemReset(); //复位函数，需要一些执行的时间
}
