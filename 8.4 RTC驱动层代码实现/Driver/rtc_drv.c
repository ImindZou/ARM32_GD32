/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-18 11:14:49
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-18 12:00:07
 * @FilePath: \8.4 RTC���������ʵ��\Driver\rtc_drv.c
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-18 11:14:49
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-18 11:30:01
 * @FilePath: \8.4 RTC���������ʵ��\Driver\rtc_drv.c
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdint.h>
#include <time.h>
#include "gd32f30x.h"
#include "rtc_drv.h"
#include <string.h>

#define MAGIC_CODE 0x5a5a

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
        /*ʹ�ܶ�RTC�ķ��ʣ�ʹ��RMU��BKPʱ��*/
        rcu_periph_clock_enable(RCU_PMU);
        rcu_periph_clock_enable(RCU_BKPI);
        /*ʹ�ܶԺ󱳼Ĵ�����RTC��дȨ��*/
        pmu_backup_write_enable();
        /*��λ�󱸼Ĵ���*/
        bkp_deinit();
        /*ʹ��LXAL�����ȴ����ȶ�*/
        rcu_osci_on(RCU_LXTAL);
        rcu_osci_stab_wait(RCU_LXTAL);
        /*����RTCʱ��ԴΪLXTAL*/
        rcu_rtc_clock_config(RCU_RTCSRC_LXTAL);
        /*ʹ��RTCʱ��*/
        rcu_periph_clock_enable(RCU_RTC);
        /*�ȴ�APB1�ӿ�ʱ�Ӻ�RTCʱ��ͬ��*/
        rtc_register_sync_wait();
        /*�ȴ��ϴζ�RTC�Ĵ���д�������*/
        rtc_lwoff_wait();
        /*���÷�ƵֵΪ72767*/
        rtc_prescaler_set(32767);
        /*�ȴ����ζ�RTC�Ĵ���д�������*/
        rtc_lwoff_wait();
        /*����ʱ��2024-01-18 12:18:17*/
        rtc_counter_set(1705551497);
        bkp_write_data(BKP_DATA_0, MAGIC_CODE);
        return;
    }
    /*ʹ�ܶ�RTC�ķ��ʣ�ʹ��RMU��BKPʱ��*/
    rcu_periph_clock_enable(RCU_PMU);
    rcu_periph_clock_enable(RCU_BKPI);
    /*ʹ�ܶԺ󱳼Ĵ�����RTC��дȨ��*/
    pmu_backup_write_enable();
    /*�ȴ�APB1�ӿ�ʱ�Ӻ�RTCʱ��ͬ��*/
    rtc_register_sync_wait();
    /*�ȴ��ϴζ�RTC�Ĵ���д�������*/
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
    memset(&tmInfo, 0, sizeof(tmInfo));
	
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


















