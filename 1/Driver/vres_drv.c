#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "gd32f30x.h"
#include "delay.h"


#define ADC0_RDATA_ADDR    (ADC0 + 0x4C)

#define MAX_BUF_SIZE        10
static uint16_t g_adcVal[MAX_BUF_SIZE];

static void GpioInit(void)
{
	rcu_periph_clock_enable(RCU_GPIOC);
	gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_2);
}

static void AdcInit(void)
{
	/* ʹ��ʱ�ӣ�*/
	rcu_periph_clock_enable(RCU_ADC0);
	/* ���÷�Ƶϵ����*/
	rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV6);  // 6��Ƶ��120MHz / 6 = 20MHz
	/* ���ö���ģʽ��*/
	adc_mode_config(ADC_MODE_FREE);
	/* ��������ģʽ��*/ 
	adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, ENABLE);
	/* ����ɨ��ģʽ��*/ 
	adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);
	/* �������ݶ��룻*/
	adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
	/* ����ת��ͨ��������*/ 
	adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 1);
	/* ����ת����һ��ͨ���Լ���������λ�ã�*/ 
	adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_12, ADC_SAMPLETIME_239POINT5);  // PC2��Ӧͨ��12���������мĴ�����0�����У�239.5������
	/* ����ѡ����һ���ⲿ����Դ��*/ 
	adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE);
	/* ʹ���ⲿ������*/ 
	adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);
	/* ʹ��ADC��DMA���ܣ�*/ 
	adc_dma_mode_enable(ADC0);
	/* ʹ��ADC��*/ 
	adc_enable(ADC0);
	/* �ڲ�У׼��*/ 
	DelayNus(50);
	adc_calibration_enable(ADC0);
	adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
}


static void DmaInit(void)
{
	/* ʹ��DMAʱ�ӣ�*/
	rcu_periph_clock_enable(RCU_DMA0);
	/* ��λDMAͨ����*/
	dma_deinit(DMA0, DMA_CH0);
	
	dma_parameter_struct dmaStruct;
	dma_struct_para_init(&dmaStruct);
	/* ���ô��䷽��*/ 
	dmaStruct.direction = DMA_PERIPHERAL_TO_MEMORY;
	/* ��������Դ��ַ��*/ 
	dmaStruct.periph_addr = ADC0_RDATA_ADDR;
	/* ����Դ��ַ�ǹ̶��Ļ��������ģ�*/ 
	dmaStruct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	/* ����Դ���ݴ���λ��*/ 
	dmaStruct.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
	
	/* ��������Ŀ�ĵ�ַ��*/
	dmaStruct.memory_addr = (uint32_t)g_adcVal;
	/* ����Ŀ�ĵ�ַ�ǹ̶��Ļ��������ģ�*/ 
	dmaStruct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	/* ����Ŀ�����ݴ���λ��*/ 
	dmaStruct.memory_width = DMA_MEMORY_WIDTH_16BIT;
	/* �������ݴ�����������*/ 
	dmaStruct.number = MAX_BUF_SIZE;
	/* ����DMAͨ�����ȼ���*/ 
	dmaStruct.priority = DMA_PRIORITY_HIGH;
	dma_init(DMA0, DMA_CH0, &dmaStruct);
	
	/* ʹ��DMAѭ��ģʽ�������ݣ�*/ 
	dma_circulation_enable(DMA0, DMA_CH0);
	/* ʹ��DMAͨ����*/ 
	dma_channel_enable(DMA0, DMA_CH0);
}

/**
***********************************************************
* @brief ADCӲ����ʼ��
* @param
* @return 
***********************************************************
*/
void VresDrvInit(void)
{
	GpioInit();
	AdcInit();
	DmaInit();
}

/**
***********************************************************
* @brief ����ƽ���˲�
* @param arr�������׵�ַ
* @param len��Ԫ�ظ���
* @return ƽ��������
***********************************************************
*/
static uint16_t ArithAvgFltr(uint16_t *arr, uint32_t len)
{
	uint32_t sum = 0;
	for (uint32_t i = 0; i < len; i++)
	{
		sum += arr[i];
	}
	return (uint16_t)(sum / len);
}

/**
***********************************************************
* @brief qsort�������õĻص��������ȽϹ��򣬽�������
* @param *_a����Ӧ����Ԫ��
* @param *_b����Ӧ����Ԫ��
* @return �ȽϽ��
***********************************************************
*/
static int32_t CmpCb(const void *_a, const void *_b)
{
	uint16_t *a = (uint16_t *)_a;
	uint16_t *b = (uint16_t *)_b;
	int32_t val = 0;
	 if (*a > *b)
	 {
		val = -1;
	 }
	 else if (*a < *b)
	 {
		val =  1;
	 }
	 else
	 {
		val = 0;
	 }
	 return val;
}

/**
***********************************************************
* @brief ��λֵƽ���˲�
* @param arr�������׵�ַ
* @param len��Ԫ�ظ�������Ҫ���ڵ���3��
* @return ƽ��������
***********************************************************
*/
static uint16_t MedianAvgFltr(uint16_t *arr, uint32_t len)
{
	qsort(arr, len, sizeof(uint16_t), CmpCb);
	printf("\n after qsort.\n");
	for (uint8_t i = 0; i < len; i++)
	{
		printf("AdcVal[%d] = %d.\n", i, arr[i]);
	}
	return ArithAvgFltr(&arr[1], len - 2);
}

void VresDrvTest(void)
{
	uint16_t buf[MAX_BUF_SIZE];
	memcpy(buf, g_adcVal, sizeof(uint16_t) * MAX_BUF_SIZE);
	for (uint8_t i = 0; i < MAX_BUF_SIZE; i++)
	{
		printf("AdcVal[%d] = %d.\n", i, buf[i]);
	}

	uint16_t res = MedianAvgFltr(buf, MAX_BUF_SIZE);
	printf("res = %d.\n", res);
	DelayNms(1000);
}
