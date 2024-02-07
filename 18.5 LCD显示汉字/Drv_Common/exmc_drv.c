#include <stdint.h>
#include <stdio.h>
#include "gd32f30x.h"

static void GpioInit(void)
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

	/* 控制信号 */
	gpio_init(GPIOG, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_9); //EXMC_NE1
	gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_4); //EXMC_NOE
	gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_5); //EXMC_NWE
}

static void ConfigBank0Region1(void)
{
	exmc_norsram_parameter_struct        sram_init_struct;
	exmc_norsram_timing_parameter_struct lcd_readwrite_timing_init_struct;
	exmc_norsram_timing_parameter_struct lcd_write_timing_init_struct;

	//使能EXMC时钟
	rcu_periph_clock_enable(RCU_EXMC);

	//读时序配置
	lcd_readwrite_timing_init_struct.asyn_access_mode = EXMC_ACCESS_MODE_A; //模式A，异步访问SRAM
	lcd_readwrite_timing_init_struct.asyn_address_setuptime = 0;  //异步访问地址建立时间
	lcd_readwrite_timing_init_struct.asyn_address_holdtime  = 0;  //异步访问地址保持时间
	lcd_readwrite_timing_init_struct.asyn_data_setuptime    = 15; //异步访问数据建立时间
	lcd_readwrite_timing_init_struct.bus_latency            = 0;  //同步/异步访问总线延时时间
	lcd_readwrite_timing_init_struct.syn_clk_division       = 0;  //同步访问时钟分频系数（从HCLK中分频）
	lcd_readwrite_timing_init_struct.syn_data_latency       = 0;  //同步访问中获得第1个数据所需要的等待延迟

	//写时序配置
	lcd_write_timing_init_struct.asyn_access_mode = EXMC_ACCESS_MODE_A; //模式A，异步访问SRAM
	lcd_write_timing_init_struct.asyn_address_setuptime     = 0; //异步访问地址建立时间
	lcd_write_timing_init_struct.asyn_address_holdtime      = 0; //异步访问地址保持时间
	lcd_write_timing_init_struct.asyn_data_setuptime        = 2; //异步访问数据建立时间
	lcd_write_timing_init_struct.bus_latency                = 0; //同步/异步访问总线延时时间
	lcd_write_timing_init_struct.syn_clk_division           = 0; //同步访问时钟分频系数（从HCLK中分频）
	lcd_write_timing_init_struct.syn_data_latency           = 0; //同步访问中获得第1个数据所需要的等待延迟

	//Region1配置
	sram_init_struct.norsram_region    = EXMC_BANK0_NORSRAM_REGION1; //Region1
	sram_init_struct.address_data_mux  = DISABLE;                    //禁用地址、数据总线多路复用
	sram_init_struct.memory_type       = EXMC_MEMORY_TYPE_SRAM;      //储存器类型为SRAM
	sram_init_struct.databus_width     = EXMC_NOR_DATABUS_WIDTH_16B; //数据宽度16位
	sram_init_struct.burst_mode        = DISABLE;                    //禁用突发访问
	sram_init_struct.nwait_config      = EXMC_NWAIT_CONFIG_BEFORE;   //等待输入配置
	sram_init_struct.nwait_polarity    = EXMC_NWAIT_POLARITY_LOW;    //等待输入信号低电平有效
	sram_init_struct.wrap_burst_mode   = DISABLE;                    //禁用包突发访问
	sram_init_struct.asyn_wait         = DISABLE;                    //禁用异步等待
	sram_init_struct.extended_mode     = ENABLE;                     //使能扩展模式
	sram_init_struct.memory_write      = ENABLE;                     //使能写入外部存储器
	sram_init_struct.nwait_signal      = DISABLE;                    //禁用等待输入信号
	sram_init_struct.write_mode        = EXMC_ASYN_WRITE;            //写入模式为异步写入
	sram_init_struct.read_write_timing = &lcd_readwrite_timing_init_struct; //读时序配置
	sram_init_struct.write_timing      = &lcd_write_timing_init_struct;     //写时序配置

	//初始化Region1
	exmc_norsram_init(&sram_init_struct);

	//使能Region1
	exmc_norsram_enable(EXMC_BANK0_NORSRAM_REGION1);
}