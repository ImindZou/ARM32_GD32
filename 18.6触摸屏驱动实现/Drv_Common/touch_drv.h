#ifndef _TOUCH_DRV_H_
#define _TOUCH_DRV_H_

#include <stdint.h>
#include <stdbool.h>
#define TOUCH_POINT_MAX             5           // ����gt911���5���������

/* �������������ݽṹ */
typedef struct
{
    uint16_t x;           // ������X����
    uint16_t y;           // ������Y����
    uint16_t size;        // �������С
} TouchPoint_t;

#define UP    0
#define DOWN  1
/* ������Ϣ�ṹ�� */
typedef struct
{
	uint8_t state;        // 1������ or 0���ͷ�
	TouchPoint_t point;
} TouchInfo_t;	          //������Ϣ�ṹ��

void TouchDrvInit(void);
void TouchScan(TouchInfo_t *touchInfo);

#endif
