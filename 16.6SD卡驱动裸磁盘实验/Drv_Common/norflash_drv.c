#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "gd32f30x.h"
#include "delay.h"

#define SET_SPI_NSS()            gpio_bit_set(GPIOE, GPIO_PIN_2)
#define CLR_SPI_NSS()            gpio_bit_reset(GPIOE, GPIO_PIN_2)


static void GpioInit(void)
{
	/* PE2��ΪƬѡSS */
	rcu_periph_clock_enable(RCU_GPIOE);                                           //ʹ��GPIOE��ʱ��
	gpio_init(GPIOE, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);            //����GPIO���ģʽ���ٶ�
	gpio_bit_set(GPIOE, GPIO_PIN_2);                                              //��PE2Ĭ��״̬����Ϊ�ߵ�ƽ

	rcu_periph_clock_enable(RCU_GPIOB);                                           //ʹ��GPIOB��ʱ��
    rcu_periph_clock_enable(RCU_SPI1);

    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13 | GPIO_PIN_15);
    gpio_init(GPIOB, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_14);

    spi_parameter_struct spiInitStruct;
	spi_struct_para_init(&spiInitStruct);
    
    spiInitStruct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spiInitStruct.device_mode          = SPI_MASTER;
    spiInitStruct.frame_size           = SPI_FRAMESIZE_8BIT;
    spiInitStruct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    spiInitStruct.nss                  = SPI_NSS_SOFT;
    spiInitStruct.prescale             = SPI_PSC_2;
    spiInitStruct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI1, &spiInitStruct);

    spi_enable(SPI1);                                    
}

static void SPIInit(void)
{
	GpioInit();

	SET_SPI_NSS();
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
    /* loop while data register in not emplty */
    while (RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_TBE));

    /* send byte through the SPI0 peripheral */
    spi_i2s_data_transmit(SPI1, sendData);
	
	uint32_t timeout = 100000;
    /* wait to receive a byte */
    while(RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_RBNE))
	{
		timeout--;
		if (timeout == 0)
		{
			break;
		}
	}

    /* return the byte read from the SPI bus */
    return spi_i2s_data_receive(SPI1);
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


#define NORFLASH_PAGE_SIZE    						256                            //һҳ�Ĵ�С���ֽ�
#define NORFLASH_SECTOR_SIZE 						4096                           //������С���ֽ�
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
	uint32_t timeout = 100000;
	SPIStart();
	SPIReadWriteByte(NORFLASH_READ_STATUS_REGISTER_1);
	
	while ((SPIReadWriteByte(DUMMY_BYTE) & 0x01) == 0x01)
	{
		timeout--;
		if (timeout == 0)
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
	uint8_t addrOffset = eraseAddr % NORFLASH_SECTOR_SIZE; 	// mod����������һ�����ڵ�ƫ�ƣ���eraseAddr��NORFLASH_SECTOR_SIZE��������������Ϊ0

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
			bufferWrite[i]= i + 1;
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
