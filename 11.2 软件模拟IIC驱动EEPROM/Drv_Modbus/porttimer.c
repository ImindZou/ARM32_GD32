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
    /* 使能定时器时钟；*/
	rcu_periph_clock_enable(RCU_TIMER3);
	/* 复位定时器；*/
	timer_deinit(TIMER3);
	
	timer_parameter_struct timerInitPara;
	timer_struct_para_init(&timerInitPara);
	/* 设置预分频器值；*/ 
	timerInitPara.prescaler = 5999;   // 频率120MHZ / 6000 = 20khz,对应周期50us  
	/* 设置自动重装载值；*/ 
	timerInitPara.period = usTim1Timerout50us - 1;
	timer_init(TIMER3, &timerInitPara);

	/* 使能定时器的计数更新中断；*/
	timer_interrupt_enable(TIMER3, TIMER_INT_UP);
	/* 使能定时器中断和优先级；*/
	nvic_irq_enable(TIMER3_IRQn, 0, 0);
	/* 使能定时器；*/ 
	//timer_enable(TIMER3);
	return TRUE;
}


inline void
vMBPortTimersEnable(  )
{
    /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
	timer_counter_value_config(TIMER3, 0);//清零定时器
    timer_interrupt_flag_clear(TIMER3, TIMER_INT_UP);//清除定时器更新中断
    timer_interrupt_enable(TIMER3, TIMER_INT_UP);//使能定时器更新中断
    timer_enable(TIMER3);//使能定时器
}

inline void
vMBPortTimersDisable(  )
{
    /* Disable any pending timers. */
	timer_counter_value_config(TIMER3,0);//清零定时器
    timer_interrupt_flag_clear(TIMER3, TIMER_INT_UP);//清除定时器更新中断
    timer_interrupt_disable(TIMER3, TIMER_INT_UP);//禁用定时器更新中断
    timer_disable(TIMER3);//失能定时器
}

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
static void prvvTIMERExpiredISR( void )
{
    ( void )pxMBPortCBTimerExpired(  );
}

void TIMER3_IRQHandler(void)
{
	if (timer_interrupt_flag_get(TIMER3, TIMER_INT_FLAG_UP) == SET)
	{
		timer_interrupt_flag_clear(TIMER3, TIMER_INT_FLAG_UP);
		timer_counter_value_config(TIMER3, 0);
		prvvTIMERExpiredISR();
	}
}

