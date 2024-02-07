#include <stdint.h>
#include <stdio.h>
#include "gd32f30x.h"
#include "delay.h"
#include "lcd_drv.h"
#include "fonts.h"

static void GpioInit(void)
{
	/* 使能RCU相关时钟 */
	rcu_periph_clock_enable(RCU_GPIOD);  //使能GPIOD的时钟
	rcu_periph_clock_enable(RCU_GPIOE);  //使能GPIOE的时钟
	rcu_periph_clock_enable(RCU_GPIOF);  //使能GPIOF的时钟
	rcu_periph_clock_enable(RCU_GPIOG);  //使能GPIOG的时钟

	/* 地址总线 */
	gpio_init(GPIOF, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_0 ); //A0
	gpio_init(GPIOF, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_1 ); //A1
	gpio_init(GPIOF, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_2 ); //A2
	gpio_init(GPIOF, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_3 ); //A3
	gpio_init(GPIOF, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_4 ); //A4
	gpio_init(GPIOF, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_5 ); //A5
	gpio_init(GPIOF, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_12); //A6
	gpio_init(GPIOF, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_13); //A7
	gpio_init(GPIOF, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_14); //A8
	gpio_init(GPIOF, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_15); //A9
	gpio_init(GPIOG, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_0 ); //A10
	gpio_init(GPIOG, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_1 ); //A11
	gpio_init(GPIOG, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_2 ); //A12
	gpio_init(GPIOG, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_3 ); //A13
	gpio_init(GPIOG, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_4 ); //A14
	gpio_init(GPIOG, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_5 ); //A15
	gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_11); //A16
	gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_12); //A17
	gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_13); //A18

	/* 数据总线 */
	gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_14); //A0
	gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_15); //A1
	gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_0 ); //A2
	gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_1 ); //A3
	gpio_init(GPIOE, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_7 ); //A4
	gpio_init(GPIOE, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_8 ); //A5
	gpio_init(GPIOE, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_9 ); //A6
	gpio_init(GPIOE, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_10); //A7
	gpio_init(GPIOE, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_11); //A8
	gpio_init(GPIOE, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_12); //A9
	gpio_init(GPIOE, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_13); //A10
	gpio_init(GPIOE, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_14); //A11
	gpio_init(GPIOE, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_15); //A12
	gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_8 ); //A13
	gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_9 ); //A14
	gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_10); //A15

	/* 控制信号 */
	gpio_init(GPIOG, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_9); //EXMC_NE1
	gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_4); //EXMC_NOE
	gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_5); //EXMC_NWE
}

static void ExmcLcdInit(void)
{
	/* 使能EXMC时钟 */
	rcu_periph_clock_enable(RCU_EXMC);

	/* 读时序配置 */
	exmc_norsram_timing_parameter_struct lcdReadTimingInitStruct;
	lcdReadTimingInitStruct.asyn_access_mode = EXMC_ACCESS_MODE_A; //模式A，异步访问SRAM
	lcdReadTimingInitStruct.asyn_address_setuptime = 0;  //异步访问地址建立时间
	lcdReadTimingInitStruct.asyn_address_holdtime  = 0;  //异步访问地址保持时间
	lcdReadTimingInitStruct.asyn_data_setuptime    = 15; //异步访问数据建立时间
	lcdReadTimingInitStruct.bus_latency            = 0;  //同步/异步访问总线延时时间
	lcdReadTimingInitStruct.syn_clk_division       = 0;  //同步访问时钟分频系数（从HCLK中分频）
	lcdReadTimingInitStruct.syn_data_latency       = 0;  //同步访问中获得第1个数据所需要的等待延迟

	/* 写时序配置 */
	exmc_norsram_timing_parameter_struct lcdWriteTimingInitStruct;
	lcdWriteTimingInitStruct.asyn_access_mode = EXMC_ACCESS_MODE_A; //模式A，异步访问SRAM
	lcdWriteTimingInitStruct.asyn_address_setuptime     = 0; //异步访问地址建立时间
	lcdWriteTimingInitStruct.asyn_address_holdtime      = 0; //异步访问地址保持时间
	lcdWriteTimingInitStruct.asyn_data_setuptime        = 2; //异步访问数据建立时间
	lcdWriteTimingInitStruct.bus_latency                = 0; //同步/异步访问总线延时时间
	lcdWriteTimingInitStruct.syn_clk_division           = 0; //同步访问时钟分频系数（从HCLK中分频）
	lcdWriteTimingInitStruct.syn_data_latency           = 0; //同步访问中获得第1个数据所需要的等待延迟

	
	/* Region1配置 */
	exmc_norsram_parameter_struct lcdInitStruct;
	exmc_norsram_struct_para_init(&lcdInitStruct);
	lcdInitStruct.norsram_region    = EXMC_BANK0_NORSRAM_REGION1; //Region1
	lcdInitStruct.address_data_mux  = DISABLE;                    //禁用地址、数据总线多路复用
	lcdInitStruct.memory_type       = EXMC_MEMORY_TYPE_SRAM;      //储存器类型为SRAM
	lcdInitStruct.databus_width     = EXMC_NOR_DATABUS_WIDTH_16B; //数据宽度16位
	lcdInitStruct.burst_mode        = DISABLE;                    //禁用突发访问
	lcdInitStruct.wrap_burst_mode   = DISABLE;                    //禁用包突发访问
	lcdInitStruct.asyn_wait         = DISABLE;                    //禁用异步等待
	lcdInitStruct.extended_mode     = ENABLE;                     //使能扩展模式
	lcdInitStruct.memory_write      = ENABLE;                     //使能写入外部存储器
	lcdInitStruct.nwait_signal      = DISABLE;                    //禁用等待输入信号
	lcdInitStruct.write_mode        = EXMC_ASYN_WRITE;            //写入模式为异步写入
	lcdInitStruct.read_write_timing = &lcdReadTimingInitStruct;   //读时序配置
	lcdInitStruct.write_timing      = &lcdWriteTimingInitStruct;  //写时序配置

	exmc_norsram_init(&lcdInitStruct);
	exmc_norsram_enable(EXMC_BANK0_NORSRAM_REGION1);
}


#define LCD_PIXEL_WIDTH      320
#define LCD_PIXEL_HEIGHT     480
#define LCD_ID         		 0x9488
#define LCD_X_CMD            0x2A
#define LCD_Y_CMD            0x2B
#define LCD_WG_CMD           0x2C


#define LcdSendCmd(cmdVal) 		(*(volatile uint16_t *)0x64000000) = cmdVal      // 发送命令 	
#define LcdSendData(dataVal) 	(*(volatile uint16_t *)0x64000002) = dataVal     // 发送数据
#define LcdGetData() 			(*(volatile uint16_t *)0x64000002)               // 读取数据

#define LCD_BACKLIGHT_ON 	gpio_bit_set(GPIOA, GPIO_PIN_1)
#define LCD_BACKLIGHT_OFF  	gpio_bit_reset(GPIOA, GPIO_PIN_1)

static void BacklightInit(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_1);
}

static void LcdInit(void)
{
	uint16_t id;
	LcdSendCmd(0xD3);				   
	id = LcdGetData();	    // dummy read 	
	id = LcdGetData();	    // 读到0X00
	id = LcdGetData();   	// 读取94								   
	id <<= 8;
	id |= LcdGetData();  	// 读取88	 
	printf("LCD ID:%x\n", id); 
	
	if (id != LCD_ID)
	{
		printf("LCD init error\n");
		return;
	}

	LcdSendCmd(0XF7);
	LcdSendData(0xA9);
	LcdSendData(0x51);
	LcdSendData(0x2C);
	LcdSendData(0x82);
	LcdSendCmd(0xC0);
	LcdSendData(0x11);
	LcdSendData(0x09);
	LcdSendCmd(0xC1);
	LcdSendData(0x41);
	LcdSendCmd(0XC5);
	LcdSendData(0x00);
	LcdSendData(0x0A);
	LcdSendData(0x80);
	LcdSendCmd(0xB1);
	LcdSendData(0xB0);
	LcdSendData(0x11);
	LcdSendCmd(0xB4);
	LcdSendData(0x02);
	LcdSendCmd(0xB6);
	LcdSendData(0x02);
	LcdSendData(0x22);
	LcdSendCmd(0xB7);
	LcdSendData(0xc6);
	LcdSendCmd(0xBE);
	LcdSendData(0x00);
	LcdSendData(0x04);
	LcdSendCmd(0xE9);
	LcdSendData(0x00);
	LcdSendCmd(0x36);
	LcdSendData(0x08);
	LcdSendCmd(0x3A);
	LcdSendData(0x55);
	LcdSendCmd(0xE0);
	LcdSendData(0x00);
	LcdSendData(0x07);
	LcdSendData(0x10);
	LcdSendData(0x09);
	LcdSendData(0x17);
	LcdSendData(0x0B);
	LcdSendData(0x41);
	LcdSendData(0x89);
	LcdSendData(0x4B);
	LcdSendData(0x0A);
	LcdSendData(0x0C);
	LcdSendData(0x0E);
	LcdSendData(0x18);
	LcdSendData(0x1B);
	LcdSendData(0x0F);
	LcdSendCmd(0XE1);
	LcdSendData(0x00);
	LcdSendData(0x17);
	LcdSendData(0x1A);
	LcdSendData(0x04);
	LcdSendData(0x0E);
	LcdSendData(0x06);
	LcdSendData(0x2F);
	LcdSendData(0x45);
	LcdSendData(0x43);
	LcdSendData(0x02);
	LcdSendData(0x0A);
	LcdSendData(0x09);
	LcdSendData(0x32);
	LcdSendData(0x36);
	LcdSendData(0x0F);
	LcdSendCmd(0x11);
	LcdSendCmd(0x29);	
}

void LcdDrvInit(void)
{
	GpioInit();
	ExmcLcdInit();
	BacklightInit();
	LcdInit();
	LCD_BACKLIGHT_ON;
}

static void LcdSetCursor(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey)
{
	LcdSendCmd(LCD_X_CMD);  // 发送x cmd
	LcdSendData(sx >> 8);
	LcdSendData(sx & 0XFF); 
	LcdSendData(ex >> 8);
	LcdSendData(ex & 0XFF);
	LcdSendCmd(LCD_Y_CMD);  // 发送y cmd
	LcdSendData(sy >> 8);
	LcdSendData(sy & 0XFF);
	LcdSendData(ey >> 8);
	LcdSendData(ey & 0XFF);
}

/**
***********************************************************
* @brief 在指定区域内填充单个颜色
* @param (sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:
		 (ex-sx+1)*(ey-sy+1)，color:要填充的颜色
* @return 
***********************************************************
*/
void LcdFillPureColor(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color)
{
	
	uint32_t totalPoint = (ex - sx + 1) * (ey - sy + 1); // 得到总点数    
	LcdSetCursor(sx, sy, ex, ey);      // 设置显示区域
	LcdSendCmd(LCD_WG_CMD);            // 开始写入GRAM
	for (uint32_t i = 0; i < totalPoint; i++)
	{
		LcdSendData(color);
	}
}

/**
***********************************************************
* @brief 清屏函数
* @param color,要清屏的填充色
* @return 
***********************************************************
*/
void LcdClear(uint16_t color)
{
	LcdFillPureColor(0, 0, LCD_PIXEL_WIDTH - 1, LCD_PIXEL_HEIGHT - 1, color);  
}


/**
***********************************************************
* @brief 在指定区域内填充多种颜色
* @param (sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:
		 (ex-sx+1)*(ey-sy+1)，color:要填充的颜色
* @return 
***********************************************************
*/
void LcdFillMultiColor(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color)
{  
	uint32_t totalPoint = (ex - sx + 1) * (ey - sy + 1); // 得到总点数 
	LcdSetCursor(sx, sy, ex, ey);  		 // 设置显示区域
	LcdSendCmd(LCD_WG_CMD);              // 开始写入GRAM
	for (uint32_t i = 0; i < totalPoint; i++)
	{
		LcdSendData(color[i]);
	}
}

/**
***********************************************************
* @brief 快速画点
* @param x,y:坐标，color:颜色
* @return 
***********************************************************
*/
void LcdDrawPoint(uint16_t xPos, uint16_t yPos, uint16_t color)
{	   
	LcdSetCursor(xPos, yPos, xPos, yPos);
	
	LcdSendCmd(LCD_WG_CMD); 
	LcdSendData(color);	
}	

/**
***********************************************************
* @brief  LCD屏幕显示ASCII字符
* @param  xPos：窗口起点x轴坐标
* 		  yPos：窗口起点y轴坐标
* 		  ascii：要显示的字符
* 		  fontSize：字体选择，参数：ASCII_FONT_SIZE_12，ASCII_FONT_SIZE_16，ASCII_FONT_SIZE_24 
* 		  charColor：选择字符的前景色
* 		  bgColor：选择字符的背景色
* @retval None 
***********************************************************
*/
void LcdShowChar(uint16_t xPos, uint16_t yPos, char ascii, uint16_t fontSize, uint16_t charColor, uint16_t bgColor)
{
	uint16_t arrRowIndex = ascii - ' ';	// 得到字符在二维数组中索引（ASCII字库是从空格开始取模，所以-‘ ’就是对应的字库）
	uint16_t fontWidth = fontSize / 2;
	uint16_t fontHeight = fontSize;
	uint16_t fontHasBytes = (fontWidth / 8 + ((fontWidth % 8) ? 1 : 0)) * fontHeight;	//得到一个字符所占的字数
	
	
	const uint8_t *charArr;
	if (fontSize == ASCII_FONT_SIZE_12)
	{
		charArr = &ascii06x12[arrRowIndex][0];
	}
	else if (fontSize == ASCII_FONT_SIZE_16)
	{
		charArr = &ascii08x16[arrRowIndex][0];
	}
	else if (fontSize == ASCII_FONT_SIZE_24)
	{
		charArr = &ascii12x24[arrRowIndex][0];
	}
	else 
	{
		return;
	}
	
	if ((xPos + fontWidth - 1 > LCD_PIXEL_WIDTH - 1) || (yPos + fontHeight - 1 > LCD_PIXEL_HEIGHT - 1))
	{
		return;
	}
	LcdSetCursor(xPos, yPos, xPos + fontWidth - 1, yPos + fontHeight - 1); //设置显示区域
	LcdSendCmd(LCD_WG_CMD);
	
	uint8_t data = 0;
	uint16_t hasShowPixels = 0;
	for (uint16_t i = 0; i < fontHasBytes; i++)
	{
		data = charArr[i];
		for (uint16_t j = 0; j < 8; j++)
		{
			if (data & 0x80)
			{
				LcdSendData(charColor);
			}
			else
			{
				LcdSendData(bgColor);
			}
			data <<= 1; 
			
			hasShowPixels++;
			
			if (hasShowPixels == fontWidth)	//8位数据中，要过滤掉补齐的位数
			{
				hasShowPixels = 0;
				break;
			}
		}
	}
}	

/**
***********************************************************
* @brief  LCD屏幕显示ASCII字符串
* @param  xPos：窗口起点x轴坐标
* 		  yPos：窗口起点y轴坐标
* 		  str：要显示的字符串
* 		  fontSize：字体选择，参数：ASCII_FONT_SIZE_12，ASCII_FONT_SIZE_16，ASCII_FONT_SIZE_24 
* 		  charColor：选择字符的前景色
* 		  bgColor：选择字符的背景色
* @retval None 
***********************************************************
*/
void LcdShowString(uint16_t xPos, uint16_t yPos, const char *str, uint16_t fontSize, uint16_t charColor, uint16_t bgColor)
{	
	while(*str != '\0')
	{
		if ((xPos + fontSize / 2) > LCD_PIXEL_WIDTH)	 // 如果起始列地址加上字体宽度已经超过了屏幕宽度，则进行换行
		{
			xPos = 0;					                 // 起始列地址置0
			yPos += fontSize;				             // 起始行地址加一个字体高度，让上方的字体显示完整
		}

		if ((yPos + fontSize) > LCD_PIXEL_HEIGHT)		 // 如果起始行地址加上一个字体高度已经超过了屏幕的高度，则返回
		{
			return;
		}

		LcdShowChar(xPos, yPos, *str, fontSize, charColor, bgColor);
		xPos += fontSize / 2;
		str++;
	}
}
