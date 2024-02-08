#include <stdint.h>
#include <stdio.h>


int32_t Caculate(int32_t val1, int32_t val2)
{
	return val1 / val2;
}

int32_t Process(void)
{
	volatile int32_t aVal = 10;
	volatile int32_t bVal = 0;
	return Caculate(aVal, bVal);
}

int main(void)
{	
	volatile int *SCB_CCR = (volatile int *)0xE000ED14;
	*SCB_CCR |= (1<<4);
	volatile int32_t mVal = Process();
	
	while (1)
	{

	}
}
