/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-18 14:48:27
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-18 14:58:02
 * @FilePath: \8.4 RTC���������ʵ��d:\Study\ARM32��Ƭ��ʵս�γ�\06�Լ�����\8.5 �������Ź�\Driver\wdg_drv.c
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdint.h>
#include "gd32f30x.h"

/**
***********************************************************
* @brief �������Ź���ʼ������
* @param
* @return 
***********************************************************
*/
void WdgDrvInit(void)
{
	/*�ر�FWDGT_PSC��FWDGT_RLD��д����*/
	/*����Ԥ��Ƶ������װ�ؼĴ���*/
    fwdgt_config(2500,FWDGT_PSC_DIV32);     //��Ƶ��40KHz / 32 = 1.25KHz������0.8ms��2500 * 0.8 = 2000ms
	/*�������Ź�*/
    fwdgt_enable();


}

/**
***********************************************************
* @brief ι�� 
* @param
* @return 
***********************************************************
*/
void FeedDog(void)
{
    fwdgt_counter_reload();
}















