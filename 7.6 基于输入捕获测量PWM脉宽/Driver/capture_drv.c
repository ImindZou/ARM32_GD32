/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-15 15:19:56
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-15 15:51:04
 * @FilePath: \7.3 �������벶�����PWM����\Driver\capture_drv.c
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
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
	/*ʹ�ܶ�ʱ��ʱ��*/
	rcu_periph_clock_enable(RCU_TIMER1);
	/*��λ��ʱ��*/
	timer_deinit(TIMER1);
	timer_parameter_struct timerInitPara;
	timer_struct_para_init(&timerInitPara);
	/*����Ԥ��Ƶֵ*/
	timerInitPara.prescaler = 120 - 1;	//�����������ʱ��Ƶ��Ϊ1MHz������1us
	/*�����Զ�����ֵ*/
	timerInitPara.period = 65535;
	timer_init(TIMER1,&timerInitPara);

	/*����ͨ��Ϊ���벶����*/
	timer_ic_parameter_struct icinitPara;
	timer_channel_input_struct_para_init(&icinitPara);
	
	/*���ò���ͨ��0�����ز���*/
	icinitPara.icpolarity = TIMER_IC_POLARITY_RISING;
	/*��������ͨ��0*/
	icinitPara.icselection = TIMER_IC_SELECTION_DIRECTTI;
#if 1  //Ч���������һ��
	timer_input_pwm_capture_config(TIMER1,TIMER_CH_0,&icinitPara);
#endif
#if 0
	timer_input_capture_config(TIMER1, TIMER_CH_0, &icinitPara);
	
	/*���ò���ͨ��1�½��ز���*/
	icinitPara.icpolarity = TIMER_IC_POLARITY_FALLING;
	/*��������ͨ��1*/
	icinitPara.icselection = TIMER_IC_SELECTION_INDIRECTTI;
	timer_input_capture_config(TIMER1, TIMER_CH_1, &icinitPara);
#endif
	/*ʹ�ܶ�ʱ���Ĳ����ж�*/
	timer_interrupt_flag_clear(TIMER1,TIMER_INT_CH0);
	timer_interrupt_enable(TIMER1,TIMER_INT_CH0);
	/*ʹ�ܶ�ʱ���жϺ����ȼ�*/
	nvic_irq_enable(TIMER1_IRQn,0,0);
	/*ʹ�ܶ�ʱ��*/
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
