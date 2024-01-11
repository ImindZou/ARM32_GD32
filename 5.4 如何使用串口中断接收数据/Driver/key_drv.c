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
	/*ʹ��EXTIʱ��*/
	rcu_periph_clock_enable(RCU_AF);
	/*I/O���ӵ�EXTI��*/
	gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOA, GPIO_PIN_SOURCE_0);
	/*��������/�½���*/
	exti_init(EXTI_0, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
	/*�����־*/
	exti_interrupt_flag_clear(EXTI_0);
	/*ʹ���ж�*/
	nvic_irq_enable(EXTI0_IRQn, 1, 1);
	
	/*I/O���ӵ�EXTI��*/
	gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOG, GPIO_PIN_SOURCE_13);
	/*��������/�½���*/
	exti_init(EXTI_13, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
	/*�����־*/
	exti_interrupt_flag_clear(EXTI_13);
	
	/*I/O���ӵ�EXTI��*/
	gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOG, GPIO_PIN_SOURCE_14);
	/*��������/�½���*/
	exti_init(EXTI_14, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
	/*�����־*/
	exti_interrupt_flag_clear(EXTI_14);
	
	/*ʹ���ж�*/
	nvic_irq_enable(EXTI10_15_IRQn, 0, 1);
}

/**
***********************************************************
* @brief ����Ӳ����ʼ��
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
* @brief EXTI0�жϷ���������ӦPA0
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
* @brief EXTI10_15�жϷ���������ӦPG13 PG14
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
