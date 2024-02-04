#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "gd32f30x.h"
#include "delay.h"

static float g_tempData;
#define MAX_BUF_SIZE         10
static uint16_t g_temp10MplBuf[MAX_BUF_SIZE];
	
static const uint16_t g_ntcAdcTable[] = {
	3123, 3089, 3051, 3013, 2973, 2933, 2893, 2852, 2810, 2767,     //0   ~   9��
	2720, 2681, 2637, 2593, 2548, 2503, 2458, 2412, 2367, 2321,     //10  ~  19��
	2275, 2230, 2184, 2138, 2093, 2048, 2002, 1958, 1913, 1869,     //20  ~  29��
	1825, 1782, 1739, 1697, 1655, 1614, 1573, 1533, 1494, 1455,     //30  ~  39��
	1417, 1380, 1343, 1307, 1272, 1237, 1203, 1170, 1138, 1106,     //40  ~  49��
	1081, 1045, 1016, 987,  959,  932,  905,  879,  854,  829,      //50  ~  59��
	806,  782,  760,  738,  716,  696,  675,  656,  637,  618,      //60  ~  69��
	600,  583,  566,  550,  534,  518,  503,  489,  475,  461,      //70  ~  79��
	448,  435,  422,  410,  398,  387,  376,  365,  355,  345,      //80  ~  89��
	335,  326,  316,  308,  299,  290,  283,  274,  267,  259,      //90  ~  99��
};
#define NTC_TABLE_SIZE         (sizeof(g_ntcAdcTable) / sizeof(g_ntcAdcTable[0]))
#define INDEX_TO_TEMP(index)   ((int32_t)index)

static void GpioInit(void)
{
	rcu_periph_clock_enable(RCU_GPIOC);
	gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_3);
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
	/* �������ݶ��룻*/
	adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
	/* ����ת��ͨ��������*/ 
	adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 1);
	/* ����ת����һ��ͨ���Լ���������λ�ã�*/ 
	adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_13, ADC_SAMPLETIME_239POINT5);  // PC3��Ӧͨ��13���������мĴ�����0�����У�239.5������
	/* ����ѡ����һ���ⲿ����Դ��*/ 
	adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE);
	/* ʹ���ⲿ������*/ 
	adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);
	/* ʹ��Ӳ���˲���*/ 
	adc_oversample_mode_config(ADC0, ADC_OVERSAMPLING_ALL_CONVERT, ADC_OVERSAMPLING_SHIFT_4B, ADC_OVERSAMPLING_RATIO_MUL16);
	adc_oversample_mode_enable(ADC0);
	/* ʹ��ADC��*/ 
	adc_enable(ADC0);
	/* �ڲ�У׼��*/ 
	DelayNus(50);
	adc_calibration_enable(ADC0);
	adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
}

static uint16_t GetAdcVal(void)
{
    /* �ȴ�ת����ɱ�־ */
    while(!adc_flag_get(ADC0, ADC_FLAG_EOC));

    /* ��ȡ���ݼĴ��� */
    return (adc_regular_data_read(ADC0));
}

/**
***********************************************************
* @brief �¶ȴ�����������ʼ��
* @param
* @return 
***********************************************************
*/
void TempDrvInit(void)
{
	GpioInit();
	AdcInit();
}

static int32_t DescBinarySearch(const uint16_t *arr, int32_t size, uint16_t key) 
{
	int32_t left = 0;              			
	int32_t right = size - 1;       		
	int32_t mid;
	int32_t index = size - 1;
	while (left <= right)             		
	{
		mid = left + (right - left) / 2; 
		if (key >= arr[mid])
		{
			right = mid - 1;
			index = mid;
		}
		else
		{
			left = mid + 1;  
		}
	}
    	return index;               				
}

static uint16_t AdcToTemp10Mpl(uint16_t adcVal)
{
	int32_t index = DescBinarySearch(g_ntcAdcTable, NTC_TABLE_SIZE, adcVal);
	
	if (index == 0)
	{
		return 0;
	}
	uint16_t temp10Mpl = INDEX_TO_TEMP(index - 1) * 10 + (g_ntcAdcTable[index - 1] - adcVal) * 10 / (g_ntcAdcTable[index - 1] - g_ntcAdcTable[index]);
	return temp10Mpl;
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
	return ArithAvgFltr(&arr[1], len - 2);
}

void PushDataToBuf(uint16_t temp10Mpl)
{
	static uint16_t s_index = 0;
	g_temp10MplBuf[s_index] = temp10Mpl; 
	s_index++;
	s_index %= MAX_BUF_SIZE;
}

/**
***********************************************************
* @brief ��������ת���¶ȴ���������
* @param
* @return 
***********************************************************
*/
void TempSensorProc(void)
{
	static uint16_t s_convertNum = 0;

	uint16_t adcVal = GetAdcVal();
	uint16_t temp10Mpl = AdcToTemp10Mpl(adcVal);
	
	
	PushDataToBuf(temp10Mpl);
	s_convertNum++;
	if (s_convertNum < 3)
	{
		g_tempData = g_temp10MplBuf[0] / 10.0f;
		return;
	}
	
	if (s_convertNum > MAX_BUF_SIZE)
	{
		s_convertNum = MAX_BUF_SIZE;
	}
	
	g_tempData = MedianAvgFltr(g_temp10MplBuf, s_convertNum) / 10.0f;
}

/**
***********************************************************
* @brief ��ȡ�¶ȴ���������
* @param
* @return �¶����ݣ�С��
***********************************************************
*/
float GetTempData(void)
{
	return g_tempData;
}
