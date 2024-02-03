#ifndef _RTC_DRV_H_
#define _RTC_DRV_H_

#include <stdint.h>

typedef struct {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} RtcTime_t;

/**
***********************************************************
* @brief RTCӲ����ʼ��
* @param
* @return 
***********************************************************
*/
void RtcDrvInit(void);

/**
***********************************************************
* @brief ����ʱ��
* @param time,���룬����ʱ��
* @return 
***********************************************************
*/
void SetRtcTime(RtcTime_t *time);

/**
***********************************************************
* @brief ��ȡʱ��
* @param time,���������ʱ��
* @return 
***********************************************************
*/
void GetRtcTime(RtcTime_t *time);

#endif

