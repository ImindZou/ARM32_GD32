#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "gd32f30x.h"
#include "delay.h"

#define GET_I2C_SDA()             gpio_input_bit_get(GPIOB, GPIO_PIN_7)    // 读取SDA端口
#define SET_I2C_SCL()             gpio_bit_set(GPIOB, GPIO_PIN_6)          // 时钟线SCL输出高电平
#define CLR_I2C_SCL()             gpio_bit_reset(GPIOB, GPIO_PIN_6)        // 时钟线SCL输出低电平
#define SET_I2C_SDA()             gpio_bit_set(GPIOB, GPIO_PIN_7)          // 数据线SDA输出高电平
#define CLR_I2C_SDA()             gpio_bit_reset(GPIOB, GPIO_PIN_7)        // 数据线SDA输出低电平

static void GpioInit(void)
{
	rcu_periph_clock_enable(RCU_GPIOB);
    gpio_init(GPIOB, GPIO_MODE_OUT_OD, GPIO_OSPEED_10MHZ, GPIO_PIN_6 | GPIO_PIN_7);
}

void EepromDrvInit(void)
{
	GpioInit();
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
void I2CSendAck(void)
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
void I2CSendNack(void)
{
	SET_I2C_SDA();          // 2#数据线SDA输出高电平
	DelayNus(4);            // 延时4us
	SET_I2C_SCL();          // 3#时钟线SCL输出高电平，在SCL上升沿前就要把SDA拉高，为非应答信号
	DelayNus(4);            // 延时4us
	CLR_I2C_SCL();          // 4#时钟线SCL输出低电平
	DelayNus(4);            // 延时4us
}

#define EEPROM_DEV_ADDR			  0xA0		// 24xx02的设备地址
#define EEPROM_PAGE_SIZE		  8			// 24xx02的页面大小
#define EEPROM_SIZE				  256	    // 24xx02总容量
#define EEPROM_I2C_WR			  0		    // 写控制bit
#define EEPROM_I2C_RD	          1		    // 读控制bit

/**
*******************************************************************
* @function 指定地址开始读出指定个数的数据
* @param    readAddr,读取地址，0~255
* @param    pBuffer,数组首地址
* @param    numToRead,要读出的数据个数,不大于256
* @return   
*******************************************************************
*/
bool ReadEepromData(uint8_t readAddr, uint8_t *pBuffer, uint16_t numToRead)
{
	if ((readAddr + numToRead) > EEPROM_SIZE || pBuffer == NULL)
	{
		return false;
	}
	
	I2CStart();                       				  // 发送起始信号  
	I2CSendByte(EEPROM_DEV_ADDR | EEPROM_I2C_WR);      // 发送器件地址和读写模式，1 0 1 0 x  x  x  R/~W  0xA0
	if (!I2CWaitAck())                                 // 等待应答
	{
		goto i2c_err;
	}
	
	I2CSendByte(readAddr);                             // 发送地址
	if (!I2CWaitAck())                                 // 等待应答
	{
		goto i2c_err;
	}
				 
	I2CStart();                                        // 发送起始信号          
	I2CSendByte(EEPROM_DEV_ADDR | EEPROM_I2C_RD);      // 发送器件地址和读写模式，1 0 1 0 x  x  x  R/~W  0xA1           
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

/**
*******************************************************************
* @function 指定地址开始写入指定个数的数据
* @param    writeAddr,写入地址，0~255
* @param    pBuffer,数组首地址
* @param    numToWrite,要写入的数据个数,不大于256
* @return                                                         
*******************************************************************
*/
bool WriteEepromData(uint8_t writeAddr, uint8_t *pBuffer, uint16_t numToWrite)
{
	if ((writeAddr + numToWrite) > EEPROM_SIZE || pBuffer == NULL)
	{
		return false;
	}
	
	uint16_t i, j;
	uint8_t dataAddr  = writeAddr;
	
	/* 
		写串行EEPROM不像读操作可以连续读取很多字节，每次写操作只能在同一个page；
		对于24c02，page size = 8
		简单的处理方法为：为了提高连续写的效率，本函数采用按页写操作，对于新页重新发送地址。
	*/

	for (i = 0; i < numToWrite; i++)
	{
		/* 当发送第1个字节或是页面首地址时，需要重新发起启动信号和地址 */
		if ((i == 0) || (dataAddr & (EEPROM_PAGE_SIZE - 1)) == 0)
		{
			/*　第０步：发停止信号　*/
			I2CStop();
			
			/* 通过检查器件应答的方式，判断内部写操作是否完成 */ 			
			for (j = 0; j < 100; j++)
			{				
				/* 第1步：发起I2C总线启动信号 */
				I2CStart();
				
				/* 第2步：发送设备地址和控制位 */
				I2CSendByte(EEPROM_DEV_ADDR | EEPROM_I2C_WR);	// 此处是写指令
				
				/* 第3步：发送一个时钟，判断器件是否正确应答 */
				if (I2CWaitAck())
				{
					break;
				}
			}
			if (j  == 100)
			{
				goto i2c_err;	// EEPROM器件写超时
			}
			//printf("dataAddr = %d, j = %d\n", dataAddr, j);
			
			
			/* 第4步：每一页的首地址或者第1个字节都要发送字节地址 */
			I2CSendByte(dataAddr);
			
			/* 第5步：等待ACK */
			if (!I2CWaitAck())
			{
				goto i2c_err;	// EEPROM器件无应答
			}
		}
	
		/* 第6步：开始写入数据 */
		I2CSendByte(pBuffer[i]);
	
		/* 第7步：发送ACK */
		if (!I2CWaitAck())
		{
			goto i2c_err;	// EEPROM器件无应答
		}

		dataAddr++;	// 地址增1		
	}
	
	/* 命令执行成功，发送I2C总线停止信号 */
	I2CStop();
	return true;

i2c_err:          // 命令执行失败后，要发送停止信号，避免影响I2C总线上其他设备
	/* 发送I2C总线停止信号 */
	I2CStop();
	return false;
}

#define BUFFER_SIZE              255
void EepromDrvTest(void)
{
	uint8_t bufferWrite[BUFFER_SIZE];
    uint8_t bufferRead[BUFFER_SIZE];
	
	printf("AT24C02 writing data：\n");
	for (uint16_t i = 0; i < BUFFER_SIZE; i++)
	{ 
			bufferWrite[i]= i + 1;
			printf("0x%02X ", bufferWrite[i]);
    }
	printf("\n开始写入\n");
	
	if (!WriteEepromData(0, bufferWrite, BUFFER_SIZE))
	{
		printf("AT24C02写数据故障，请排查！\n");
		return;
	}
	
	printf("AT24C02 reading...\n");
	if (!ReadEepromData(0, bufferRead, BUFFER_SIZE))
	{
		printf("AT24C02读数据故障，请排查！\n");
		return;
	}
	for (uint16_t i = 0; i < BUFFER_SIZE; i++)
	{
        if (bufferRead[i] != bufferWrite[i]){
            printf("0x%02X ", bufferRead[i]);
            printf("AT24C02测试故障，请排查！\n");
            return;
        }
        printf("0x%02X ", bufferRead[i]);

    }
	printf("\n24C02测试通过！\n");
}
