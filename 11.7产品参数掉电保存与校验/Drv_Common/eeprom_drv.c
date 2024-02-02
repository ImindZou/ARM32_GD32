#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "gd32f30x.h"
#include "delay.h"

#define GET_I2C_SDA()             gpio_input_bit_get(GPIOB, GPIO_PIN_7)    // ��ȡSDA�˿�
#define SET_I2C_SCL()             gpio_bit_set(GPIOB, GPIO_PIN_6)          // ʱ����SCL����ߵ�ƽ
#define CLR_I2C_SCL()             gpio_bit_reset(GPIOB, GPIO_PIN_6)        // ʱ����SCL����͵�ƽ
#define SET_I2C_SDA()             gpio_bit_set(GPIOB, GPIO_PIN_7)          // ������SDA����ߵ�ƽ
#define CLR_I2C_SDA()             gpio_bit_reset(GPIOB, GPIO_PIN_7)        // ������SDA����͵�ƽ

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
void I2CSendAck(void)
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
void I2CSendNack(void)
{
	SET_I2C_SDA();          // 2#������SDA����ߵ�ƽ
	DelayNus(4);            // ��ʱ4us
	SET_I2C_SCL();          // 3#ʱ����SCL����ߵ�ƽ����SCL������ǰ��Ҫ��SDA���ߣ�Ϊ��Ӧ���ź�
	DelayNus(4);            // ��ʱ4us
	CLR_I2C_SCL();          // 4#ʱ����SCL����͵�ƽ
	DelayNus(4);            // ��ʱ4us
}

#define EEPROM_DEV_ADDR			  0xA0		// 24xx02���豸��ַ
#define EEPROM_PAGE_SIZE		  8			// 24xx02��ҳ���С
#define EEPROM_SIZE				  256	    // 24xx02������
#define EEPROM_I2C_WR			  0		    // д����bit
#define EEPROM_I2C_RD	          1		    // ������bit

/**
*******************************************************************
* @function ָ����ַ��ʼ����ָ������������
* @param    readAddr,��ȡ��ַ��0~255
* @param    pBuffer,�����׵�ַ
* @param    numToRead,Ҫ���������ݸ���,������256
* @return   
*******************************************************************
*/
bool ReadEepromData(uint8_t readAddr, uint8_t *pBuffer, uint16_t numToRead)
{
	if ((readAddr + numToRead) > EEPROM_SIZE || pBuffer == NULL)
	{
		return false;
	}
	
	I2CStart();                       				  // ������ʼ�ź�  
	I2CSendByte(EEPROM_DEV_ADDR | EEPROM_I2C_WR);      // ����������ַ�Ͷ�дģʽ��1 0 1 0 x  x  x  R/~W  0xA0
	if (!I2CWaitAck())                                 // �ȴ�Ӧ��
	{
		goto i2c_err;
	}
	
	I2CSendByte(readAddr);                             // ���͵�ַ
	if (!I2CWaitAck())                                 // �ȴ�Ӧ��
	{
		goto i2c_err;
	}
				 
	I2CStart();                                        // ������ʼ�ź�          
	I2CSendByte(EEPROM_DEV_ADDR | EEPROM_I2C_RD);      // ����������ַ�Ͷ�дģʽ��1 0 1 0 x  x  x  R/~W  0xA1           
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

/**
*******************************************************************
* @function ָ����ַ��ʼд��ָ������������
* @param    writeAddr,д���ַ��0~255
* @param    pBuffer,�����׵�ַ
* @param    numToWrite,Ҫд������ݸ���,������256
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
		д����EEPROM�������������������ȡ�ܶ��ֽڣ�ÿ��д����ֻ����ͬһ��page��
		����24c02��page size = 8
		�򵥵Ĵ�����Ϊ��Ϊ���������д��Ч�ʣ����������ð�ҳд������������ҳ���·��͵�ַ��
	*/

	for (i = 0; i < numToWrite; i++)
	{
		/* �����͵�1���ֽڻ���ҳ���׵�ַʱ����Ҫ���·��������źź͵�ַ */
		if ((i == 0) || (dataAddr & (EEPROM_PAGE_SIZE - 1)) == 0)
		{
			/*���ڣ�������ֹͣ�źš�*/
			I2CStop();
			
			/* ͨ���������Ӧ��ķ�ʽ���ж��ڲ�д�����Ƿ���� */ 			
			for (j = 0; j < 100; j++)
			{				
				/* ��1��������I2C���������ź� */
				I2CStart();
				
				/* ��2���������豸��ַ�Ϳ���λ */
				I2CSendByte(EEPROM_DEV_ADDR | EEPROM_I2C_WR);	// �˴���дָ��
				
				/* ��3��������һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
				if (I2CWaitAck())
				{
					break;
				}
			}
			if (j  == 100)
			{
				goto i2c_err;	// EEPROM����д��ʱ
			}
			//printf("dataAddr = %d, j = %d\n", dataAddr, j);
			
			
			/* ��4����ÿһҳ���׵�ַ���ߵ�1���ֽڶ�Ҫ�����ֽڵ�ַ */
			I2CSendByte(dataAddr);
			
			/* ��5�����ȴ�ACK */
			if (!I2CWaitAck())
			{
				goto i2c_err;	// EEPROM������Ӧ��
			}
		}
	
		/* ��6������ʼд������ */
		I2CSendByte(pBuffer[i]);
	
		/* ��7��������ACK */
		if (!I2CWaitAck())
		{
			goto i2c_err;	// EEPROM������Ӧ��
		}

		dataAddr++;	// ��ַ��1		
	}
	
	/* ����ִ�гɹ�������I2C����ֹͣ�ź� */
	I2CStop();
	return true;

i2c_err:          // ����ִ��ʧ�ܺ�Ҫ����ֹͣ�źţ�����Ӱ��I2C�����������豸
	/* ����I2C����ֹͣ�ź� */
	I2CStop();
	return false;
}

#define BUFFER_SIZE              255
void EepromDrvTest(void)
{
	uint8_t bufferWrite[BUFFER_SIZE];
    uint8_t bufferRead[BUFFER_SIZE];
	
	printf("AT24C02 writing data��\n");
	for (uint16_t i = 0; i < BUFFER_SIZE; i++)
	{ 
			bufferWrite[i]= i + 1;
			printf("0x%02X ", bufferWrite[i]);
    }
	printf("\n��ʼд��\n");
	
	if (!WriteEepromData(0, bufferWrite, BUFFER_SIZE))
	{
		printf("AT24C02д���ݹ��ϣ����Ų飡\n");
		return;
	}
	
	printf("AT24C02 reading...\n");
	if (!ReadEepromData(0, bufferRead, BUFFER_SIZE))
	{
		printf("AT24C02�����ݹ��ϣ����Ų飡\n");
		return;
	}
	for (uint16_t i = 0; i < BUFFER_SIZE; i++)
	{
        if (bufferRead[i] != bufferWrite[i]){
            printf("0x%02X ", bufferRead[i]);
            printf("AT24C02���Թ��ϣ����Ų飡\n");
            return;
        }
        printf("0x%02X ", bufferRead[i]);

    }
	printf("\n24C02����ͨ����\n");
}
