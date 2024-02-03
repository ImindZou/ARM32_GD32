#include "gd32f30x.h"

void InitIrqAfterBoot(void)
{
	nvic_vector_table_set(NVIC_VECTTAB_FLASH, 0x3000);
	__enable_irq();
}

void ResetToBoot(void)
{
	__disable_irq();    //�ر������ж�
	NVIC_SystemReset(); //��λ��������ҪһЩִ�е�ʱ��
}
