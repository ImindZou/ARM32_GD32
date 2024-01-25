#include <stdint.h>
#include "gd32f30x.h"

/**
***********************************************************
* @brief DWT初始化配置
* @param
* @return 
***********************************************************
*/
void DelayInit(void) 
{
	/* 关闭 TRC */
	CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk;
	/* 打开 TRC */
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

	/* 关闭计数功能 */
	DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk;
	/* 打开计数功能 */
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

	/* 计数清零 */
	DWT->CYCCNT = 0;
}

/**
***********************************************************
* @brief 微秒级延时函数
* @param nUs，最大延时时间( 2^32 / 内核主频 ) * 10^6 us 
* @return 
***********************************************************
*/
void DelayNus(uint32_t nUs)
{
	uint32_t tickStart = DWT->CYCCNT;

	/* 转换为nUs对应的时钟跳动次数*/
	nUs *= (rcu_clock_freq_get(CK_AHB) / 1000000);

	/* 延时等待 */
	while ((DWT->CYCCNT - tickStart) < nUs);
}

/**
***********************************************************
* @brief 毫秒级延时函数
* @param nMs，延时时间n毫秒
* @return 
***********************************************************
*/
void DelayNms(uint32_t nMs)
{
	for (uint32_t i = 0; i < nMs; i++)
	{
		DelayNus(1000);
	}
}

