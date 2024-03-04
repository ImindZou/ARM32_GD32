#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "gd32f30x.h"

#define BANK0_REGON3_ADDR        ((uint32_t)(0x6C000000))    // Bank0 Regon3��ʼ��ַ
#define EX_SRAM_BUFFER_SIZE      (1 * 1024 * 1024)           // �ⲿSRAM��������С��1M�ֽ�
#define EX_SRAM_BEGIN_ADDR       (BANK0_REGON3_ADDR)
#define EX_SRAM_END_ADDR         (BANK0_REGON3_ADDR + EX_SRAM_BUFFER_SIZE - 1)  // 0x6C100000 - 1

static void ConfigEXMCGPIO(void)
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

	/* �����ź� */
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
	/* ʹ��EXMCʱ�� */
	rcu_periph_clock_enable(RCU_EXMC);

	/* �ⲿSRAM��дʱ�� */
	sramTimingInitStruct.asyn_access_mode = EXMC_ACCESS_MODE_A; //ģʽA���첽����SRAM
	sramTimingInitStruct.asyn_address_setuptime = 0; //�첽���ʵ�ַ����ʱ��
	sramTimingInitStruct.asyn_address_holdtime  = 0; //�첽���ʵ�ַ����ʱ��
	sramTimingInitStruct.asyn_data_setuptime    = 3; //�첽�������ݽ���ʱ��
	sramTimingInitStruct.bus_latency            = 0; //ͬ��/�첽����������ʱʱ��
	sramTimingInitStruct.syn_clk_division       = 0; //ͬ������ʱ�ӷ�Ƶϵ������HCLK�з�Ƶ��
	sramTimingInitStruct.syn_data_latency       = 0; //ͬ�������л�õ�1����������Ҫ�ĵȴ��ӳ�

	/* Region3���� */
	sramInitStruct.norsram_region    = EXMC_BANK0_NORSRAM_REGION3; //Region3
	sramInitStruct.address_data_mux  = DISABLE;                    //���õ�ַ���������߶�·����
	sramInitStruct.memory_type       = EXMC_MEMORY_TYPE_SRAM;      //����������ΪSRAM
	sramInitStruct.databus_width     = EXMC_NOR_DATABUS_WIDTH_16B; //���ݿ��16λ
	sramInitStruct.burst_mode        = DISABLE;                    //����ͻ������
	sramInitStruct.nwait_config      = EXMC_NWAIT_CONFIG_BEFORE;   //�ȴ���������
	sramInitStruct.nwait_polarity    = EXMC_NWAIT_POLARITY_LOW;    //�ȴ������źŵ͵�ƽ��Ч
	sramInitStruct.wrap_burst_mode   = DISABLE;                    //���ð�ͻ������
	sramInitStruct.asyn_wait         = DISABLE;                    //�����첽�ȴ�
	sramInitStruct.extended_mode     = DISABLE;                    //������չģʽ
	sramInitStruct.memory_write      = ENABLE;                     //ʹ��д���ⲿ�洢��
	sramInitStruct.nwait_signal      = DISABLE;                    //���õȴ������ź�
	sramInitStruct.write_mode        = EXMC_ASYN_WRITE;            //д��ģʽΪ�첽д��
	sramInitStruct.read_write_timing = &sramTimingInitStruct;      //��дʱ������

	/* ��ʼ��Region3 */
	exmc_norsram_init(&sramInitStruct);

	/* ʹ��Region3 */
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
            printf("\n\nex sram error��please check��\n\n");
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
