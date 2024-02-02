#include <stdint.h>
#include <stdlib.h>
#include "gd32f30x.h"

static volatile uint64_t g_sysRunTime = 0;

static void (*g_pTaskScheduleFunc)(void);          // ����ָ�����������������ȵĺ�����ַ

/**
***********************************************************
* @brief ע��������Ȼص�����
* @param pFunc, ����ص�������ַ
* @return 
***********************************************************
*/
void TaskScheduleCbReg(void (*pFunc)(void))
{
	g_pTaskScheduleFunc = pFunc;
}

/**
***********************************************************
* @brief systick��ʼ��
* @param
* @return 
***********************************************************
*/
void SystickInit(void)
{
	/*1ms ����һ�ζ�ʱ�ж�*/
	if (SysTick_Config(rcu_clock_freq_get(CK_AHB) / 1000))
	{
		while (1);
	}
}

/**
***********************************************************
* @brief ��ʱ�жϷ�������1ms����һ���ж�
* @param
* @return 
***********************************************************
*/
void SysTick_Handler(void)
{
	g_sysRunTime++;
	if (g_pTaskScheduleFunc == NULL)
	{
		return;
	}
	g_pTaskScheduleFunc();
}

/**
***********************************************************
* @brief ��ȡϵͳ����ʱ��
* @param
* @return ��1msΪ��λ
***********************************************************
*/
uint64_t GetSysRunTime(void)
{
	return g_sysRunTime;
}