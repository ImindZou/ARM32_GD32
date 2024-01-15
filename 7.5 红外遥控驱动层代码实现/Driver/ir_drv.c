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
	/*使能定时器时钟*/
	rcu_periph_clock_enable(RCU_TIMER7);
	/*复位定时器*/
	timer_deinit(TIMER7);
	timer_parameter_struct timerInitPara;
	timer_struct_para_init(&timerInitPara);
	/*设置预分频值*/
	timerInitPara.prescaler = 120 - 1;	//输入给计数器时钟频率为1MHz，周期1us
	/*设置自动重载值*/
	timerInitPara.period = 65535;
	timer_init(TIMER7,&timerInitPara);

	/*设置通道为输入捕获功能*/
	timer_ic_parameter_struct icinitPara;
	timer_channel_input_struct_para_init(&icinitPara);
	/*设置上升沿/下降沿捕获*/
	icinitPara.icpolarity = TIMER_IC_POLARITY_FALLING;
	/*设置输入通道*/
	icinitPara.icselection = TIMER_IC_SELECTION_DIRECTTI;
	timer_input_capture_config(TIMER7, TIMER_CH_0, &icinitPara);
	/*使能定时器的捕获中断*/
	timer_interrupt_flag_clear(TIMER7,TIMER_INT_CH0);
	timer_interrupt_enable(TIMER7,TIMER_INT_CH0);
	/*使能定时器中断和优先级*/
	nvic_irq_enable(TIMER7_Channel_IRQn,0,0);
	/*使能定时器*/
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
	if(tickNum > TICK_1_MIN && tickNum < TICK_1_MAX)	//解析二进制位1
	{
		g_irCode[s_index / 8] >>= 1;		//逆天移位算法
		g_irCode[s_index / 8] |= 0x80;	
		s_index ++;
	}
	if(tickNum > TICK_0_MIN && tickNum < TICK_0_MAX)	//解析二进制位0
	{
		g_irCode[s_index / 8] >>= 1;		//逆天移位算法
		s_index ++;
	}
	if(s_index == 32)
	{	
		//if(g_irCode[2] == (uint8_t)~g_irCode[3])	//第一个方案的解决方法	keilMDK的问题，keilC51正常
		if((g_irCode[2] & g_irCode[3]) == 0)//第二个解决方案 
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
* @brief 获取遥控键码值
* @param code,输出，按键键码值
* @return 是否成功获取到按键码值
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
* @brief 红外接收硬件初始化函数
* @param 
* @return 
***********************************************************
*/
void IrDrvInit(void)
{
	GpioInit();
	TimerInit();
}





