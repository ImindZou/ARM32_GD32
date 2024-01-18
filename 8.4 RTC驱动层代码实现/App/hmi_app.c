/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-18 09:40:34
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-18 12:04:43
 * @FilePath: \8.4 RTC驱动层代码实现\App\hmi_app.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
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
	printf("%d-%02d-%02d  %02d:%02d:%02d\n",rtcTime.year,rtcTime.month,rtcTime.day,rtcTime.hour,rtcTime.minute,rtcTime.second);
}


