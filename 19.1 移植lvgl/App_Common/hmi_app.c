#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "rtc_drv.h"
#include "sensor_drv.h"
#include "led_drv.h"
#include "key_drv.h"
#include "touch_drv.h"
#include "lcd_drv.h"
#include "store_app.h"

/*********************************************************************************************************
* 函数名称：DrawPoint
* 函数功能：绘制实心圆
* 输入参数：（x0、y0）：圆心坐标，r：半径，color：颜色
* 输出参数：void
* 返 回 值：void
* 创建日期：2021年07月01日
* 注    意：
*********************************************************************************************************/
static void DrawPoint(uint16_t x0,uint16_t y0, uint16_t r, uint16_t color)
{
	int a = 0;
	int b = r;	  
	int di = 3 - (r << 1);             //判断下个点位置的标志
	while (a <= b)
	{
		int i = a, p = b;
		while (i > 0)
		{
			LcdDrawPoint(x0+b,y0-i, color);
			LcdDrawPoint(x0-i,y0+b, color);
			i--;
		}
		while ( p > 0)
		{
			LcdDrawPoint(x0 - a, y0 - p, color);
			LcdDrawPoint(x0 - p, y0 - a, color);
			LcdDrawPoint(x0 + a, y0 - p, color);
			LcdDrawPoint(x0 - p, y0 + a, color);
			LcdDrawPoint(x0 + a, y0 + p, color);
			LcdDrawPoint(x0 + p, y0 + a, color);
			p--;
		}
		a++;

		//Bresenham算法画圆
		if (di < 0)
		{
			di += 4 * a + 6;
		}
		else
		{
			di += 10 + 4 * (a - b);
			b--;
		}
	}
	LcdDrawPoint(x0, y0, color); //圆心坐标
}
/*********************************************************************************************************
* 函数名称：DrawLine
* 函数功能：绘制直线
* 输入参数：（x0、y0）：起点坐标，（x1、y1）：终点坐标，size：直线粗细，color：颜色
* 输出参数：void
* 返 回 值：void
* 创建日期：2021年07月01日
* 注    意：直线太粗会导致绘制缓慢
*********************************************************************************************************/
static void DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t size, uint16_t color)
{
	uint16_t t; 
	int xerr = 0, yerr = 0, delta_x, delta_y, distance; 
	int incx, incy, uRow, uCol; 
	delta_x = x1 - x0; //计算坐标增量 
	delta_y = y1 - y0; 
	uRow = x0; 
	uCol = y0; 

	//设置单步方向 
	if (delta_x > 0)
	{
		incx=1;
	}

	//垂直线
	else if (delta_x == 0)
	{
		incx=0;
	}
	else 
	{
		incx = -1;
		delta_x = -delta_x;
	} 

	if (delta_y > 0)
	{
		incy = 1;
	}

	//水平线
	else if (delta_y == 0)
	{
		incy = 0;
	}
	else
	{
		incy = -1;
		delta_y = -delta_y;
	} 

	//选取基本增量坐标轴
	if(delta_x > delta_y)
	{
		distance = delta_x;
	}
	else 
	{
		distance = delta_y;
	}

	//画线输出
	for(t = 0; t <= distance + 1; t++ )
	{ 
		DrawPoint(uRow, uCol, size, color);
		xerr += delta_x ; 
		yerr += delta_y ; 
		if (xerr > distance) 
		{ 
			xerr -= distance; 
			uRow += incx; 
		} 
		if (yerr > distance) 
		{ 
			yerr -= distance; 
			uCol += incy; 
		}
	}
}

void CanvasTask(void)
{	
	static bool s_lineFlag = false;  // 标记线条是否已开始绘制
	static TouchPoint_t s_arrLastPoint;          // 上一个点的坐标
	
	TouchInfo_t touchInfoNow;
	memset(&touchInfoNow, 0, sizeof(touchInfoNow));
	TouchScan(&touchInfoNow);
	
	if (touchInfoNow.state == UP)
	{
		s_lineFlag = false;
		return;
	}

	uint16_t x0, y0, x1, y1, size;

	/* 获得起点终点坐标、颜色和触点大小 */
	x0 = s_arrLastPoint.x;
	y0 = s_arrLastPoint.y;
	x1 = touchInfoNow.point.x;
	y1 = touchInfoNow.point.y;
	size  = touchInfoNow.point.size;

	/* 触点太大，需要缩小处理 */
	size = size / 5;
	if (0 == size)
	{
		size = 1;
	}
	else if (size > 15)
	{
		size = 15;
	}
  
	
	if (!s_lineFlag)
	{
		DrawPoint(x1, y1, size, YELLOW); // 线条第一个点用画点方式
		s_lineFlag = true;  // 标记线条已经开始绘制
	}
	
	else
	{
		DrawLine(x0, y0, x1, y1, size, YELLOW); // 后边的用画线方式 
	}

	/* 保存当前位置，为画线做准备 */
	s_arrLastPoint.x = touchInfoNow.point.x;
	s_arrLastPoint.y = touchInfoNow.point.y;
}

/**
***********************************************************
* @brief 人机交互任务处理函数
* @param 
* @return 
***********************************************************
*/
void HmiTask(void)
{
	CanvasTask();
	
//	TouchInfo_t touchInfoNow;
//	memset(&touchInfoNow, 0, sizeof(touchInfoNow));
//	TouchScan(&touchInfoNow);
	
	uint8_t keyVal;
	keyVal = GetKeyVal();
	switch (keyVal)
	{
		case KEY1_SHORT_PRESS:
			TurnOnLed(LED1);
			if (SetModbusParam(2))
			{
				printf("SetModbusParam sucess\n");
			}
			else
			{
				printf("SetModbusParam fail\n");
			}
			break;
		case KEY1_LONG_PRESS:
			TurnOffLed(LED1);
			break;
		case KEY2_SHORT_PRESS:
			TurnOnLed(LED2);
			break;
		case KEY2_LONG_PRESS:
			TurnOffLed(LED2);
			break;
		case KEY3_SHORT_PRESS:
			TurnOnLed(LED3);
			break;
		case KEY3_LONG_PRESS:
			TurnOffLed(LED3);
			break;
		default:
			break;
	}
}
