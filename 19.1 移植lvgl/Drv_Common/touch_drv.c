#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "gd32f30x.h"
#include "delay.h"
#include "touch_drv.h"
#include "systick.h"

#define GET_I2C_SDA()             gpio_input_bit_get(GPIOF, GPIO_PIN_9)    // ��ȡSDA�˿�
#define SET_I2C_SCL()             gpio_bit_set(GPIOF, GPIO_PIN_10)          // ʱ����SCL����ߵ�ƽ
#define CLR_I2C_SCL()             gpio_bit_reset(GPIOF, GPIO_PIN_10)        // ʱ����SCL����͵�ƽ
#define SET_I2C_SDA()             gpio_bit_set(GPIOF, GPIO_PIN_9)          // ������SDA����ߵ�ƽ
#define CLR_I2C_SDA()             gpio_bit_reset(GPIOF, GPIO_PIN_9)        // ������SDA����͵�ƽ

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
* @function ����IIC��ʼʱ��׼�����ͻ��������ǰ��������ʼ���п�ʼ 
* @param
* @return 
* @brief    SCLΪ�ߵ�ƽʱ��SDA�ɸߵ�ƽ��͵�ƽ���䣬��ʼ�������� 
*           ������ͼ��ʾ�Ĳ���ͼ����Ϊ��ʼʱ�� 
*                1 2    3     4   
*                    __________     
*           SCL : __/          \_____ 
*                 ________          
*           SDA :         \___________ 
*******************************************************************
*/
static void I2CStart(void)
{
	SET_I2C_SDA();          // 1#������SDA����ߵ�ƽ
	SET_I2C_SCL();          // 2#ʱ����SCL����ߵ�ƽ   
	DelayNus(4);            // ��ʱ4us
	CLR_I2C_SDA();          // 3#������SDA����͵�ƽ 
	DelayNus(4);            // ��ʱ4us
	CLR_I2C_SCL();          // 4#ʱ����SCL����͵�ƽ������I2C��ʱ����SCLΪ�͵�ƽ��׼�����ͻ�������� 
	DelayNus(4);            // ��ʱ4us
}

/**
*******************************************************************
* @function ����IICֹͣʱ��  
* @param
* @return 
* @brief    SCLΪ�ߵ�ƽʱ��SDA�ɵ͵�ƽ��ߵ�ƽ���䣬������������ 
*          ������ͼ��ʾ�Ĳ���ͼ����Ϊֹͣʱ�� 
*                1 2   3  4   
*                       _______________     
*          SCL : ______/          
*                __        ____________  
*          SDA:    \______/
*******************************************************************
*/
static void I2CStop(void)
{
	CLR_I2C_SDA();          //2#������SDA����͵�ƽ
	DelayNus(4);            //��ʱ4us
	SET_I2C_SCL();          //3#ʱ����SCL����ߵ�ƽ
	DelayNus(4);  
	SET_I2C_SDA();          //4#������SDA����ߵ�ƽ������I2C���߽����ź�
}

/**
*******************************************************************
* @function ����һ�ֽڣ����ݴӸ�λ��ʼ���ͳ�ȥ
* @param    byte
* @return 
* @brief    �����Ǿ����ʱ��ͼ 
*                1 2     3      4
*                         ______
*           SCL: ________/      \______    
*                ______________________    
*           SDA: \\\___________________
*******************************************************************
*/
static void I2CSendByte(uint8_t byte)
{                          
	for (uint8_t i = 0; i < 8; i++)   // ѭ��8�Σ��Ӹߵ���ȡ���ֽڵ�8��λ
	{     
		if ((byte & 0x80))            // 2#ȡ���ֽ����λ�����ж�Ϊ��0�����ǡ�1�����Ӷ�������Ӧ�Ĳ���
		{
			SET_I2C_SDA();            // ������SDA����ߵ�ƽ������λΪ��1��
		}
		else
		{  
			CLR_I2C_SDA();      	  // ������SDA����͵�ƽ������λΪ��0��
		}
		
		byte <<= 1;            		  // ����һλ���θ�λ�Ƶ����λ
		
		DelayNus(4);          		  // ��ʱ4us
		SET_I2C_SCL();                // 3#ʱ����SCL����ߵ�ƽ
		DelayNus(4);          		  // ��ʱ4us
		CLR_I2C_SCL();        		  // 4#ʱ����SCL����͵�ƽ
		DelayNus(4);                  // ��ʱ4us  
	}  
}

/**
*******************************************************************
* @function ��ȡһ�ֽ�����
* @param    
* @return   ��ȡ���ֽ�
* @brief    �����Ǿ����ʱ��ͼ
*                       ______
*           SCL: ______/      \___        
*                ____________________    
*           SDA: \\\\______________\\\
*******************************************************************
*/
static uint8_t I2CReadByte(void)
{
	uint8_t byte = 0;           		// byte������Ž��յ�����
	SET_I2C_SDA();                      // �ͷ�SDA
	for (uint8_t i = 0; i < 8; i++)     // ѭ��8�Σ��Ӹߵ��Ͷ�ȡ�ֽڵ�8��λ
	{
		SET_I2C_SCL();          		// ʱ����SCL����ߵ�ƽ
		DelayNus(4);            		// ��ʱ4us
		byte <<= 1;          			// ����һλ���ճ��µ����λ

		if (GET_I2C_SDA())       		// ��ȡ������SDA������λ
		{
			byte++;            			// ��SCL�������غ������Ѿ��ȶ�����˿���ȡ�����ݣ��������λ
		}
		CLR_I2C_SCL();          		// ʱ����SCL����͵�ƽ
		DelayNus(4);            		// ��ʱ4us
	} 

	return byte;           				// ���ض�ȡ��������
}

/**
*******************************************************************
* @function �ȴ����ն˵�Ӧ���ź�
* @param    
* @return   1������Ӧ��ʧ�ܣ�0������Ӧ��ɹ�
* @brief    ��SDA���ͺ󣬱�ʾ���յ�ACK�źţ�Ȼ������SCL��
*           �˴���ʾ���Ͷ��յ����ն˵�ACK
*                _______|____     
*           SCL:        |    \_________    
*                _______|     
*           SDA:         \_____________ 
*******************************************************************
*/
static bool I2CWaitAck(void)
{
	uint8_t errTimes = 0;
	
	SET_I2C_SDA();             // �ͷ�SDA����,����Ҫ
	DelayNus(4);               // ��ʱ4us
	
	SET_I2C_SCL();             // ʱ����SCL����ߵ�ƽ
	DelayNus(4);               // ��ʱ4us

	while (GET_I2C_SDA())      // ����������������Ǹߵ�ƽ�������ն�û��Ӧ��
	{
		errTimes++;            // ��������1

		if (errTimes > 250)    // �������250�Σ����ж�Ϊ���ն˳��ֹ��ϣ���˷��ͽ����ź�
		{
			I2CStop();         // ����һ��ֹͣ�ź�
			return false;      // ����ֵΪ1����ʾû���յ�Ӧ���ź�
		}
	}

	CLR_I2C_SCL();             // ��ʾ���յ�Ӧ���źţ�ʱ����SCL����͵�ƽ
	DelayNus(4);               // ��ʱ4us
	
	return true;               // ����ֵΪ0����ʾ����Ӧ��ɹ�  
}

/**
*******************************************************************
* @function ����Ӧ���ź�
* @param    
* @return   
* @brief    �����Ǿ����ʱ��ͼ 
*                 1 2     3      4      5     
*                         ______
*           SCL: ________/      \____________    
*                __                     ______
*           SDA:   \___________________/        
*******************************************************************
*/
static void I2CSendAck(void)
{
	CLR_I2C_SDA();          // 2#������SDA����͵�ƽ
	DelayNus(4);            // ��ʱ4us
	SET_I2C_SCL();          // 3#ʱ����SCL����ߵ�ƽ,��SCL������ǰ��Ҫ��SDA���ͣ�ΪӦ���ź�
	DelayNus(4);            // ��ʱ4us
	CLR_I2C_SCL();          // 4#ʱ����SCL����͵�ƽ
	DelayNus(4);            // ��ʱ4us
	SET_I2C_SDA();          // 5#������SDA����ߵ�ƽ���ͷ�SDA����,����Ҫ
}

/**
*******************************************************************
* @function ���ͷ�Ӧ���ź�
* @param    
* @return   
* @brief    �����Ǿ����ʱ��ͼ 
*               1 2     3      4
*                        ______
*          SCL: ________/      \______    
*               __ ___________________    
*          SDA: __/
*******************************************************************
*/
static void I2CSendNack(void)
{
	SET_I2C_SDA();          // 2#������SDA����ߵ�ƽ
	DelayNus(4);            // ��ʱ4us
	SET_I2C_SCL();          // 3#ʱ����SCL����ߵ�ƽ����SCL������ǰ��Ҫ��SDA���ߣ�Ϊ��Ӧ���ź�
	DelayNus(4);            // ��ʱ4us
	CLR_I2C_SCL();          // 4#ʱ����SCL����͵�ƽ
	DelayNus(4);            // ��ʱ4us
}

#define TOUCH_I2C_WR			  	0		    // д����bit
#define TOUCH_I2C_RD	         	1		    // ������bit


/* GT911 ���ּĴ������� */ 
#define TOUCH_DEV_ADDR 	 			0x28
#define GT911_CTRL_REG   			0x8040      // GT911���ƼĴ���
#define GT911_CFGS_REG   			0x8050      // GT911������ʼ��ַ�Ĵ���
#define GT911_PID_REG    			0x8140      // GT911��ƷID�Ĵ���

#define GT911_STATUS_REG 			0x814E      // GT911��ǰ��⵽�Ĵ������
#define GT911_TP1_REG    			0x8150      // ��һ�����������ݵ�ַ
#define GT911_TP2_REG    			0x8158      // �ڶ������������ݵ�ַ
#define GT911_TP3_REG    			0x8160      // ���������������ݵ�ַ
#define GT911_TP4_REG    			0x8168      // ���ĸ����������ݵ�ַ
#define GT911_TP5_REG    			0x8170      // ��������������ݵ�ַ

#define DETECT_INTERVAL_TIME        20          // GT911�����ʱ��Ҫ��10ms����

/**
*******************************************************************
* @function ���ô���оƬ���豸��ַ 0x28/0x29
* @param    
* @return                                                        
*******************************************************************
*/
static void ConfigDevAddr(void)            
{
	gpio_bit_reset(GPIOB, GPIO_PIN_9);    	// RST���ͣ���λGT911
 
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);  	// INT��Ϊ���
	gpio_bit_reset(GPIOB, GPIO_PIN_12);

	DelayNms(1);  	// ��ʱ1����

	gpio_bit_set(GPIOB, GPIO_PIN_12);  	// ����INT

	DelayNms(1);  	// ��ʱ1����

	gpio_bit_set(GPIOB, GPIO_PIN_9);   // RST���ߣ��ͷŸ�λ״̬

	DelayNms(10);  	// ��ʱ10����

	gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_12);  	// INT��Ϊ����
}

/**
*******************************************************************
* @function ����оƬ�ļĴ���д������
* @param    reg,�Ĵ�����ַ
* @param    pBuffer,�����׵�ַ
* @param    numToWrite,Ҫд������ݸ���,������256
* @return   д���Ƿ�ɹ�                                                      
*******************************************************************
*/
static bool WriteTouchReg(uint16_t reg, uint8_t *pBuffer, uint8_t numToWrite)
{
    I2CStart();

    I2CSendByte(TOUCH_DEV_ADDR | TOUCH_I2C_WR);
	if (!I2CWaitAck())
	{
		goto i2c_err;	// ������Ӧ��
	}

    I2CSendByte((uint8_t)(reg >> 8) & 0xFF);
	if (!I2CWaitAck())
	{
		goto i2c_err;	// ������Ӧ��
	}
    I2CSendByte((uint8_t)reg & 0xFF);
    if (!I2CWaitAck())
	{
		goto i2c_err;	// ������Ӧ��
	}

    for (uint8_t i = 0; i < numToWrite; i++)
    {
        I2CSendByte(pBuffer[i]);
        if (!I2CWaitAck())
		{
			goto i2c_err;	// ������Ӧ��
		}
    }

	I2CStop();
	return true;

i2c_err:          // ����ִ��ʧ�ܺ�Ҫ����ֹͣ�źţ�����Ӱ��I2C�����������豸
	I2CStop();
	return false;
}

/**
*******************************************************************
* @function ָ����ַ��ʼ����ָ������������
* @param    readAddr,��ȡ��ַ��0~255
* @param    pBuffer,�����׵�ַ
* @param    numToRead,Ҫ���������ݸ���,������256
* @return   
*******************************************************************
*/
static bool ReadTouchReg(uint16_t reg, uint8_t *pBuffer, uint8_t numToRead)
{	
	I2CStart();                       				  // ������ʼ�ź�
	
	I2CSendByte(TOUCH_DEV_ADDR | TOUCH_I2C_WR);      // ����������ַ�Ͷ�дģʽ
	if (!I2CWaitAck())                                 // �ȴ�Ӧ��
	{
		goto i2c_err;
	}
	
    I2CSendByte((uint8_t)(reg >> 8) & 0xFF);
	if (!I2CWaitAck())
	{
		goto i2c_err;	// ������Ӧ��
	}
    I2CSendByte((uint8_t)reg & 0xFF);
    if (!I2CWaitAck())
	{
		goto i2c_err;	// ������Ӧ��
	}
				 
	I2CStart();                                        // ������ʼ�ź�          
	I2CSendByte(TOUCH_DEV_ADDR | TOUCH_I2C_RD);      // ����������ַ�Ͷ�дģʽ      
	if (!I2CWaitAck())                                 // �ȴ�Ӧ��
	{
		goto i2c_err;
	}
	
	numToRead--;	
	while(numToRead--)                                // ����δ����
	{
		*pBuffer++ = I2CReadByte();                   // ���ֽڶ�����ŵ���������
		I2CSendAck(); 
	}
	*pBuffer = I2CReadByte();                         // ���һ���ֽڷ��ͷ�Ӧ��
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

	if ((statRegVal & 0x80) == 0)  // ��ȡ���λ�鿴�Ƿ��а���
	{
		printf("It has not been touched\n");
		lastTouchInfo.state = UP;
		*touchInfo = lastTouchInfo;
		return;
	}

	uint8_t touchNums = statRegVal & 0x0F;
	printf("It has been toucher, 	touch nums = %d\n", touchNums);
	
	statRegVal = 0;
	WriteTouchReg(GT911_STATUS_REG, &statRegVal, 1);//������ݱ�־λ
	
	if (touchNums == 0 || touchNums > TOUCH_POINT_MAX) 
	{
		lastTouchInfo.state = UP;
		*touchInfo = lastTouchInfo;
		return;
	}
	
	ReadTouchReg(GT911_TP1_REG, buff, 6);	// ����������x y��������
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
	if ((GetSysRunTime() - lastSysTime) < DETECT_INTERVAL_TIME)	//�ж����ε��ýӿں�����ʱ�����Ƿ�С��DETECT_INTERVAL_TIME
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
	
	if ((statRegVal & 0x80) == 0)	//��ȡ���λ�鿴�Ƿ��а���
	{
		printf("It has not been touched\n");
		touchInfo->state = UP;
		lastTouchInfo = *touchInfo;
		return;
	}
	
	uint8_t touchNums = statRegVal & 0x0F;
	printf("touch nums = %d\n", touchNums);
	
	statRegVal = 0;
	WriteTouchReg(GT911_STATUS_REG, &statRegVal, 1); //������ݱ�־λ
	
	if (touchNums == 0 || touchNums > TOUCH_POINT_MAX)
	{
		touchInfo->state = UP;
		lastTouchInfo = *touchInfo;
		return;
	}
	
	ReadTouchReg(GT911_TP1_REG, buff, 6);	//����������x y��������
	touchInfo->point.x = (uint16_t)(buff[1] << 8) | buff[0];
	touchInfo->point.y = (uint16_t)(buff[3] << 8) | buff[2];
	touchInfo->point.size = (uint16_t)(buff[5] << 8) | buff[4];
	//printf("point[%d].x = %d, point[%d],y = %d, point[%d].size = %d\n", \
			0, touchInfo->point.x, 0, touchInfo.point.y, 0, touchInfo.size);
	
	touchInfo->state = DOWN;
	lastTouchInfo = *touchInfo;
	return;
}




