#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "gd32f30x.h"
#include "delay.h"

#define SET_SPI_NSS()            gpio_bit_set(GPIOE, GPIO_PIN_2)
#define CLR_SPI_NSS()            gpio_bit_reset(GPIOE, GPIO_PIN_2)


static void GpioInit(void)
{
	/* PE2作为片选SS */
	rcu_periph_clock_enable(RCU_GPIOE);                                           //使能GPIOE的时钟
	gpio_init(GPIOE, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);            //设置GPIO输出模式及速度
	gpio_bit_set(GPIOE, GPIO_PIN_2);                                              //将PE2默认状态设置为高电平

	rcu_periph_clock_enable(RCU_GPIOB);                                           //使能GPIOB的时钟
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


#define NORFLASH_PAGE_SIZE    						256                            //一页的大小，字节
#define NORFLASH_SECTOR_SIZE 						4096                           //扇区大小，字节
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
* @function 指定地址开始读出指定个数的数据
* @param    readAddr,读取地址
* @param    pBuffer,数组首地址
* @param    len,要读出的数据个数
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
* @function 指定地址开始写入指定个数的数据，调用前需要擦除flash
* @param    writeAddr,写入地址
* @param    pBuffer,数组首地址
* @param    len,要写入的数据个数
* @return                                                         
*******************************************************************
*/
void WriteNorflashData(uint32_t writeAddr, uint8_t *pBuffer, uint32_t len)
{ 
	uint8_t pageNum; 
	uint8_t addrOffset = writeAddr % NORFLASH_PAGE_SIZE; 	// mod运算求余在一页内的偏移，若writeAddr是NORFLASH_PAGE_SIZE整数倍，运算结果为0

	if (len > (NORFLASH_PAGE_SIZE - addrOffset))    // 跨页
	{
		ProgramNorflashPage(writeAddr, pBuffer, NORFLASH_PAGE_SIZE - addrOffset);  // 写满本页
		writeAddr += NORFLASH_PAGE_SIZE - addrOffset;   // 对齐到页地址
		pBuffer += NORFLASH_PAGE_SIZE - addrOffset;
		len -= NORFLASH_PAGE_SIZE - addrOffset;
		pageNum = len / NORFLASH_PAGE_SIZE;

		while (pageNum--) 
		{
			ProgramNorflashPage(writeAddr, pBuffer, NORFLASH_PAGE_SIZE);  // 写整页数据
			writeAddr += NORFLASH_PAGE_SIZE;
			pBuffer += NORFLASH_PAGE_SIZE;
			len -= NORFLASH_PAGE_SIZE;
		}

		ProgramNorflashPage(writeAddr, pBuffer, len);  // 写不满整页的剩余数据
	}
	else  // 没有跨页
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
* @function 擦除从eraseAddr开始到eraseAddr + len所对应的扇区
* @param    eraseAddr,地址
* @param    len,对应写入数据的个数
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
	uint8_t addrOffset = eraseAddr % NORFLASH_SECTOR_SIZE; 	// mod运算求余在一扇区内的偏移，若eraseAddr是NORFLASH_SECTOR_SIZE整数倍，运算结果为0

	if (len > (NORFLASH_SECTOR_SIZE - addrOffset))      // 跨扇区
	{
		EraseNorflashSector(eraseAddr);                 // 擦本扇区
		eraseAddr += NORFLASH_SECTOR_SIZE - addrOffset;   // 对齐到扇区地址
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
	else  // 没有跨扇区
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
	printf("********GD25Qxx测试开始********\n");
	printf("***************************\n");
	
	ReadNorflashID(&mid, &did);
    printf("Norflash MID: 0x%X, DID: 0x%X\n", mid, did);
	
	for (uint16_t i = 0; i < BUFFER_SIZE; i++)
	{ 
			bufferWrite[i]= i + 1;
			printf("0x%02X ", bufferWrite[i]);
    }
	printf("\n开始写入\n");
	
	EraseNorflashForWrite(FLASH_ADDRESS, BUFFER_SIZE);
	WriteNorflashData(FLASH_ADDRESS, bufferWrite, BUFFER_SIZE);
	
	printf("开始读取...\n");
	ReadNorflashData(FLASH_ADDRESS, bufferRead, BUFFER_SIZE);

	for (uint16_t i = 0; i < BUFFER_SIZE; i++)
	{
        if (bufferRead[i] != bufferWrite[i]){
            printf("0x%02X ", bufferRead[i]);
            printf("测试故障，请排查！\n");
            return;
        }
        printf("0x%02X ", bufferRead[i]);

    }
	printf("\n测试通过！\n");
}
