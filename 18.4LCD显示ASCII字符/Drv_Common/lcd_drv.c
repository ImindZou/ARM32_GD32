#include <stdint.h>
#include <stdio.h>
#include "gd32f30x.h"
#include "delay.h"
#include "lcd_drv.h"
#include "fonts.h"

static void GpioInit(void)
{
	/* ʹ��RCU���ʱ�� */
	rcu_periph_clock_enable(RCU_GPIOD);  //ʹ��GPIOD��ʱ��
	rcu_periph_clock_enable(RCU_GPIOE);  //ʹ��GPIOE��ʱ��
	rcu_periph_clock_enable(RCU_GPIOF);  //ʹ��GPIOF��ʱ��
	rcu_periph_clock_enable(RCU_GPIOG);  //ʹ��GPIOG��ʱ��

	/* ��ַ���� */
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

	/* �������� */
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

	/* �����ź� */
	gpio_init(GPIOG, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_9); //EXMC_NE1
	gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_4); //EXMC_NOE
	gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_5); //EXMC_NWE
}

static void ExmcLcdInit(void)
{
	/* ʹ��EXMCʱ�� */
	rcu_periph_clock_enable(RCU_EXMC);

	/* ��ʱ������ */
	exmc_norsram_timing_parameter_struct lcdReadTimingInitStruct;
	lcdReadTimingInitStruct.asyn_access_mode = EXMC_ACCESS_MODE_A; //ģʽA���첽����SRAM
	lcdReadTimingInitStruct.asyn_address_setuptime = 0;  //�첽���ʵ�ַ����ʱ��
	lcdReadTimingInitStruct.asyn_address_holdtime  = 0;  //�첽���ʵ�ַ����ʱ��
	lcdReadTimingInitStruct.asyn_data_setuptime    = 15; //�첽�������ݽ���ʱ��
	lcdReadTimingInitStruct.bus_latency            = 0;  //ͬ��/�첽����������ʱʱ��
	lcdReadTimingInitStruct.syn_clk_division       = 0;  //ͬ������ʱ�ӷ�Ƶϵ������HCLK�з�Ƶ��
	lcdReadTimingInitStruct.syn_data_latency       = 0;  //ͬ�������л�õ�1����������Ҫ�ĵȴ��ӳ�

	/* дʱ������ */
	exmc_norsram_timing_parameter_struct lcdWriteTimingInitStruct;
	lcdWriteTimingInitStruct.asyn_access_mode = EXMC_ACCESS_MODE_A; //ģʽA���첽����SRAM
	lcdWriteTimingInitStruct.asyn_address_setuptime     = 0; //�첽���ʵ�ַ����ʱ��
	lcdWriteTimingInitStruct.asyn_address_holdtime      = 0; //�첽���ʵ�ַ����ʱ��
	lcdWriteTimingInitStruct.asyn_data_setuptime        = 2; //�첽�������ݽ���ʱ��
	lcdWriteTimingInitStruct.bus_latency                = 0; //ͬ��/�첽����������ʱʱ��
	lcdWriteTimingInitStruct.syn_clk_division           = 0; //ͬ������ʱ�ӷ�Ƶϵ������HCLK�з�Ƶ��
	lcdWriteTimingInitStruct.syn_data_latency           = 0; //ͬ�������л�õ�1����������Ҫ�ĵȴ��ӳ�

	
	/* Region1���� */
	exmc_norsram_parameter_struct lcdInitStruct;
	exmc_norsram_struct_para_init(&lcdInitStruct);
	lcdInitStruct.norsram_region    = EXMC_BANK0_NORSRAM_REGION1; //Region1
	lcdInitStruct.address_data_mux  = DISABLE;                    //���õ�ַ���������߶�·����
	lcdInitStruct.memory_type       = EXMC_MEMORY_TYPE_SRAM;      //����������ΪSRAM
	lcdInitStruct.databus_width     = EXMC_NOR_DATABUS_WIDTH_16B; //���ݿ��16λ
	lcdInitStruct.burst_mode        = DISABLE;                    //����ͻ������
	lcdInitStruct.wrap_burst_mode   = DISABLE;                    //���ð�ͻ������
	lcdInitStruct.asyn_wait         = DISABLE;                    //�����첽�ȴ�
	lcdInitStruct.extended_mode     = ENABLE;                     //ʹ����չģʽ
	lcdInitStruct.memory_write      = ENABLE;                     //ʹ��д���ⲿ�洢��
	lcdInitStruct.nwait_signal      = DISABLE;                    //���õȴ������ź�
	lcdInitStruct.write_mode        = EXMC_ASYN_WRITE;            //д��ģʽΪ�첽д��
	lcdInitStruct.read_write_timing = &lcdReadTimingInitStruct;   //��ʱ������
	lcdInitStruct.write_timing      = &lcdWriteTimingInitStruct;  //дʱ������

	exmc_norsram_init(&lcdInitStruct);
	exmc_norsram_enable(EXMC_BANK0_NORSRAM_REGION1);
}


#define LCD_PIXEL_WIDTH      320
#define LCD_PIXEL_HEIGHT     480
#define LCD_ID         		 0x9488
#define LCD_X_CMD            0x2A
#define LCD_Y_CMD            0x2B
#define LCD_WG_CMD           0x2C


#define LcdSendCmd(cmdVal) 		(*(volatile uint16_t *)0x64000000) = cmdVal      // �������� 	
#define LcdSendData(dataVal) 	(*(volatile uint16_t *)0x64000002) = dataVal     // ��������
#define LcdGetData() 			(*(volatile uint16_t *)0x64000002)               // ��ȡ����

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
	id = LcdGetData();	    // ����0X00
	id = LcdGetData();   	// ��ȡ94								   
	id <<= 8;
	id |= LcdGetData();  	// ��ȡ88	 
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
	LcdSendCmd(LCD_X_CMD);  // ����x cmd
	LcdSendData(sx >> 8);
	LcdSendData(sx & 0XFF); 
	LcdSendData(ex >> 8);
	LcdSendData(ex & 0XFF);
	LcdSendCmd(LCD_Y_CMD);  // ����y cmd
	LcdSendData(sy >> 8);
	LcdSendData(sy & 0XFF);
	LcdSendData(ey >> 8);
	LcdSendData(ey & 0XFF);
}

/**
***********************************************************
* @brief ��ָ����������䵥����ɫ
* @param (sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:
		 (ex-sx+1)*(ey-sy+1)��color:Ҫ������ɫ
* @return 
***********************************************************
*/
void LcdFillPureColor(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color)
{
	
	uint32_t totalPoint = (ex - sx + 1) * (ey - sy + 1); // �õ��ܵ���    
	LcdSetCursor(sx, sy, ex, ey);      // ������ʾ����
	LcdSendCmd(LCD_WG_CMD);            // ��ʼд��GRAM
	for (uint32_t i = 0; i < totalPoint; i++)
	{
		LcdSendData(color);
	}
}

/**
***********************************************************
* @brief ��������
* @param color,Ҫ���������ɫ
* @return 
***********************************************************
*/
void LcdClear(uint16_t color)
{
	LcdFillPureColor(0, 0, LCD_PIXEL_WIDTH - 1, LCD_PIXEL_HEIGHT - 1, color);  
}


/**
***********************************************************
* @brief ��ָ����������������ɫ
* @param (sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:
		 (ex-sx+1)*(ey-sy+1)��color:Ҫ������ɫ
* @return 
***********************************************************
*/
void LcdFillMultiColor(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color)
{  
	uint32_t totalPoint = (ex - sx + 1) * (ey - sy + 1); // �õ��ܵ��� 
	LcdSetCursor(sx, sy, ex, ey);  		 // ������ʾ����
	LcdSendCmd(LCD_WG_CMD);              // ��ʼд��GRAM
	for (uint32_t i = 0; i < totalPoint; i++)
	{
		LcdSendData(color[i]);
	}
}

/**
***********************************************************
* @brief ���ٻ���
* @param x,y:���꣬color:��ɫ
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
* @brief  LCD��Ļ��ʾASCII�ַ�
* @param  xPos���������x������
* 		  yPos���������y������
* 		  ascii��Ҫ��ʾ���ַ�
* 		  fontSize������ѡ�񣬲�����ASCII_FONT_SIZE_12��ASCII_FONT_SIZE_16��ASCII_FONT_SIZE_24 
* 		  charColor��ѡ���ַ���ǰ��ɫ
* 		  bgColor��ѡ���ַ��ı���ɫ
* @retval None 
***********************************************************
*/
void LcdShowChar(uint16_t xPos, uint16_t yPos, char ascii, uint16_t fontSize, uint16_t charColor, uint16_t bgColor)
{
	uint16_t arrRowIndex = ascii - ' ';	// �õ��ַ��ڶ�ά������������ASCII�ֿ��Ǵӿո�ʼȡģ������-�� �����Ƕ�Ӧ���ֿ⣩
	uint16_t fontWidth = fontSize / 2;
	uint16_t fontHeight = fontSize;
	uint16_t fontHasBytes = (fontWidth / 8 + ((fontWidth % 8) ? 1 : 0)) * fontHeight;	//�õ�һ���ַ���ռ������
	
	
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
	LcdSetCursor(xPos, yPos, xPos + fontWidth - 1, yPos + fontHeight - 1); //������ʾ����
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
			
			if (hasShowPixels == fontWidth)	//8λ�����У�Ҫ���˵������λ��
			{
				hasShowPixels = 0;
				break;
			}
		}
	}
}	

/**
***********************************************************
* @brief  LCD��Ļ��ʾASCII�ַ���
* @param  xPos���������x������
* 		  yPos���������y������
* 		  str��Ҫ��ʾ���ַ���
* 		  fontSize������ѡ�񣬲�����ASCII_FONT_SIZE_12��ASCII_FONT_SIZE_16��ASCII_FONT_SIZE_24 
* 		  charColor��ѡ���ַ���ǰ��ɫ
* 		  bgColor��ѡ���ַ��ı���ɫ
* @retval None 
***********************************************************
*/
void LcdShowString(uint16_t xPos, uint16_t yPos, const char *str, uint16_t fontSize, uint16_t charColor, uint16_t bgColor)
{	
	while(*str != '\0')
	{
		if ((xPos + fontSize / 2) > LCD_PIXEL_WIDTH)	 // �����ʼ�е�ַ�����������Ѿ���������Ļ��ȣ�����л���
		{
			xPos = 0;					                 // ��ʼ�е�ַ��0
			yPos += fontSize;				             // ��ʼ�е�ַ��һ������߶ȣ����Ϸ���������ʾ����
		}

		if ((yPos + fontSize) > LCD_PIXEL_HEIGHT)		 // �����ʼ�е�ַ����һ������߶��Ѿ���������Ļ�ĸ߶ȣ��򷵻�
		{
			return;
		}

		LcdShowChar(xPos, yPos, *str, fontSize, charColor, bgColor);
		xPos += fontSize / 2;
		str++;
	}
}
