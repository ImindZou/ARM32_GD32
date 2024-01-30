/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

#include "port.h"
#include "gd32f30x.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

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

static void (*pProcUartDataFunc)(uint8_t data);   //����ָ�����������Ӧ�ò�ص�������ַ

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
	/* ʹ�ܴ��ڽ����жϣ�*/
	usart_interrupt_enable(g_uartHwInfo.uartNo, USART_INT_RBNE);
	/* ʹ�ܴ����жϣ�*/
	nvic_irq_enable(g_uartHwInfo.irq, 0, 0);
	/* ��USART_CTL0�Ĵ�������λUENλ��ʹ��UART��*/ 
	usart_enable(g_uartHwInfo.uartNo);
}

static void SwitchInit(void)
{
	rcu_periph_clock_enable(RCU_GPIOC);								//ʹ��GPIOC��ʱ��
	gpio_init(GPIOC,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_5);	//�������
}

/* ----------------------- static functions ---------------------------------*/
static void prvvUARTTxReadyISR( void );
static void prvvUARTRxISR( void );

/* ----------------------- Start implementation -----------------------------*/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    /* If xRXEnable enable serial receive interrupts. If xTxENable enable
     * transmitter empty interrupts.
     */
	if(TRUE == xRxEnable)
    {
        usart_interrupt_enable(g_uartHwInfo.uartNo, USART_INT_RBNE);//ʹ�ܽ����ж�
		gpio_bit_reset(GPIOC, GPIO_PIN_5); 
    }
    else
    {
        usart_interrupt_disable(g_uartHwInfo.uartNo, USART_INT_RBNE);//ʧ�ܽ����ж�
		gpio_bit_set(GPIOC, GPIO_PIN_5);   
    }
    
    if(TRUE == xTxEnable)
    {
        usart_interrupt_enable(g_uartHwInfo.uartNo, USART_INT_TC);//ʹ�ܷ����ж�
		gpio_bit_set(GPIOC, GPIO_PIN_5);   
    }
    else
    {
        usart_interrupt_disable(g_uartHwInfo.uartNo, USART_INT_TC);//ʧ�ܷ����ж�
		gpio_bit_reset(GPIOC, GPIO_PIN_5);   
    }	
}

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
	(void)ucPORT;
	(void)ucDataBits;
	(void)eParity;
	SwitchInit();
	GpioInit();
	UartInit(ulBaudRate);
    return TRUE;
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
    /* Put a byte in the UARTs transmit buffer. This function is called
     * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
     * called. */
	usart_data_transmit(g_uartHwInfo.uartNo,ucByte);
    return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
    /* Return the byte in the UARTs receive buffer. This function is called
     * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
     */
	*pucByte = usart_data_receive(g_uartHwInfo.uartNo);
    return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
static void prvvUARTTxReadyISR( void )
{
    pxMBFrameCBTransmitterEmpty(  );
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
static void prvvUARTRxISR( void )
{
    pxMBFrameCBByteReceived(  );
}

void USART1_IRQHandler(void)
{
    //�жϽ��ջ��Ƿ����ж�
    if (RESET != usart_interrupt_flag_get(g_uartHwInfo.uartNo, USART_INT_FLAG_RBNE))
    {
        //��������ж�
        prvvUARTRxISR();
        usart_interrupt_flag_clear(g_uartHwInfo.uartNo, USART_INT_FLAG_RBNE_ORERR);
    }
    if(RESET != usart_interrupt_flag_get(g_uartHwInfo.uartNo, USART_INT_FLAG_TC))
    {
        //��������ж�
        prvvUARTTxReadyISR();
        usart_interrupt_flag_clear(g_uartHwInfo.uartNo, USART_INT_FLAG_TC);
    }
}
