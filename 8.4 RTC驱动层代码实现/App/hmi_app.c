/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-18 09:40:34
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-18 12:04:43
 * @FilePath: \8.4 RTC���������ʵ��\App\hmi_app.c
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdint.h>
#include <stdio.h>
#include "rtc_drv.h"

/**
***********************************************************
* @brief �˻�������������
* @param 
* @return 
***********************************************************
*/
void HmiTask(void)
{
	RtcTime_t rtcTime;
	GetRtcTime(&rtcTime);
	printf("%d-%02d-%02d  %02d:%02d:%02d\n",rtcTime.year,rtcTime.month,rtcTime.day,rtcTime.hour,rtcTime.minute,rtcTime.second);
}


