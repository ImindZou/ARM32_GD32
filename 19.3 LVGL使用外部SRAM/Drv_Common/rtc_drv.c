#include <stdint.h>
#include <time.h>
#include "gd32f30x.h"
#include "rtc_drv.h"

#define MAGIC_CODE    0x5A5A

/**
***********************************************************
* @brief RTC硬件初始化
* @param
* @return 
***********************************************************
*/
void RtcDrvInit(void)
{
	if (bkp_read_data(BKP_DATA_0) != MAGIC_CODE)
	{
		/* 使能对RTC的访问，使能PMU和BKP时钟 */
		rcu_periph_clock_enable(RCU_PMU);
		rcu_periph_clock_enable(RCU_BKPI);
		/* 使能对后备寄存器和RTC的写权限 */
		pmu_backup_write_enable();
		/* 复位后备寄存器 */
		bkp_deinit();
		/* 使能LXTAL，并等待其稳定 */
		rcu_osci_on(RCU_LXTAL);
		rcu_osci_stab_wait(RCU_LXTAL);
		/* 设置RTC时钟源为LXTAL */
		rcu_rtc_clock_config(RCU_RTCSRC_LXTAL);
		/* 使能RTC时钟 */
		rcu_periph_clock_enable(RCU_RTC);
		/* 等待APB1接口时钟和RTC时钟同步 */
		rtc_register_sync_wait();
		/* 等待上次对 RTC 寄存器写操作完成 */
		rtc_lwoff_wait();
		/* 设置分频值32767 */
		rtc_prescaler_set(32767);
		/* 等待上次对 RTC 寄存器写操作完成 */
		rtc_lwoff_wait();
		/* 设置时间2023-02-28 23:59:50 */
		rtc_counter_set(1677599990);
		bkp_write_data(BKP_DATA_0, MAGIC_CODE);
		return;
	}
	/* 使能对RTC的访问，使能PMU和BKP时钟 */
	rcu_periph_clock_enable(RCU_PMU);
	rcu_periph_clock_enable(RCU_BKPI);
	/* 使能对后备寄存器和RTC的写权限 */
	pmu_backup_write_enable();
	/* 等待APB1接口时钟和RTC时钟同步 */
	rtc_register_sync_wait();
	/* 等待上次对 RTC 寄存器写操作完成 */
	rtc_lwoff_wait();
}

/**
***********************************************************
* @brief 设置时间
* @param time,输入，日历时间
* @return 
***********************************************************
*/
void SetRtcTime(RtcTime_t *time)
{
	time_t timeStamp;
	struct tm tmInfo;
	
	tmInfo.tm_year = time->year - 1900;
	tmInfo.tm_mon = time->month - 1;
	tmInfo.tm_mday = time->day;
	tmInfo.tm_hour = time->hour;
	tmInfo.tm_min = time->minute;
	tmInfo.tm_sec = time->second;
	
	timeStamp = mktime(&tmInfo) - 8 * 60 * 60;
	
	rtc_lwoff_wait();
	rtc_counter_set(timeStamp);
}

/**
***********************************************************
* @brief 获取时间
* @param time,输出，日历时间
* @return 
***********************************************************
*/
void GetRtcTime(RtcTime_t *time)
{
	time_t timeStamp;
	struct tm *tmInfo;
	
	timeStamp = rtc_counter_get() + 8 * 60 * 60;
	tmInfo = localtime(&timeStamp);
	time->year = tmInfo->tm_year + 1900;
	time->month = tmInfo->tm_mon + 1;
	time->day = tmInfo->tm_mday;
	time->hour = tmInfo->tm_hour;
	time->minute = tmInfo->tm_min;
	time->second = tmInfo->tm_sec;
}
