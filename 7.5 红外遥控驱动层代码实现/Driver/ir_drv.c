#include <stdint.h>
#include "gd32f30x.h"
#include "stdio.h"
#include <stdbool.h>

static void GpioInit(void)
{
	rcu_periph_clock_enable(RCU_GPIOC);
	gpio_init(GPIOC,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_10MHZ,GPIO_PIN_6);
}

static void TimerInit(void)
{
	/*ʹ�ܶ�ʱ��ʱ��*/
	rcu_periph_clock_enable(RCU_TIMER7);
	/*��λ��ʱ��*/
	timer_deinit(TIMER7);
	timer_parameter_struct timerInitPara;
	timer_struct_para_init(&timerInitPara);
	/*����Ԥ��Ƶֵ*/
	timerInitPara.prescaler = 120 - 1;	//�����������ʱ��Ƶ��Ϊ1MHz������1us
	/*�����Զ�����ֵ*/
	timerInitPara.period = 65535;
	timer_init(TIMER7,&timerInitPara);

	/*����ͨ��Ϊ���벶����*/
	timer_ic_parameter_struct icinitPara;
	timer_channel_input_struct_para_init(&icinitPara);
	/*����������/�½��ز���*/
	icinitPara.icpolarity = TIMER_IC_POLARITY_FALLING;
	/*��������ͨ��*/
	icinitPara.icselection = TIMER_IC_SELECTION_DIRECTTI;
	timer_input_capture_config(TIMER7, TIMER_CH_0, &icinitPara);
	/*ʹ�ܶ�ʱ���Ĳ����ж�*/
	timer_interrupt_flag_clear(TIMER7,TIMER_INT_CH0);
	timer_interrupt_enable(TIMER7,TIMER_INT_CH0);
	/*ʹ�ܶ�ʱ���жϺ����ȼ�*/
	nvic_irq_enable(TIMER7_Channel_IRQn,0,0);
	/*ʹ�ܶ�ʱ��*/
	timer_enable(TIMER7);
}

#define TICK_HEAD_MAX	20000
#define TICK_HEAD_MIN   10000
#define TICK_0_MAX		1800
#define TICK_0_MIN		500
#define TICK_1_MAX		3000
#define TICK_1_MIN		1800

static uint8_t g_irCode[4];
static bool g_irCodeFlag = false;

static void ParseIrFame(uint32_t tickNum)
{
	static bool s_headFlag = false;
	static uint8_t s_index = 0;
	if(tickNum > TICK_HEAD_MIN && tickNum < TICK_HEAD_MAX)
	{
		s_headFlag = true;
		return;
	}
	if(!s_headFlag)
	{
		return;
	}
	if(tickNum > TICK_1_MIN && tickNum < TICK_1_MAX)	//����������λ1
	{
		g_irCode[s_index / 8] >>= 1;		//������λ�㷨
		g_irCode[s_index / 8] |= 0x80;	
		s_index ++;
	}
	if(tickNum > TICK_0_MIN && tickNum < TICK_0_MAX)	//����������λ0
	{
		g_irCode[s_index / 8] >>= 1;		//������λ�㷨
		s_index ++;
	}
	if(s_index == 32)
	{	
		//if(g_irCode[2] == (uint8_t)~g_irCode[3])	//��һ�������Ľ������	keilMDK�����⣬keilC51����
		if((g_irCode[2] & g_irCode[3]) == 0)//�ڶ���������� 
		{
			g_irCodeFlag = true;
		}
		else
		{
			g_irCodeFlag = false;
		}
		s_headFlag = false;
		s_index = 0;
	}
}

/**
***********************************************************
* @brief ��ȡң�ؼ���ֵ
* @param code,�������������ֵ
* @return �Ƿ�ɹ���ȡ��������ֵ
***********************************************************
*/
bool GetIrCode(uint8_t *code)
{
	if(!g_irCodeFlag)
	{
		return false;
	}
	*code = g_irCode[2];
	g_irCodeFlag = false;
	return true;
}


void TIMER7_Channel_IRQHandler(void)
{
	uint32_t icValue;

	if(timer_interrupt_flag_get(TIMER7,TIMER_INT_CH0) != RESET)
	{
		timer_interrupt_flag_clear(TIMER7,TIMER_INT_CH0);
		icValue = timer_channel_capture_value_register_read(TIMER7,TIMER_CH_0) + 1;
		timer_counter_value_config(TIMER7,0);
		ParseIrFame(icValue);
	}
}

/**
***********************************************************
* @brief �������Ӳ����ʼ������
* @param 
* @return 
***********************************************************
*/
void IrDrvInit(void)
{
	GpioInit();
	TimerInit();
}





