#ifndef _TOUCH_DRV_H_
#define _TOUCH_DRV_H_

#include <stdint.h>
#include <stdbool.h>
#define TOUCH_POINT_MAX             5           // 对于gt911最多5个点的坐标

/* 触摸点坐标数据结构 */
typedef struct
{
    uint16_t x;           // 触摸点X坐标
    uint16_t y;           // 触摸点Y坐标
    uint16_t size;        // 触摸点大小
} TouchPoint_t;

#define UP    0
#define DOWN  1
/* 触摸信息结构体 */
typedef struct
{
	uint8_t state;        // 1：按下 or 0：释放
	TouchPoint_t point;
} TouchInfo_t;	          //触摸信息结构体

void TouchDrvInit(void);
void TouchScan(TouchInfo_t *touchInfo);

#endif
