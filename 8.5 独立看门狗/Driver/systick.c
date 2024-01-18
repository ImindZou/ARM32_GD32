/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-14 00:34:46
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-14 00:59:30
 * @FilePath: \6.2 �������������ȿ��ʵ��\Driver\systick.c
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdint.h>
#include "gd32f30x.h"

static uint64_t g_sysRunTime = 0;

static void (*g_pTaskScheduleFunc)(void);	//����ָ�����������������ȵĺ�����ַ

/**
***********************************************************
* @brief ע��������Ȼص�����
* @param pFunc������ص�������ַ
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
