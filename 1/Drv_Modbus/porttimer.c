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

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
#include "gd32f30x.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/
static void prvvTIMERExpiredISR( void );

/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
    /* ʹ�ܶ�ʱ��ʱ�ӣ�*/
	rcu_periph_clock_enable(RCU_TIMER3);
	/* ��λ��ʱ����*/
	timer_deinit(TIMER3);
	
	timer_parameter_struct timerInitPara;
	timer_struct_para_init(&timerInitPara);
	/* ����Ԥ��Ƶ��ֵ��*/ 
	timerInitPara.prescaler = 5999;   // Ƶ��120MHZ / 6000 = 20khz,��Ӧ����50us  
	/* �����Զ���װ��ֵ��*/ 
	timerInitPara.period = usTim1Timerout50us - 1;
	timer_init(TIMER3, &timerInitPara);

	/* ʹ�ܶ�ʱ���ļ��������жϣ�*/
	timer_interrupt_enable(TIMER3, TIMER_INT_UP);
	/* ʹ�ܶ�ʱ���жϺ����ȼ���*/
	nvic_irq_enable(TIMER3_IRQn, 0, 0);
	/* ʹ�ܶ�ʱ����*/ 
	//timer_enable(TIMER3);
	return TRUE;
}


inline void
vMBPortTimersEnable(  )
{
    /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
}

inline void
vMBPortTimersDisable(  )
{
    /* Disable any pending timers. */
}

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
static void prvvTIMERExpiredISR( void )
{
    ( void )pxMBPortCBTimerExpired(  );
}
