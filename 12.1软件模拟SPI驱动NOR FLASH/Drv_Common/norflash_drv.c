/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-02-02 13:51:12
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-02-02 14:50:58
 * @FilePath: \12.1软件模拟SPI驱动NOR FLASH\Drv_Common\norflash_drv.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-02-02 13:51:12
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-02-02 14:18:38
 * @FilePath: \12.1软件模拟SPI驱动NOR FLASH\Drv_Common\norflash_drv.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "gd32f30x.h"
#include "delay.h"

#define SET_SPI_NSS()            gpio_bit_set(GPIOE, GPIO_PIN_2)
#define CLR_SPI_NSS()            gpio_bit_reset(GPIOE, GPIO_PIN_2)

#define SET_SPI_SCK()            gpio_bit_set(GPIOB, GPIO_PIN_13)
#define CLR_SPI_SCK()            gpio_bit_reset(GPIOB, GPIO_PIN_13)

#define READ_SPI_MISO()          gpio_input_port_get(GPIOB)

#define SET_SPI_MOSI()           gpio_bit_set(GPIOB, GPIO_PIN_15)
#define CLR_SPI_MOSI()           gpio_bit_reset(GPIOB, GPIO_PIN_15)


static void GpioInit(void)
{
    /*PE2 is SS*/
    //Enable GPIOE
    rcu_periph_clock_enable(RCU_GPIOE);
    //config Gpio output mode and speed
    gpio_init(GPIOE, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
    //set PE2 defalut status is high level
    gpio_bit_set(GPIOE, GPIO_PIN_2);

    /*PB13 is SCK*/
    //Enable GPIOB Clock
    rcu_periph_clock_enable(RCU_GPIOB);
    //Config GPIO output mode and speed
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
    //set PB13 defalut is high level
    gpio_bit_set(GPIOB, GPIO_PIN_13);

    /*PB14 is MISO*/
    //config PB14 is input mode
    gpio_init(GPIOB, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_14);

    /*PB15 is MOSI*/
    //config GPIO output mode and speed
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_15);
    //PB15 default status is high level
    gpio_bit_set(GPIOB, GPIO_PIN_15);
}

static void SPIInit(void)
{
    GpioInit();

    SET_SPI_NSS();
    SET_SPI_SCK();
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
    uint8_t revData = 0;

    for (uint8_t i = 0; i < 8; i++)
    {
        CLR_SPI_SCK();
        if((sendData & 0x80))
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
    }
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


#define NORFLASH_PAGE_SIZE    						256                            //One Page Size(Byte)
#define NORFLASH_SECTOR_SIZE 						4096                           //Sector Size(Byte)
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
***********************************************************
* @brief 
* @param
* @return 
***********************************************************
*/







#define BUFFER_SIZE                4099
#define FLASH_ADDRESS              0x000002
void NorflashDrvTest(void)
{
    uint8_t mid = 0;
    uint16_t did = 0;
    uint8_t bufferWrite[BUFFER_SIZE];
    uint8_t bufferRead[BUFFER_SIZE];

    printf("*************************\r\n");
    printf("*******GD25QxxTestStart*********\r\n");
    printf("*************************\r\n");

    ReadNorflashID(&mid,&did);
    printf("Norfalsh MID: 0x%X, DID: 0x%X\n",mid, did);
}

