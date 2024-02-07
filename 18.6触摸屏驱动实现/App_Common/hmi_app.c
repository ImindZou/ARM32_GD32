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
* �������ƣ�DrawPoint
* �������ܣ�����ʵ��Բ
* �����������x0��y0����Բ�����꣬r���뾶��color����ɫ
* ���������void
* �� �� ֵ��void
* �������ڣ�2021��07��01��
* ע    �⣺
*********************************************************************************************************/
static void DrawPoint(uint16_t x0,uint16_t y0, uint16_t r, uint16_t color)
{
	int a = 0;
	int b = r;	  
	int di = 3 - (r << 1);             //�ж��¸���λ�õı�־
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

		//Bresenham�㷨��Բ
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
	LcdDrawPoint(x0, y0, color); //Բ������
}
/*********************************************************************************************************
* �������ƣ�DrawLine
* �������ܣ�����ֱ��
* �����������x0��y0����������꣬��x1��y1�����յ����꣬size��ֱ�ߴ�ϸ��color����ɫ
* ���������void
* �� �� ֵ��void
* �������ڣ�2021��07��01��
* ע    �⣺ֱ��̫�ֻᵼ�»��ƻ���
*********************************************************************************************************/
static void DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t size, uint16_t color)
{
	uint16_t t; 
	int xerr = 0, yerr = 0, delta_x, delta_y, distance; 
	int incx, incy, uRow, uCol; 
	delta_x = x1 - x0; //������������ 
	delta_y = y1 - y0; 
	uRow = x0; 
	uCol = y0; 

	//���õ������� 
	if (delta_x > 0)
	{
		incx=1;
	}

	//��ֱ��
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

	//ˮƽ��
	else if (delta_y == 0)
	{
		incy = 0;
	}
	else
	{
		incy = -1;
		delta_y = -delta_y;
	} 

	//ѡȡ��������������
	if(delta_x > delta_y)
	{
		distance = delta_x;
	}
	else 
	{
		distance = delta_y;
	}

	//�������
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
	static bool s_lineFlag = false;  // ��������Ƿ��ѿ�ʼ����
	static TouchPoint_t s_arrLastPoint;          // ��һ���������
	
	TouchInfo_t touchInfoNow;
	memset(&touchInfoNow, 0, sizeof(touchInfoNow));
	TouchScan(&touchInfoNow);
	
	if (touchInfoNow.state == UP)
	{
		s_lineFlag = false;
		return;
	}

	uint16_t x0, y0, x1, y1, size;

	/* �������յ����ꡢ��ɫ�ʹ����С */
	x0 = s_arrLastPoint.x;
	y0 = s_arrLastPoint.y;
	x1 = touchInfoNow.point.x;
	y1 = touchInfoNow.point.y;
	size  = touchInfoNow.point.size;

	/* ����̫����Ҫ��С���� */
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
		DrawPoint(x1, y1, size, YELLOW); // ������һ�����û��㷽ʽ
		s_lineFlag = true;  // ��������Ѿ���ʼ����
	}
	
	else
	{
		DrawLine(x0, y0, x1, y1, size, YELLOW); // ��ߵ��û��߷�ʽ 
	}

	/* ���浱ǰλ�ã�Ϊ������׼�� */
	s_arrLastPoint.x = touchInfoNow.point.x;
	s_arrLastPoint.y = touchInfoNow.point.y;
}

/**
***********************************************************
* @brief �˻�������������
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
