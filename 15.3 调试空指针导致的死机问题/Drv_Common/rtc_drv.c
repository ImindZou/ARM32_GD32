#include <stdint.h>
#include <time.h>
#include "gd32f30x.h"
#include "rtc_drv.h"

#define MAGIC_CODE    0x5A5A

/**
***********************************************************
* @brief RTCӲ����ʼ��
* @param
* @return 
***********************************************************
*/
void RtcDrvInit(void)
{
	if (bkp_read_data(BKP_DATA_0) != MAGIC_CODE)
	{
		/* ʹ�ܶ�RTC�ķ��ʣ�ʹ��PMU��BKPʱ�� */
		rcu_periph_clock_enable(RCU_PMU);
		rcu_periph_clock_enable(RCU_BKPI);
		/* ʹ�ܶԺ󱸼Ĵ�����RTC��дȨ�� */
		pmu_backup_write_enable();
		/* ��λ�󱸼Ĵ��� */
		bkp_deinit();
		/* ʹ��LXTAL�����ȴ����ȶ� */
		rcu_osci_on(RCU_LXTAL);
		rcu_osci_stab_wait(RCU_LXTAL);
		/* ����RTCʱ��ԴΪLXTAL */
		rcu_rtc_clock_config(RCU_RTCSRC_LXTAL);
		/* ʹ��RTCʱ�� */
		rcu_periph_clock_enable(RCU_RTC);
		/* �ȴ�APB1�ӿ�ʱ�Ӻ�RTCʱ��ͬ�� */
		rtc_register_sync_wait();
		/* �ȴ��ϴζ� RTC �Ĵ���д������� */
		rtc_lwoff_wait();
		/* ���÷�Ƶֵ32767 */
		rtc_prescaler_set(32767);
		/* �ȴ��ϴζ� RTC �Ĵ���д������� */
		rtc_lwoff_wait();
		/* ����ʱ��2023-02-28 23:59:50 */
		rtc_counter_set(1677599990);
		bkp_write_data(BKP_DATA_0, MAGIC_CODE);
		return;
	}
	/* ʹ�ܶ�RTC�ķ��ʣ�ʹ��PMU��BKPʱ�� */
	rcu_periph_clock_enable(RCU_PMU);
	rcu_periph_clock_enable(RCU_BKPI);
	/* ʹ�ܶԺ󱸼Ĵ�����RTC��дȨ�� */
	pmu_backup_write_enable();
	/* �ȴ�APB1�ӿ�ʱ�Ӻ�RTCʱ��ͬ�� */
	rtc_register_sync_wait();
	/* �ȴ��ϴζ� RTC �Ĵ���д������� */
	rtc_lwoff_wait();
}

/**
***********************************************************
* @brief ����ʱ��
* @param time,���룬����ʱ��
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
* @brief ��ȡʱ��
* @param time,���������ʱ��
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
