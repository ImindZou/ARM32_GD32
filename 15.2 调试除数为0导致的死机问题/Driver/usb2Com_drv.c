#include <stdint.h>
#include <stdio.h>
#include "gd32f30x.h"

typedef struct
{
	uint32_t uartNo;
	rcu_periph_enum rcuUart;
	rcu_periph_enum rcuGpio;
	uint32_t gpio;
	uint32_t txPin;
	uint32_t rxPin;
} UartHwInfo_t;

static UartHwInfo_t g_uartHwInfo = {USART0, RCU_USART0, RCU_GPIOA, GPIOA, GPIO_PIN_9, GPIO_PIN_10};

static void Usb2ComGpioInit(void)
{
	rcu_periph_clock_enable(g_uartHwInfo.rcuGpio);
	gpio_init(g_uartHwInfo.gpio, GPIO_MODE_AF_PP, GPIO_OSPEED_10MHZ, g_uartHwInfo.txPin);
	gpio_init(g_uartHwInfo.gpio, GPIO_MODE_IPU, GPIO_OSPEED_10MHZ, g_uartHwInfo.rxPin);
}

static void Usb2ComUartInit(uint32_t baudRate)
{
	/* 使能UART时钟；*/
	rcu_periph_clock_enable(g_uartHwInfo.rcuUart);
	/* 复位UART；*/
	usart_deinit (g_uartHwInfo.uartNo);
	/* 通过USART_CTL0寄存器的WL设置字长；*/ 
	//usart_word_length_set(g_uartHwInfo.uartNo, USART_WL_8BIT);
	/* 通过USART_CTL0寄存器的PCEN设置校验位；*/ 
	//usart_parity_config(g_uartHwInfo.uartNo, USART_PM_NONE);
	/* 在USART_CTL1寄存器中写STB[1:0]位来设置停止位的长度；*/ 
	//usart_stop_bit_set(g_uartHwInfo.uartNo, USART_STB_1BIT);
	/* 在USART_BAUD寄存器中设置波特率；*/ 
	usart_baudrate_set(g_uartHwInfo.uartNo, baudRate);
	/* 在USART_CTL0寄存器中设置TEN位，使能发送功能；*/
	usart_transmit_config(g_uartHwInfo.uartNo, USART_TRANSMIT_ENABLE);
	/* 在USART_CTL0寄存器中置位UEN位，使能UART；*/ 
	usart_enable(g_uartHwInfo.uartNo);
}

void Usb2ComTest(void)
{
	for (uint8_t i = 0; i <= 250; i++)
	{
		usart_data_transmit(g_uartHwInfo.uartNo, i);
		while (RESET == usart_flag_get(g_uartHwInfo.uartNo, USART_FLAG_TBE));
	}
//	while (RESET == usart_flag_get(g_uartHwInfo.uartNo, USART_FLAG_TC));
//	usart_transmit_config(g_uartHwInfo.uartNo, USART_TRANSMIT_DISABLE);
}

/**
***********************************************************
* @brief USB转串口硬件初始化
* @param
* @return 
***********************************************************
*/
void Usb2ComDrvInit(void)
{
	Usb2ComGpioInit();
	Usb2ComUartInit(115200);
}

/**
***********************************************************
* @brief printf函数默认打印输出到显示器，如果要输出到串口，
		 必须重新实现fputc函数，将输出指向串口，称为重定向
* @param
* @return 
***********************************************************
*/
int fputc(int ch, FILE *f)
{
	usart_data_transmit(g_uartHwInfo.uartNo, (uint8_t)ch);
	while (RESET == usart_flag_get(g_uartHwInfo.uartNo, USART_FLAG_TBE));
	return ch;
}
