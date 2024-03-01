#include <stdint.h>
#include <stdio.h>
#include "gd32f30x.h"

static void GpioInit(void)
{
	/* ʹ��RCU���ʱ�� */
	rcu_periph_clock_enable(RCU_GPIOD);  //ʹ��GPIOD��ʱ��
	rcu_periph_clock_enable(RCU_GPIOE);  //ʹ��GPIOE��ʱ��
	rcu_periph_clock_enable(RCU_GPIOF);  //ʹ��GPIOF��ʱ��
	rcu_periph_clock_enable(RCU_GPIOG);  //ʹ��GPIOG��ʱ��

	/* ��ַ���� */
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

	/* �������� */
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

	/* �����ź� */
	gpio_init(GPIOG, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_9); //EXMC_NE1
	gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_4); //EXMC_NOE
	gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_MAX, GPIO_PIN_5); //EXMC_NWE
}

static void ConfigBank0Region1(void)
{
	exmc_norsram_parameter_struct        sram_init_struct;
	exmc_norsram_timing_parameter_struct lcd_readwrite_timing_init_struct;
	exmc_norsram_timing_parameter_struct lcd_write_timing_init_struct;

	//ʹ��EXMCʱ��
	rcu_periph_clock_enable(RCU_EXMC);

	//��ʱ������
	lcd_readwrite_timing_init_struct.asyn_access_mode = EXMC_ACCESS_MODE_A; //ģʽA���첽����SRAM
	lcd_readwrite_timing_init_struct.asyn_address_setuptime = 0;  //�첽���ʵ�ַ����ʱ��
	lcd_readwrite_timing_init_struct.asyn_address_holdtime  = 0;  //�첽���ʵ�ַ����ʱ��
	lcd_readwrite_timing_init_struct.asyn_data_setuptime    = 15; //�첽�������ݽ���ʱ��
	lcd_readwrite_timing_init_struct.bus_latency            = 0;  //ͬ��/�첽����������ʱʱ��
	lcd_readwrite_timing_init_struct.syn_clk_division       = 0;  //ͬ������ʱ�ӷ�Ƶϵ������HCLK�з�Ƶ��
	lcd_readwrite_timing_init_struct.syn_data_latency       = 0;  //ͬ�������л�õ�1����������Ҫ�ĵȴ��ӳ�

	//дʱ������
	lcd_write_timing_init_struct.asyn_access_mode = EXMC_ACCESS_MODE_A; //ģʽA���첽����SRAM
	lcd_write_timing_init_struct.asyn_address_setuptime     = 0; //�첽���ʵ�ַ����ʱ��
	lcd_write_timing_init_struct.asyn_address_holdtime      = 0; //�첽���ʵ�ַ����ʱ��
	lcd_write_timing_init_struct.asyn_data_setuptime        = 2; //�첽�������ݽ���ʱ��
	lcd_write_timing_init_struct.bus_latency                = 0; //ͬ��/�첽����������ʱʱ��
	lcd_write_timing_init_struct.syn_clk_division           = 0; //ͬ������ʱ�ӷ�Ƶϵ������HCLK�з�Ƶ��
	lcd_write_timing_init_struct.syn_data_latency           = 0; //ͬ�������л�õ�1����������Ҫ�ĵȴ��ӳ�

	//Region1����
	sram_init_struct.norsram_region    = EXMC_BANK0_NORSRAM_REGION1; //Region1
	sram_init_struct.address_data_mux  = DISABLE;                    //���õ�ַ���������߶�·����
	sram_init_struct.memory_type       = EXMC_MEMORY_TYPE_SRAM;      //����������ΪSRAM
	sram_init_struct.databus_width     = EXMC_NOR_DATABUS_WIDTH_16B; //���ݿ��16λ
	sram_init_struct.burst_mode        = DISABLE;                    //����ͻ������
	sram_init_struct.nwait_config      = EXMC_NWAIT_CONFIG_BEFORE;   //�ȴ���������
	sram_init_struct.nwait_polarity    = EXMC_NWAIT_POLARITY_LOW;    //�ȴ������źŵ͵�ƽ��Ч
	sram_init_struct.wrap_burst_mode   = DISABLE;                    //���ð�ͻ������
	sram_init_struct.asyn_wait         = DISABLE;                    //�����첽�ȴ�
	sram_init_struct.extended_mode     = ENABLE;                     //ʹ����չģʽ
	sram_init_struct.memory_write      = ENABLE;                     //ʹ��д���ⲿ�洢��
	sram_init_struct.nwait_signal      = DISABLE;                    //���õȴ������ź�
	sram_init_struct.write_mode        = EXMC_ASYN_WRITE;            //д��ģʽΪ�첽д��
	sram_init_struct.read_write_timing = &lcd_readwrite_timing_init_struct; //��ʱ������
	sram_init_struct.write_timing      = &lcd_write_timing_init_struct;     //дʱ������

	//��ʼ��Region1
	exmc_norsram_init(&sram_init_struct);

	//ʹ��Region1
	exmc_norsram_enable(EXMC_BANK0_NORSRAM_REGION1);
}