#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "gd32f30x.h"
#include "delay.h"

#define SET_SPI_NSS()            gpio_bit_set(GPIOE, GPIO_PIN_2)
#define CLR_SPI_NSS()            gpio_bit_reset(GPIOE, GPIO_PIN_2)

#define SET_SPI_SCK()            gpio_bit_set(GPIOB, GPIO_PIN_13)  
#define CLR_SPI_SCK()            gpio_bit_reset(GPIOB, GPIO_PIN_13)

#define READ_SPI_MISO()          gpio_input_bit_get(GPIOB, GPIO_PIN_14) 

#define SET_SPI_MOSI()           gpio_bit_set(GPIOB, GPIO_PIN_15)
#define CLR_SPI_MOSI()           gpio_bit_reset(GPIOB, GPIO_PIN_15)


static void GpioInit(void)
{
	/* PE2��ΪƬѡSS */
	rcu_periph_clock_enable(RCU_GPIOE);                                           //ʹ��GPIOE��ʱ��
	gpio_init(GPIOE, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);            //����GPIO���ģʽ���ٶ�
	gpio_bit_set(GPIOE, GPIO_PIN_2);                                              //��PE2Ĭ��״̬����Ϊ�ߵ�ƽ

	/* PB13��ΪSCK */
	rcu_periph_clock_enable(RCU_GPIOB);                                           //ʹ��GPIOB��ʱ��
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);           //����GPIO���ģʽ���ٶ�
	gpio_bit_set(GPIOB, GPIO_PIN_13);                                             //��PB13Ĭ��״̬����Ϊ�ߵ�ƽ
	
	/* PB14��ΪMISO */
	gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_14);      //����PB14Ϊ����ģʽ
	
	/* PB15��ΪMOSI */
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_15);           //����GPIO���ģʽ���ٶ�
	gpio_bit_set(GPIOB, GPIO_PIN_15);                                             //��PB15Ĭ��״̬����Ϊ�ߵ�ƽ
}

static void SPIInit(void)
{
	GpioInit();

	SET_SPI_NSS();
	CLR_SPI_SCK();
}

static void SPIStart(void)
{
	CLR_SPI_NSS();
}

static void SPIStop(void)
{
	SET_SPI_NSS();
}

static uint8_t SPIReadWriteByte(uint8_t sendData)
{
	uint8_t  revData = 0;

	for (uint8_t i = 0; i < 8; i++)
	{
		if ((sendData & 0x80))
		{
			SET_SPI_MOSI();
		}
		else
		{  
			CLR_SPI_MOSI();
		}
		sendData <<= 1;
		SET_SPI_SCK();
		revData <<= 1;
		revData |= READ_SPI_MISO();
		CLR_SPI_SCK();
	}
  
	return revData;
}

#define DUMMY_BYTE							        0xFF

#define NORFLASH_WRITE_ENABLE						0x06
#define NORFLASH_WRITE_DISABLE						0x04
#define NORFLASH_READ_STATUS_REGISTER_1				0x05
#define NORFLASH_READ_STATUS_REGISTER_2				0x35
#define NORFLASH_WRITE_STATUS_REGISTER				0x01
#define NORFLASH_PAGE_PROGRAM						0x02
#define NORFLASH_QUAD_PAGE_PROGRAM					0x32
#define NORFLASH_BLOCK_ERASE_64KB					0xD8
#define NORFLASH_BLOCK_ERASE_32KB					0x52
#define NORFLASH_SECTOR_ERASE_4KB					0x20
#define NORFLASH_CHIP_ERASE							0xC7
#define NORFLASH_ERASE_SUSPEND						0x75
#define NORFLASH_ERASE_RESUME						0x7A
#define NORFLASH_POWER_DOWN							0xB9
#define NORFLASH_HIGH_PERFORMANCE_MODE				0xA3
#define NORFLASH_CONTINUOUS_READ_MODE_RESET			0xFF
#define NORFLASH_RELEASE_POWER_DOWN_HPM_DEVICE_ID	0xAB
#define NORFLASH_MANUFACTURER_DEVICE_ID				0x90
#define NORFLASH_READ_UNIQUE_ID						0x4B
#define NORFLASH_JEDEC_ID							0x9F
#define NORFLASH_READ_DATA							0x03
#define NORFLASH_FAST_READ							0x0B
#define NORFLASH_FAST_READ_DUAL_OUTPUT				0x3B
#define NORFLASH_FAST_READ_DUAL_IO					0xBB
#define NORFLASH_FAST_READ_QUAD_OUTPUT				0x6B
#define NORFLASH_FAST_READ_QUAD_IO					0xEB
#define NORFLASH_OCTAL_WORD_READ_QUAD_IO			0xE3


#define NORFLASH_PAGE_SIZE    						256                            //һҳ�Ĵ�С��256�ֽ�
#define NORFLASH_SECTOR_SIZE 						(4 * 1024)                      //������С���ֽ�
#define NORFLASH_BLOCK_SIZE  						(16 * NORFLASH_SECTOR_SIZE)
#define NORFLASH_SIZE        						(256 * NORFLASH_BLOCK_SIZE)

void NorflashDrvInit(void)
{
	SPIInit();
}


void ReadNorflashID(uint8_t *mid, uint16_t *did)
{
	SPIStart();
	SPIReadWriteByte(NORFLASH_JEDEC_ID);
	*mid = SPIReadWriteByte(DUMMY_BYTE);
	*did = SPIReadWriteByte(DUMMY_BYTE) << 8;
	*did |= SPIReadWriteByte(DUMMY_BYTE);
	SPIStop();
}

/**
*******************************************************************
* @function ָ����ַ��ʼ����ָ������������
* @param    readAddr,��ȡ��ַ
* @param    pBuffer,�����׵�ַ
* @param    len,Ҫ���������ݸ���
* @return   
*******************************************************************
*/
void ReadNorflashData(uint32_t readAddr, uint8_t *pBuffer, uint32_t len)
{ 
	SPIStart();
	SPIReadWriteByte(NORFLASH_READ_DATA);
	SPIReadWriteByte((readAddr & 0xFF0000) >> 16);
	SPIReadWriteByte((readAddr & 0x00FF00) >> 8);
	SPIReadWriteByte(readAddr & 0xFF);

	for (uint32_t i = 0; i < len; i++)
	{
		pBuffer[i] = SPIReadWriteByte(DUMMY_BYTE);
	}

	SPIStop();
}

static void EnableNorflashWrite(void)
{
	SPIStart();
	SPIReadWriteByte(NORFLASH_WRITE_ENABLE);
	SPIStop();
}

static void WaitNorflashWriteEnd(void)
{
	uint32_t Timeout = 100000;
	SPIStart();
	SPIReadWriteByte(NORFLASH_READ_STATUS_REGISTER_1);
	
	while ((SPIReadWriteByte(DUMMY_BYTE) & 0x01) == 0x01)
	{
		Timeout--;
		if (Timeout == 0)
		{
			break;
		}
	}
	SPIStop();
}

static void ProgramNorflashPage(uint32_t addr, uint8_t *pBuffer, uint32_t len)
{
	EnableNorflashWrite();
	SPIStart();
	SPIReadWriteByte(NORFLASH_PAGE_PROGRAM);
	SPIReadWriteByte((addr & 0xFF0000) >> 16);
	SPIReadWriteByte((addr & 0x00FF00) >> 8);
	SPIReadWriteByte(addr & 0xFF);

	while (len--)
	{
		SPIReadWriteByte(*pBuffer);
		pBuffer++;
	}

	SPIStop();
	WaitNorflashWriteEnd();
}

/**
*******************************************************************
* @function ָ����ַ��ʼд��ָ�����������ݣ�����ǰ��Ҫ����flash
* @param    writeAddr,д���ַ
* @param    pBuffer,�����׵�ַ
* @param    len,Ҫд������ݸ���
* @return                                                         
*******************************************************************
*/
void WriteNorflashData(uint32_t writeAddr, uint8_t *pBuffer, uint32_t len)
{
	uint8_t pageNum;
	uint8_t addrOffset = writeAddr % NORFLASH_PAGE_SIZE; 	// mod����������һҳ�ڵ�ƫ�ƣ���writeAddr��NORFLASH_PAGE_SIZE��������������Ϊ0

	if (len > (NORFLASH_PAGE_SIZE - addrOffset))    // ��ҳ
	{
		ProgramNorflashPage(writeAddr, pBuffer, NORFLASH_PAGE_SIZE - addrOffset);  // д����ҳ
		writeAddr += NORFLASH_PAGE_SIZE - addrOffset;   // ���뵽ҳ��ַ
		pBuffer += NORFLASH_PAGE_SIZE - addrOffset;
		len -= NORFLASH_PAGE_SIZE - addrOffset;
		pageNum = len / NORFLASH_PAGE_SIZE;

		while (pageNum--) 
		{
			ProgramNorflashPage(writeAddr, pBuffer, NORFLASH_PAGE_SIZE);  // д��ҳ����
			writeAddr += NORFLASH_PAGE_SIZE;
			pBuffer += NORFLASH_PAGE_SIZE;
			len -= NORFLASH_PAGE_SIZE;
		}

		ProgramNorflashPage(writeAddr, pBuffer, len);  // д������ҳ��ʣ������
	}
	else  // û�п�ҳ
	{
		ProgramNorflashPage(writeAddr, pBuffer, len);
	}
}

static void EraseNorflashSector(uint32_t eraseAddr)
{
	EnableNorflashWrite();
	SPIStart();
	SPIReadWriteByte(NORFLASH_SECTOR_ERASE_4KB);
	SPIReadWriteByte((eraseAddr & 0xFF0000) >> 16);
	SPIReadWriteByte((eraseAddr & 0x00FF00) >> 8);
	SPIReadWriteByte(eraseAddr & 0xFF);
	SPIStop();
	WaitNorflashWriteEnd();
}


/**
*******************************************************************
* @function ������eraseAddr��ʼ��eraseAddr + len����Ӧ������
* @param    eraseAddr,��ַ
* @param    len,��Ӧд�����ݵĸ���
* @return                                                         
*******************************************************************
*/
void EraseNorflashForWrite(uint32_t eraseAddr, uint32_t len)
{
	if (len == 0)
	{
		return;
	} 
	
	uint8_t sectorNum;
	uint8_t addrOffset = eraseAddr % NORFLASH_SECTOR_SIZE; 	// mod����������һҳ�ڵ�ƫ�ƣ���eraseAddr��NORFLASH_SECTOR_SIZE��������������Ϊ0

	if (len > (NORFLASH_SECTOR_SIZE - addrOffset))      // ������
	{
		EraseNorflashSector(eraseAddr);                 // ��������
		eraseAddr += NORFLASH_SECTOR_SIZE - addrOffset;   // ���뵽������ַ
		len -= NORFLASH_SECTOR_SIZE - addrOffset;
		sectorNum = len / NORFLASH_SECTOR_SIZE;

		while (sectorNum--) 
		{
			EraseNorflashSector(eraseAddr);
			eraseAddr += NORFLASH_SECTOR_SIZE;
		}
		if (len % NORFLASH_SECTOR_SIZE != 0)
		{
			EraseNorflashSector(eraseAddr);
		}
	}
	else  // û�п�����
	{
		EraseNorflashSector(eraseAddr);
	}
}

#define   BUFFER_SIZE              4099
#define   FLASH_ADDRESS            0x000002
void NorflashDrvTest(void)
{
	uint8_t mid = 0;
	uint16_t did = 0;
	uint8_t  bufferWrite[BUFFER_SIZE];
	uint8_t  bufferRead[BUFFER_SIZE];
	
    printf("***************************\n");
	printf("********GD25Qxx���Կ�ʼ********\n");
	printf("***************************\n");
	
	ReadNorflashID(&mid, &did);
    printf("Norflash MID: 0x%X, DID: 0x%X\n", mid, did);
	
	for (uint16_t i = 0; i < BUFFER_SIZE; i++)
	{ 
			bufferWrite[i]= i;
			printf("0x%02X ", bufferWrite[i]);
    }
	printf("\n��ʼд��\n");
	
	EraseNorflashForWrite(FLASH_ADDRESS, BUFFER_SIZE);
	WriteNorflashData(FLASH_ADDRESS, bufferWrite, BUFFER_SIZE);
	
	printf("��ʼ��ȡ...\n");
	ReadNorflashData(FLASH_ADDRESS, bufferRead, BUFFER_SIZE);

	for (uint16_t i = 0; i < BUFFER_SIZE; i++)
	{
        if (bufferRead[i] != bufferWrite[i]){
            printf("0x%02X ", bufferRead[i]);
            printf("���Թ��ϣ����Ų飡\n");
            return;
        }
        printf("0x%02X ", bufferRead[i]);

    }
	printf("\n����ͨ����\n");
}
