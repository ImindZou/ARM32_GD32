/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-14 21:04:37
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-14 21:40:04
 * @FilePath: \7.1 基于定时器实现计时功能\Driver\timing_drv.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdint.h>
#include "gd32f30x.h"
#include "led_drv.h"

#if 0
static void TimerInit(uint32_t periodUs)
{
	/*使能定时器时钟*/
	rcu_periph_clock_enable(RCU_TIMER0);
	/*复位定时器*/
	timer_deinit(TIMER0);
	timer_parameter_struct timerInitPara;
	timer_struct_para_init(&timerInitPara);
	/*设置预分频值*/
	timerInitPara.prescaler = 120 - 1;	//输入给计数器时钟频率为1MHz，周期1us
	/*设置自动重载值*/
	timerInitPara.period = periodUs - 1;
	timer_init(TIMER0,&timerInitPara);
	/*使能定时器的计数更新中断*/
	timer_interrupt_enable(TIMER0,TIMER_INT_UP);
	/*使能定时器中断和优先级*/
	nvic_irq_enable(TIMER0_UP_IRQn,0,0);
	/*使能定时器*/
	timer_enable(TIMER0);
}

void TIMER0_UP_IRQHandler(void)
{
	if(timer_interrupt_flag_get(TIMER0,TIMER_INT_FLAG_UP) != RESET)
	{
		timer_interrupt_flag_clear(TIMER0,TIMER_INT_FLAG_UP);
		ToggleLed(LED1);
	}
}
#endif
static void TimerInit(uint32_t periodUs)
{
	/*使能定时器时钟*/
	rcu_periph_clock_enable(RCU_TIMER4);
	/*复位定时器*/
	timer_deinit(TIMER4);
	timer_parameter_struct timerInitPara;
	timer_struct_para_init(&timerInitPara);
	/*设置预分频值*/
	timerInitPara.prescaler = 120 - 1;	//输入给计数器时钟频率为1MHz，周期1us
	/*设置自动重载值*/
	timerInitPara.period = periodUs - 1;
	timer_init(TIMER4,&timerInitPara);
	/*使能定时器的计数更新中断*/
	timer_interrupt_enable(TIMER4,TIMER_INT_UP);
	/*使能定时器中断和优先级*/
	nvic_irq_enable(TIMER4_IRQn,0,0);
	/*使能定时器*/
	timer_enable(TIMER4);
}

void TIMER4_IRQHandler(void)
{
	if(timer_interrupt_flag_get(TIMER4,TIMER_INT_FLAG_UP) != RESET)
	{
		timer_interrupt_flag_clear(TIMER4,TIMER_INT_FLAG_UP);
		ToggleLed(LED1);
	}
}
void TimingDrvInit(void)
{
	TimerInit(1000);
}

