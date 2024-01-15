/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-15 15:19:56
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-15 15:51:04
 * @FilePath: \7.3 基于输入捕获测量PWM周期\Driver\capture_drv.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdint.h>
#include "gd32f30x.h"
#include "stdio.h"
#include "delay.h"

static void GpioInit(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);
	gpio_init(GPIOA,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_10MHZ,GPIO_PIN_0);
}

static void TimerInit(void)
{
	/*使能定时器时钟*/
	rcu_periph_clock_enable(RCU_TIMER1);
	/*复位定时器*/
	timer_deinit(TIMER1);
	timer_parameter_struct timerInitPara;
	timer_struct_para_init(&timerInitPara);
	/*设置预分频值*/
	timerInitPara.prescaler = 120 - 1;	//输入给计数器时钟频率为1MHz，周期1us
	/*设置自动重载值*/
	timerInitPara.period = 65535;
	timer_init(TIMER1,&timerInitPara);

	/*设置通道为输入捕获功能*/
	timer_ic_parameter_struct icinitPara;
	timer_channel_input_struct_para_init(&icinitPara);
	
	/*设置捕获通道0上升沿捕获*/
	icinitPara.icpolarity = TIMER_IC_POLARITY_RISING;
	/*设置输入通道0*/
	icinitPara.icselection = TIMER_IC_SELECTION_DIRECTTI;
#if 1  //效果跟下面的一样
	timer_input_pwm_capture_config(TIMER1,TIMER_CH_0,&icinitPara);
#endif
#if 0
	timer_input_capture_config(TIMER1, TIMER_CH_0, &icinitPara);
	
	/*设置捕获通道1下降沿捕获*/
	icinitPara.icpolarity = TIMER_IC_POLARITY_FALLING;
	/*设置输入通道1*/
	icinitPara.icselection = TIMER_IC_SELECTION_INDIRECTTI;
	timer_input_capture_config(TIMER1, TIMER_CH_1, &icinitPara);
#endif
	/*使能定时器的捕获中断*/
	timer_interrupt_flag_clear(TIMER1,TIMER_INT_CH0);
	timer_interrupt_enable(TIMER1,TIMER_INT_CH0);
	/*使能定时器中断和优先级*/
	nvic_irq_enable(TIMER1_IRQn,0,0);
	/*使能定时器*/
	timer_enable(TIMER1);
}

static uint32_t g_periodUs;
static uint32_t g_pulseUs;

void TIMER1_IRQHandler(void)
{
	if(timer_interrupt_flag_get(TIMER1,TIMER_INT_CH0) != RESET)
	{
		timer_interrupt_flag_clear(TIMER1,TIMER_INT_CH0);
		g_periodUs = timer_channel_capture_value_register_read(TIMER1,TIMER_CH_0) + 1;
		g_pulseUs = timer_channel_capture_value_register_read(TIMER1,TIMER_CH_1) + 1;
		timer_counter_value_config(TIMER1,0);
	}
}

void CaptureDrvInit(void)
{
	GpioInit();
	TimerInit();
}

void CaptureDrvTest(void)
{
	printf("period is %d us pulse is %d us.\n",g_periodUs,g_pulseUs);
	DelayNms(500);
}
