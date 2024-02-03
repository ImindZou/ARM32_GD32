#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "gd32f30x.h"

typedef struct
{
	uint32_t uartNo;
	rcu_periph_enum rcuUart;
	rcu_periph_enum rcuGpio;
	uint32_t gpio;
	uint32_t txPin;
	uint32_t rxPin;
	uint8_t irq;
} UartHwInfo_t;

static UartHwInfo_t g_uartHwInfo = {USART1, RCU_USART1, RCU_GPIOA, GPIOA, GPIO_PIN_2, GPIO_PIN_3, USART1_IRQn};


static void GpioInit(void)
{
	rcu_periph_clock_enable(g_uartHwInfo.rcuGpio);
	gpio_init(g_uartHwInfo.gpio, GPIO_MODE_AF_PP, GPIO_OSPEED_10MHZ, g_uartHwInfo.txPin);
	gpio_init(g_uartHwInfo.gpio, GPIO_MODE_IPU, GPIO_OSPEED_10MHZ, g_uartHwInfo.rxPin);
}

static void UartInit(uint32_t baudRate)
{
	/* ʹ��UARTʱ�ӣ�*/
	rcu_periph_clock_enable(g_uartHwInfo.rcuUart);
	/* ��λUART��*/
	usart_deinit (g_uartHwInfo.uartNo);
	/* ͨ��USART_CTL0�Ĵ�����WL�����ֳ���*/ 
	//usart_word_length_set(g_uartHwInfo.uartNo, USART_WL_8BIT);
	/* ͨ��USART_CTL0�Ĵ�����PCEN����У��λ��*/ 
	//usart_parity_config(g_uartHwInfo.uartNo, USART_PM_NONE);
	/* ��USART_CTL1�Ĵ�����дSTB[1:0]λ������ֹͣλ�ĳ��ȣ�*/ 
	//usart_stop_bit_set(g_uartHwInfo.uartNo, USART_STB_1BIT);
	/* ��USART_BAUD�Ĵ��������ò����ʣ�*/ 
	usart_baudrate_set(g_uartHwInfo.uartNo, baudRate);
	/* ��USART_CTL0�Ĵ���������TENλ��ʹ�ܷ��͹��ܣ�*/
	usart_transmit_config(g_uartHwInfo.uartNo, USART_TRANSMIT_ENABLE);
	/* ��USART_CTL0�Ĵ���������TENλ��ʹ�ܽ��չ��ܣ�*/
	usart_receive_config(g_uartHwInfo.uartNo, USART_RECEIVE_ENABLE);

	/* ��USART_CTL0�Ĵ�������λUENλ��ʹ��UART��*/ 
	usart_enable(g_uartHwInfo.uartNo);
}

#define SWITCH_RS485_TO_RX()           gpio_bit_reset(GPIOC, GPIO_PIN_5)
#define SWITCH_RS485_TO_TX()           gpio_bit_set(GPIOC, GPIO_PIN_5)
static void SwitchInit(void)
{
	rcu_periph_clock_enable(RCU_GPIOC);                                 //ʹ��GPIOG��ʱ��
	gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5);  //�������
                                
}

void RS485DrvInit(void)
{
	GpioInit();
    UartInit(9600);
	SwitchInit();	
}

static bool ReceiveByte(uint8_t *key)
{
	if (usart_flag_get(g_uartHwInfo.uartNo, USART_FLAG_RBNE) != RESET)
	{
		*key = (uint8_t) usart_data_receive(g_uartHwInfo.uartNo);
		return true;
	}
	else
	{
		return false;
	}
}

int32_t ReceiveByteTimeout(uint8_t *c, uint32_t timeout)
{
	while (timeout-- > 0)
	{
		if (ReceiveByte(c))
		{
			return 0;
		}
	}
	return -1;
}

/**
  * @brief  Test to see if a key has been pressed on the HyperTerminal
  * @param  key: The key pressed
  * @retval 1: Correct
  *         0: Error
  */
bool GetKeyPressed(uint8_t *key)
{
	return ReceiveByte(key);
}


/**
  * @brief  Print a character on the HyperTerminal
  * @param  c: The character to be printed
  * @retval None
  */
static void SerialPutChar(uint8_t c)
{
	SWITCH_RS485_TO_TX();
    usart_data_transmit(g_uartHwInfo.uartNo, (uint8_t)c);
    while(RESET == usart_flag_get(g_uartHwInfo.uartNo, USART_FLAG_TC));
	SWITCH_RS485_TO_RX();
}

void SendByte(uint8_t c)
{
	SerialPutChar(c);
}

/**
***********************************************************
* @brief printf����Ĭ�ϴ�ӡ�������ʾ�������Ҫ��������ڣ�
		 ��������ʵ��fputc�����������ָ�򴮿ڣ���Ϊ�ض���
* @param
* @return 
***********************************************************
*/
int fputc(int ch, FILE *f)
{
	SWITCH_RS485_TO_TX();
	usart_data_transmit(g_uartHwInfo.uartNo, (uint8_t)ch);
	while (RESET == usart_flag_get(g_uartHwInfo.uartNo, USART_FLAG_TC));
	SWITCH_RS485_TO_RX();
	return ch;
}
