/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-09 16:57:50
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-09 21:53:46
 * @FilePath: \2.11 ��ˮ�Ƶ�����ܹ��ʹ���ʵ��\App\main.c
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/**
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-09 15:38:06
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-09 16:38:54
 * @FilePath: \2.10 �����ں�DWTʵ��Ӳ����ʱ\App\main.c
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdint.h>
#include "gd32f30x.h"
#include "delay.h"
#include "led_drv.h"
#include "key_drv.h"

int main(void)
{
	LedDrvInit(); 
	KeyDrvInit();
	while (1)
	{
		
	}
}
