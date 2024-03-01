#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "gd32f30x.h"
#include "delay.h"
#include "touch_drv.h"
#include "systick.h"

#define GET_I2C_SDA()             gpio_input_bit_get(GPIOF, GPIO_PIN_9)    // 读取SDA端口
#define SET_I2C_SCL()             gpio_bit_set(GPIOF, GPIO_PIN_10)          // 时钟线SCL输出高电平
#define CLR_I2C_SCL()             gpio_bit_reset(GPIOF, GPIO_PIN_10)        // 时钟线SCL输出低电平
#define SET_I2C_SDA()             gpio_bit_set(GPIOF, GPIO_PIN_9)          // 数据线SDA输出高电平
#define CLR_I2C_SDA()             gpio_bit_reset(GPIOF, GPIO_PIN_9)        // 数据线SDA输出低电平

static void GpioInit(void)
{
	rcu_periph_clock_enable(RCU_GPIOF);
    gpio_init(GPIOF, GPIO_MODE_OUT_OD, GPIO_OSPEED_10MHZ, GPIO_PIN_9 | GPIO_PIN_10);  // SDA SCL
	
	rcu_periph_clock_enable(RCU_GPIOB);
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_9);  // RST
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_12); // INT
}


/**
*******************************************************************
* @function 产生IIC起始时序，准备发送或接收数据前必须由起始序列开始 
* @param
* @return 
* @brief    SCL为高电平时，SDA由高电平向低电平跳变，开始传输数据 
*           生成下图所示的波形图，即为起始时序 
*                1 2    3     4   
*                    __________     
*           SCL : __/          \_____ 
*                 ________          
*           SDA :         \___________ 
*******************************************************************
*/
static void I2CStart(void)
{
	SET_I2C_SDA();          // 1#数据线SDA输出高电平
	SET_I2C_SCL();          // 2#时钟线SCL输出高电平   
	DelayNus(4);            // 延时4us
	CLR_I2C_SDA();          // 3#数据线SDA输出低电平 
	DelayNus(4);            // 延时4us
	CLR_I2C_SCL();          // 4#时钟线SCL输出低电平，保持I2C的时钟线SCL为低电平，准备发送或接收数据 
	DelayNus(4);            // 延时4us
}

/**
*******************************************************************
* @function 产生IIC停止时序  
* @param
* @return 
* @brief    SCL为高电平时，SDA由低电平向高电平跳变，结束传输数据 
*          生成下图所示的波形图，即为停止时序 
*                1 2   3  4   
*                       _______________     
*          SCL : ______/          
*                __        ____________  
*          SDA:    \______/
*******************************************************************
*/
static void I2CStop(void)
{
	CLR_I2C_SDA();          //2#数据线SDA输出低电平
	DelayNus(4);            //延时4us
	SET_I2C_SCL();          //3#时钟线SCL输出高电平
	DelayNus(4);  
	SET_I2C_SDA();          //4#数据线SDA输出高电平，发送I2C总线结束信号
}

/**
*******************************************************************
* @function 发送一字节，数据从高位开始发送出去
* @param    byte
* @return 
* @brief    下面是具体的时序图 
*                1 2     3      4
*                         ______
*           SCL: ________/      \______    
*                ______________________    
*           SDA: \\\___________________
*******************************************************************
*/
static void I2CSendByte(uint8_t byte)
{                          
	for (uint8_t i = 0; i < 8; i++)   // 循环8次，从高到低取出字节的8个位
	{     
		if ((byte & 0x80))            // 2#取出字节最高位，并判断为‘0’还是‘1’，从而做出相应的操作
		{
			SET_I2C_SDA();            // 数据线SDA输出高电平，数据位为‘1’
		}
		else
		{  
			CLR_I2C_SDA();      	  // 数据线SDA输出低电平，数据位为‘0’
		}
		
		byte <<= 1;            		  // 左移一位，次高位移到最高位
		
		DelayNus(4);          		  // 延时4us
		SET_I2C_SCL();                // 3#时钟线SCL输出高电平
		DelayNus(4);          		  // 延时4us
		CLR_I2C_SCL();        		  // 4#时钟线SCL输出低电平
		DelayNus(4);                  // 延时4us  
	}  
}

/**
*******************************************************************
* @function 读取一字节数据
* @param    
* @return   读取的字节
* @brief    下面是具体的时序图
*                       ______
*           SCL: ______/      \___        
*                ____________________    
*           SDA: \\\\______________\\\
*******************************************************************
*/
static uint8_t I2CReadByte(void)
{
	uint8_t byte = 0;           		// byte用来存放接收的数据
	SET_I2C_SDA();                      // 释放SDA
	for (uint8_t i = 0; i < 8; i++)     // 循环8次，从高到低读取字节的8个位
	{
		SET_I2C_SCL();          		// 时钟线SCL输出高电平
		DelayNus(4);            		// 延时4us
		byte <<= 1;          			// 左移一位，空出新的最低位

		if (GET_I2C_SDA())       		// 读取数据线SDA的数据位
		{
			byte++;            			// 在SCL的上升沿后，数据已经稳定，因此可以取该数据，存入最低位
		}
		CLR_I2C_SCL();          		// 时钟线SCL输出低电平
		DelayNus(4);            		// 延时4us
	} 

	return byte;           				// 返回读取到的数据
}

/**
*******************************************************************
* @function 等待接收端的应答信号
* @param    
* @return   1，接收应答失败；0，接收应答成功
* @brief    当SDA拉低后，表示接收到ACK信号，然后，拉低SCL，
*           此处表示发送端收到接收端的ACK
*                _______|____     
*           SCL:        |    \_________    
*                _______|     
*           SDA:         \_____________ 
*******************************************************************
*/
static bool I2CWaitAck(void)
{
	uint8_t errTimes = 0;
	
	SET_I2C_SDA();             // 释放SDA总线,很重要
	DelayNus(4);               // 延时4us
	
	SET_I2C_SCL();             // 时钟线SCL输出高电平
	DelayNus(4);               // 延时4us

	while (GET_I2C_SDA())      // 读回来的数据如果是高电平，即接收端没有应答
	{
		errTimes++;            // 计数器加1

		if (errTimes > 250)    // 如果超过250次，则判断为接收端出现故障，因此发送结束信号
		{
			I2CStop();         // 产生一个停止信号
			return false;      // 返回值为1，表示没有收到应答信号
		}
	}

	CLR_I2C_SCL();             // 表示已收到应答信号，时钟线SCL输出低电平
	DelayNus(4);               // 延时4us
	
	return true;               // 返回值为0，表示接收应答成功  
}

/**
*******************************************************************
* @function 发送应答信号
* @param    
* @return   
* @brief    下面是具体的时序图 
*                 1 2     3      4      5     
*                         ______
*           SCL: ________/      \____________    
*                __                     ______
*           SDA:   \___________________/        
*******************************************************************
*/
static void I2CSendAck(void)
{
	CLR_I2C_SDA();          // 2#数据线SDA输出低电平
	DelayNus(4);            // 延时4us
	SET_I2C_SCL();          // 3#时钟线SCL输出高电平,在SCL上升沿前就要把SDA拉低，为应答信号
	DelayNus(4);            // 延时4us
	CLR_I2C_SCL();          // 4#时钟线SCL输出低电平
	DelayNus(4);            // 延时4us
	SET_I2C_SDA();          // 5#数据线SDA输出高电平，释放SDA总线,很重要
}

/**
*******************************************************************
* @function 发送非应答信号
* @param    
* @return   
* @brief    下面是具体的时序图 
*               1 2     3      4
*                        ______
*          SCL: ________/      \______    
*               __ ___________________    
*          SDA: __/
*******************************************************************
*/
static void I2CSendNack(void)
{
	SET_I2C_SDA();          // 2#数据线SDA输出高电平
	DelayNus(4);            // 延时4us
	SET_I2C_SCL();          // 3#时钟线SCL输出高电平，在SCL上升沿前就要把SDA拉高，为非应答信号
	DelayNus(4);            // 延时4us
	CLR_I2C_SCL();          // 4#时钟线SCL输出低电平
	DelayNus(4);            // 延时4us
}

#define TOUCH_I2C_WR			  	0		    // 写控制bit
#define TOUCH_I2C_RD	         	1		    // 读控制bit


/* GT911 部分寄存器定义 */ 
#define TOUCH_DEV_ADDR 	 			0x28
#define GT911_CTRL_REG   			0x8040      // GT911控制寄存器
#define GT911_CFGS_REG   			0x8050      // GT911配置起始地址寄存器
#define GT911_PID_REG    			0x8140      // GT911产品ID寄存器

#define GT911_STATUS_REG 			0x814E      // GT911当前检测到的触摸情况
#define GT911_TP1_REG    			0x8150      // 第一个触摸点数据地址
#define GT911_TP2_REG    			0x8158      // 第二个触摸点数据地址
#define GT911_TP3_REG    			0x8160      // 第三个触摸点数据地址
#define GT911_TP4_REG    			0x8168      // 第四个触摸点数据地址
#define GT911_TP5_REG    			0x8170      // 第五个触摸点数据地址

#define DETECT_INTERVAL_TIME        20          // GT911检测间隔时间要求10ms以上

/**
*******************************************************************
* @function 配置触摸芯片的设备地址 0x28/0x29
* @param    
* @return                                                        
*******************************************************************
*/
static void ConfigDevAddr(void)            
{
	gpio_bit_reset(GPIOB, GPIO_PIN_9);    	// RST拉低，复位GT911
 
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);  	// INT设为输出
	gpio_bit_reset(GPIOB, GPIO_PIN_12);

	DelayNms(1);  	// 延时1毫秒

	gpio_bit_set(GPIOB, GPIO_PIN_12);  	// 拉高INT

	DelayNms(1);  	// 延时1毫秒

	gpio_bit_set(GPIOB, GPIO_PIN_9);   // RST拉高，释放复位状态

	DelayNms(10);  	// 延时10毫秒

	gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_12);  	// INT设为输入
}

/**
*******************************************************************
* @function 向触摸芯片的寄存器写入数据
* @param    reg,寄存器地址
* @param    pBuffer,数组首地址
* @param    numToWrite,要写入的数据个数,不大于256
* @return   写入是否成功                                                      
*******************************************************************
*/
static bool WriteTouchReg(uint16_t reg, uint8_t *pBuffer, uint8_t numToWrite)
{
    I2CStart();

    I2CSendByte(TOUCH_DEV_ADDR | TOUCH_I2C_WR);
	if (!I2CWaitAck())
	{
		goto i2c_err;	// 器件无应答
	}

    I2CSendByte((uint8_t)(reg >> 8) & 0xFF);
	if (!I2CWaitAck())
	{
		goto i2c_err;	// 器件无应答
	}
    I2CSendByte((uint8_t)reg & 0xFF);
    if (!I2CWaitAck())
	{
		goto i2c_err;	// 器件无应答
	}

    for (uint8_t i = 0; i < numToWrite; i++)
    {
        I2CSendByte(pBuffer[i]);
        if (!I2CWaitAck())
		{
			goto i2c_err;	// 器件无应答
		}
    }

	I2CStop();
	return true;

i2c_err:          // 命令执行失败后，要发送停止信号，避免影响I2C总线上其他设备
	I2CStop();
	return false;
}

/**
*******************************************************************
* @function 指定地址开始读出指定个数的数据
* @param    readAddr,读取地址，0~255
* @param    pBuffer,数组首地址
* @param    numToRead,要读出的数据个数,不大于256
* @return   
*******************************************************************
*/
static bool ReadTouchReg(uint16_t reg, uint8_t *pBuffer, uint8_t numToRead)
{	
	I2CStart();                       				  // 发送起始信号
	
	I2CSendByte(TOUCH_DEV_ADDR | TOUCH_I2C_WR);      // 发送器件地址和读写模式
	if (!I2CWaitAck())                                 // 等待应答
	{
		goto i2c_err;
	}
	
    I2CSendByte((uint8_t)(reg >> 8) & 0xFF);
	if (!I2CWaitAck())
	{
		goto i2c_err;	// 器件无应答
	}
    I2CSendByte((uint8_t)reg & 0xFF);
    if (!I2CWaitAck())
	{
		goto i2c_err;	// 器件无应答
	}
				 
	I2CStart();                                        // 发送起始信号          
	I2CSendByte(TOUCH_DEV_ADDR | TOUCH_I2C_RD);      // 发送器件地址和读写模式      
	if (!I2CWaitAck())                                 // 等待应答
	{
		goto i2c_err;
	}
	
	numToRead--;	
	while(numToRead--)                                // 数据未读完
	{
		*pBuffer++ = I2CReadByte();                   // 逐字节读出存放到数据数组
		I2CSendAck(); 
	}
	*pBuffer = I2CReadByte();                         // 最后一个字节发送非应答
	I2CSendNack();  
	
	I2CStop(); 
	return true;
	
i2c_err:
	I2CStop(); 
	return false;
}

void TouchDrvInit(void)
{
	GpioInit();
	ConfigDevAddr();
	
	uint8_t id[5];
	if (ReadTouchReg(GT911_PID_REG, id, 4))
	{
		id[4] = 0;
		printf("Touch ID: %s\n", id);
		return;
	}
	printf("Touch init error\n");
}
#if 0
void TouchScan(TouchInfo_t *touchInfo)
{
	static TouchInfo_t lastTouchInfo = {UP};
	static uint64_t lastSysTime = 0;
	if ((GetSysRunTime() - lastSysTime) < DETECT_INTERVAL_TIME)
	{
		*touchInfo = lastTouchInfo;
		return;
	}
	lastSysTime = GetSysRunTime();

	uint8_t statRegVal;
	uint8_t buff[6];
		
	if (!ReadTouchReg(GT911_STATUS_REG, &statRegVal, 1))
	{
		printf("read GT911_STATUS_REG error\n");
		lastTouchInfo.state = UP;
		*touchInfo = lastTouchInfo;
		return;
	}

	if ((statRegVal & 0x80) == 0)  // 读取最高位查看是否有按下
	{
		printf("It has not been touched\n");
		lastTouchInfo.state = UP;
		*touchInfo = lastTouchInfo;
		return;
	}

	uint8_t touchNums = statRegVal & 0x0F;
	printf("It has been toucher, 	touch nums = %d\n", touchNums);
	
	statRegVal = 0;
	WriteTouchReg(GT911_STATUS_REG, &statRegVal, 1);//清除数据标志位
	
	if (touchNums == 0 || touchNums > TOUCH_POINT_MAX) 
	{
		lastTouchInfo.state = UP;
		*touchInfo = lastTouchInfo;
		return;
	}
	
	ReadTouchReg(GT911_TP1_REG, buff, 6);	// 读出触摸点x y坐标和面积
	touchInfo->point.x = (uint16_t)(buff[1] << 8) | buff[0];
	touchInfo->point.y = (uint16_t)(buff[3] << 8) | buff[2];
	touchInfo->point.size = (uint16_t)(buff[5] << 8) | buff[4];
	//printf("point[%d].x = %d, point[%d].y = %d, point[%d].size = %d\n", \
			0, touchInfo->point.x, 0, touchInfo->point.y, 0, touchInfo->point.size);
		
	touchInfo->state = DOWN;
	lastTouchInfo = *touchInfo;
	return;
}
#endif


void TouchScan(TouchInfo_t *touchInfo)
{
	static TouchInfo_t lastTouchInfo = {UP};
	static uint64_t lastSysTime = 0;
	if ((GetSysRunTime() - lastSysTime) < DETECT_INTERVAL_TIME)	//判断两次调用接口函数的时间间隔是否小于DETECT_INTERVAL_TIME
	{
		*touchInfo = lastTouchInfo;
		return;
	}
	lastSysTime = GetSysRunTime();
	
	uint8_t statRegVal;
	uint8_t buff[6];
	
	if (!ReadTouchReg(GT911_STATUS_REG, &statRegVal, 1))
	{
		printf("read GT911_STATUS_REG error\n");
		touchInfo->state = UP;
		lastTouchInfo = *touchInfo;
		return;
	}
	
	if ((statRegVal & 0x80) == 0)	//读取最高位查看是否有按下
	{
		printf("It has not been touched\n");
		touchInfo->state = UP;
		lastTouchInfo = *touchInfo;
		return;
	}
	
	uint8_t touchNums = statRegVal & 0x0F;
	printf("touch nums = %d\n", touchNums);
	
	statRegVal = 0;
	WriteTouchReg(GT911_STATUS_REG, &statRegVal, 1); //清除数据标志位
	
	if (touchNums == 0 || touchNums > TOUCH_POINT_MAX)
	{
		touchInfo->state = UP;
		lastTouchInfo = *touchInfo;
		return;
	}
	
	ReadTouchReg(GT911_TP1_REG, buff, 6);	//读出触摸点x y坐标和面积
	touchInfo->point.x = (uint16_t)(buff[1] << 8) | buff[0];
	touchInfo->point.y = (uint16_t)(buff[3] << 8) | buff[2];
	touchInfo->point.size = (uint16_t)(buff[5] << 8) | buff[4];
	//printf("point[%d].x = %d, point[%d],y = %d, point[%d].size = %d\n", \
			0, touchInfo->point.x, 0, touchInfo.point.y, 0, touchInfo.size);
	
	touchInfo->state = DOWN;
	lastTouchInfo = *touchInfo;
	return;
}




