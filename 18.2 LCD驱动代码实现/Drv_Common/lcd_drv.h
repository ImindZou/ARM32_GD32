#ifndef _LCD_DRV_H_
#define _LCD_DRV_H_

#include <stdint.h>

#define LCD_PIXEL_WIDTH       320
#define LCD_PIXEL_HEIGHT      480

#define WHITE                 0xFFFF //白色
#define BLACK                 0x0000 //黑色
#define BLUE                  0x001F //蓝色
#define BRED                  0xF81F //红色
#define GRED                  0xFFE0 //红色
#define GBLUE                 0x07FF //蓝色
#define RED                   0xF800 //红色
#define MAGENTA               0xF81F //品红
#define GREEN                 0x07E0 //绿色
#define CYAN                  0x7FFF //青色
#define YELLOW                0xFFE0 //黄色
#define BROWN                 0xBC40 //棕色
#define BRRED                 0xFC07 //棕红色
#define GRAY                  0x8430 //灰色

#define ASCII_FONT_SIZE_12   12
#define ASCII_FONT_SIZE_16   16
#define ASCII_FONT_SIZE_24   24

void LcdDrvInit(void);

/**
***********************************************************
* @brief 在指定区域内填充单个颜色
* @param (sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:
		 (ex-sx+1)*(ey-sy+1)，color:要填充的颜色
* @return 
***********************************************************
*/
void LcdFillPureColor(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color);

/**
***********************************************************
* @brief 清屏函数
* @param color,要清屏的填充色
* @return 
***********************************************************
*/
void LcdClear(uint16_t color);

/**
***********************************************************
* @brief 在指定区域内填充多种颜色
* @param (sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:
		 (ex-sx+1)*(ey-sy+1)，color:要填充的颜色
* @return 
***********************************************************
*/
void LcdFillMultiColor(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color);

/**
***********************************************************
* @brief 快速画点
* @param x,y:坐标，color:颜色
* @return 
***********************************************************
*/
void LcdDrawPoint(uint16_t xPos, uint16_t yPos, uint16_t color);

#endif
