#include <stdint.h>
#include <stdio.h>
#include "rtc_drv.h"

/**
***********************************************************
* @brief 人机交互任务处理函数
* @param 
* @return 
***********************************************************
*/
void HmiTask(void)
{
	RtcTime_t rtcTime;
	GetRtcTime(&rtcTime);
	printf("%d-%02d-%02d %02d:%02d:%02d\n", rtcTime.year, rtcTime.month, rtcTime.day,
											rtcTime.hour,rtcTime.minute, rtcTime.second);
}
