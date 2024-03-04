#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "gd32f30x.h"

#define BANK0_REGON3_ADDR        ((uint32_t)(0x6C000000))    // Bank0 Regon3起始地址
#define EX_SRAM_BUFFER_SIZE      (1 * 1024 * 1024)           // 外部SRAM缓冲区大小，1M字节
#define EX_SRAM_BEGIN_ADDR       (BANK0_REGON3_ADDR)
#define EX_SRAM_END_ADDR         (BANK0_REGON3_ADDR + EX_SRAM_BUFFER_SIZE - 1)  // 0x6C100000 - 1

static void ConfigEXMCGPIO(void)
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
	gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_14); //D0
	gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_15); //D1
	gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_0 ); //D2
	gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_1 ); //D3
	gpio_init(GPIOE, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_7 ); //D4
	gpio_init(GPIOE, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_8 ); //D5
	gpio_init(GPIOE, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_9 ); //D6
	gpio_init(GPIOE, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_10); //D7
	gpio_init(GPIOE, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_11); //D8
	gpio_init(GPIOE, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_12); //D9
	gpio_init(GPIOE, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_13); //D10
	gpio_init(GPIOE, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_14); //D11
	gpio_init(GPIOE, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_15); //D12
	gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_8 ); //D13
	gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_9 ); //D14
	gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_10); //D15

	/* 控制信号 */
	gpio_init(GPIOG, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_9 ); //EXMC_NE1
	gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_4 ); //EXMC_NOE
	gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_5 ); //EXMC_NWE
	gpio_init(GPIOG, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_12); //EXMC_NE3
	gpio_init(GPIOE, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_0 ); //EXMC_NBL0
	gpio_init(GPIOE, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_1 ); //EXMC_NBL1
}

static void ConfigBank0Region3(void)
{
	exmc_norsram_parameter_struct        sramInitStruct;
	exmc_norsram_timing_parameter_struct sramTimingInitStruct;
	exmc_norsram_struct_para_init(&sramInitStruct);
	/* 使能EXMC时钟 */
	rcu_periph_clock_enable(RCU_EXMC);

	/* 外部SRAM读写时序 */
	sramTimingInitStruct.asyn_access_mode = EXMC_ACCESS_MODE_A; //模式A，异步访问SRAM
	sramTimingInitStruct.asyn_address_setuptime = 0; //异步访问地址建立时间
	sramTimingInitStruct.asyn_address_holdtime  = 0; //异步访问地址保持时间
	sramTimingInitStruct.asyn_data_setuptime    = 3; //异步访问数据建立时间
	sramTimingInitStruct.bus_latency            = 0; //同步/异步访问总线延时时间
	sramTimingInitStruct.syn_clk_division       = 0; //同步访问时钟分频系数（从HCLK中分频）
	sramTimingInitStruct.syn_data_latency       = 0; //同步访问中获得第1个数据所需要的等待延迟

	/* Region3配置 */
	sramInitStruct.norsram_region    = EXMC_BANK0_NORSRAM_REGION3; //Region3
	sramInitStruct.address_data_mux  = DISABLE;                    //禁用地址、数据总线多路复用
	sramInitStruct.memory_type       = EXMC_MEMORY_TYPE_SRAM;      //储存器类型为SRAM
	sramInitStruct.databus_width     = EXMC_NOR_DATABUS_WIDTH_16B; //数据宽度16位
	sramInitStruct.burst_mode        = DISABLE;                    //禁用突发访问
	sramInitStruct.nwait_config      = EXMC_NWAIT_CONFIG_BEFORE;   //等待输入配置
	sramInitStruct.nwait_polarity    = EXMC_NWAIT_POLARITY_LOW;    //等待输入信号低电平有效
	sramInitStruct.wrap_burst_mode   = DISABLE;                    //禁用包突发访问
	sramInitStruct.asyn_wait         = DISABLE;                    //禁用异步等待
	sramInitStruct.extended_mode     = DISABLE;                    //禁用扩展模式
	sramInitStruct.memory_write      = ENABLE;                     //使能写入外部存储器
	sramInitStruct.nwait_signal      = DISABLE;                    //禁用等待输入信号
	sramInitStruct.write_mode        = EXMC_ASYN_WRITE;            //写入模式为异步写入
	sramInitStruct.read_write_timing = &sramTimingInitStruct;      //读写时序配置

	/* 初始化Region3 */
	exmc_norsram_init(&sramInitStruct);

	/* 使能Region3 */
	exmc_norsram_enable(EXMC_BANK0_NORSRAM_REGION3);
}

void ExSramDrvInit(void)
{
	ConfigEXMCGPIO();
	ConfigBank0Region3();
}

#define BUFFER_SIZE              6
//static uint8_t g_buffer[BUFFER_SIZE] __attribute__((at(EX_SRAM_BEGIN_ADDR)));	
//static uint8_t g_buffer[BUFFER_SIZE];
static uint8_t g_buffer[BUFFER_SIZE] = {0, 1, 2, 3, 4, 5,};
void ExSramDrvTest(void)
{
	printf("********************************\n");
	printf("********ex SRAM stst start********\n");
	printf("********************************\n");

//	printf("ex sram writing...\r\n");
//	for (uint16_t i = 0; i < BUFFER_SIZE; i++)
//	{ 
//        g_buffer[i] = i;
//        printf("0x%02X ", i);
//        if (15 == i % 16)
//		{
//            printf("\r\n");
//        }
//    }

    printf("ex sram reading...\r\n");
    for (uint16_t i = 0; i < BUFFER_SIZE; i++)
	{
        if (i != g_buffer[i])
		{
            printf("0x%02X ", g_buffer[i]);
            printf("\n\nex sram error，please check！\n\n");
            return;
        }
        printf("0x%02X ", g_buffer[i]);
        if (15 == i % 16)
		{
            printf("\r\n");
		}
	}

	printf("********************************\n");
	printf("********ex SRAM test ending********\n");
	printf("********************************\n");
}
