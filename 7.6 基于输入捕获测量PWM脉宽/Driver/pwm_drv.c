/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-14 22:11:23
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-14 22:31:25
 * @FilePath: \7.2 基于PWM实现呼吸灯\Driver\pwm_drv.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdint.h>
#include "gd32f30x.h"
#include "led_drv.h"
#include "delay.h"

static void GpioInit(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);
	gpio_init(GPIOA,GPIO_MODE_AF_PP,GPIO_OSPEED_10MHZ,GPIO_PIN_8);
}

static void TimerInit(void)
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
	timerInitPara.period = 500 - 1;
	timer_init(TIMER0,&timerInitPara);
	
	timer_oc_parameter_struct ocInitPara;
	timer_channel_output_struct_para_init(&ocInitPara);
	/*设置通道为输出功能*/
	ocInitPara.outputnstate = TIMER_CCX_ENABLE;
	/*设置通道输出极性*/
	ocInitPara.ocpolarity = TIMER_OC_POLARITY_HIGH;
	timer_channel_output_config(TIMER0,TIMER_CH_0,&ocInitPara);
	/*设置占空比*/
	timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_0,400-1);
	/*设置通道输出PWM模式*/
	timer_channel_output_mode_config(TIMER0,TIMER_CH_0,TIMER_OC_MODE_PWM0);
	timer_primary_output_config(TIMER0,ENABLE);
	/*使能定时器*/
	timer_enable(TIMER0);
}

void PwmDrvInit(void)
{
	GpioInit();
	TimerInit();
}

void PwmDrvTest(void)
{
	for(uint32_t i = 0; i < 500; i += 10)
	{
		timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_0,i);
		DelayNms(50);	
	}

	for(uint32_t i = 500; i > 0; i -= 10)
	{
		timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_0,i);
		DelayNms(50);	
	}

}
