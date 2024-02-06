#ifndef _LCD_DRV_H_
#define _LCD_DRV_H_

#include <stdint.h>

#define LCD_PIXEL_WIDTH       320
#define LCD_PIXEL_HEIGHT      480

#define WHITE                 0xFFFF //��ɫ
#define BLACK                 0x0000 //��ɫ
#define BLUE                  0x001F //��ɫ
#define BRED                  0xF81F //��ɫ
#define GRED                  0xFFE0 //��ɫ
#define GBLUE                 0x07FF //��ɫ
#define RED                   0xF800 //��ɫ
#define MAGENTA               0xF81F //Ʒ��
#define GREEN                 0x07E0 //��ɫ
#define CYAN                  0x7FFF //��ɫ
#define YELLOW                0xFFE0 //��ɫ
#define BROWN                 0xBC40 //��ɫ
#define BRRED                 0xFC07 //�غ�ɫ
#define GRAY                  0x8430 //��ɫ

#define ASCII_FONT_SIZE_12   12
#define ASCII_FONT_SIZE_16   16
#define ASCII_FONT_SIZE_24   24

void LcdDrvInit(void);

/**
***********************************************************
* @brief ��ָ����������䵥����ɫ
* @param (sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:
		 (ex-sx+1)*(ey-sy+1)��color:Ҫ������ɫ
* @return 
***********************************************************
*/
void LcdFillPureColor(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color);

/**
***********************************************************
* @brief ��������
* @param color,Ҫ���������ɫ
* @return 
***********************************************************
*/
void LcdClear(uint16_t color);

/**
***********************************************************
* @brief ��ָ����������������ɫ
* @param (sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:
		 (ex-sx+1)*(ey-sy+1)��color:Ҫ������ɫ
* @return 
***********************************************************
*/
void LcdFillMultiColor(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color);

/**
***********************************************************
* @brief ���ٻ���
* @param x,y:���꣬color:��ɫ
* @return 
***********************************************************
*/
void LcdDrawPoint(uint16_t xPos, uint16_t yPos, uint16_t color);

#endif
