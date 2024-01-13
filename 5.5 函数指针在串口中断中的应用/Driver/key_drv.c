#include <stdint.h>
#include "gd32f30x.h"
#include "led_drv.h"

static void KeyGpioInit(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);
	gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_2MHZ, GPIO_PIN_0);
	
	rcu_periph_clock_enable(RCU_GPIOG);
	gpio_init(GPIOG, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_2MHZ, GPIO_PIN_13);
	gpio_init(GPIOG, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_2MHZ, GPIO_PIN_14);
}

static void KeyExtiInit(void)
{
	/*使能EXTI时钟*/
	rcu_periph_clock_enable(RCU_AF);
	/*I/O连接到EXTI线*/
	gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOA, GPIO_PIN_SOURCE_0);
	/*配置上升/下降沿*/
	exti_init(EXTI_0, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
	/*清除标志*/
	exti_interrupt_flag_clear(EXTI_0);
	/*使能中断*/
	nvic_irq_enable(EXTI0_IRQn, 1, 1);
	
	/*I/O连接到EXTI线*/
	gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOG, GPIO_PIN_SOURCE_13);
	/*配置上升/下降沿*/
	exti_init(EXTI_13, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
	/*清除标志*/
	exti_interrupt_flag_clear(EXTI_13);
	
	/*I/O连接到EXTI线*/
	gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOG, GPIO_PIN_SOURCE_14);
	/*配置上升/下降沿*/
	exti_init(EXTI_14, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
	/*清除标志*/
	exti_interrupt_flag_clear(EXTI_14);
	
	/*使能中断*/
	nvic_irq_enable(EXTI10_15_IRQn, 0, 1);
}

/**
***********************************************************
* @brief 按键硬件初始化
* @param
* @return 
***********************************************************
*/
void KeyDrvInit(void)
{
	KeyGpioInit();
	KeyExtiInit();
}

/**
***********************************************************
* @brief EXTI0中断服务函数，对应PA0
* @param
* @return 
***********************************************************
*/
void EXTI0_IRQHandler(void)
{
	if (exti_interrupt_flag_get(EXTI_0) != RESET)
	{
		ToggleLed(LED1);
		exti_interrupt_flag_clear(EXTI_0);
		while (1);
	}
}

/**
***********************************************************
* @brief EXTI10_15中断服务函数，对应PG13 PG14
* @param
* @return 
***********************************************************
*/
void EXTI10_15_IRQHandler(void)
{
	if (exti_interrupt_flag_get(EXTI_13) != RESET)
	{
		ToggleLed(LED2);
		exti_interrupt_flag_clear(EXTI_13);
	}
	
	if (exti_interrupt_flag_get(EXTI_14) != RESET)
	{
		ToggleLed(LED3);
		exti_interrupt_flag_clear(EXTI_14);
	}
}
