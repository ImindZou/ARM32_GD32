#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "gd32f30x.h"
#include "delay.h"
#include "led_drv.h"

static uint16_t g_adcVal;
static uint8_t g_humiData;
#define HUMI_LEVEL_NUM   18
static const uint16_t g_rhADCVal[][HUMI_LEVEL_NUM] = {
//         10%RH  15%RH  20%RH  25%RH  30%RH  35%RH  40%RH  45%RH  50%RH  55%RH  60%RH  65%RH  70%RH  75%RH  80%RH  85%RH  90%RH  95%RH               
/* 0��*/  {154,   155,   160,   165,   177,   204,   257,   354,   516,   803,   1189,  1633,  2101,  2579,  2919,  3204,  3434,  3595,},
/* 5��*/  {154,   155,   161,   168,   186,   223,   296,   429,   650,   985,   1408,  1874,  2330,  2796,  3113,  3356,  3571,  3702,},
/*10��*/  {154,   156,   164,   175,   199,   250,   342,   508,   775,   1154,  1633,  2127,  2540,  2945,  3236,  3456,  3619,  3736,},
/*15��*/  {154,   157,   168,   183,   217,   284,   410,   619,   941,   1367,  1835,  2330,  2750,  3113,  3370,  3555,  3676,  3780,},
/*20��*/  {155,   158,   176,   197,   237,   320,   477,   750,   1121,  1604,  2075,  2540,  2919,  3236,  3442,  3611,  3736,  3825,},
/*25��*/  {155,   160,   183,   210,   264,   375,   579,   901,   1338,  1855,  2298,  2727,  3055,  3342,  3539,  3676,  3780,  3853,},
/*30��*/  {155,   161,   191,   230,   301,   451,   696,   1062,  1537,  2075,  2502,  2894,  3204,  3427,  3595,  3719,  3807,  3871,},
/*35��*/  {156,   164,   202,   250,   344,   508,   803,   1226,  1728,  2238,  2705,  3055,  3302,  3501,  3652,  3754,  3834,  3899,},
/*40��*/  {156,   166,   214,   284,   397,   619,   985,   1429,  1936,  2430,  2820,  3143,  3370,  3547,  3685,  3780,  3853,  3909,},
/*45��*/  {157,   171,   237,   315,   477,   750,   1154,  1604,  2101,  2540,  2945,  3236,  3442,  3603,  3728,  3816,  3871,  3918,},
/*50��*/  {158,   174,   253,   354,   545,   865,   1328,  1835,  2298,  2727,  3055,  3329,  3516,  3660,  3762,  3843,  3895,  3938,},
/*55��*/  {159,   179,   278,   401,   634,   941,   1524,  2027,  2502,  2844,  3173,  3398,  3563,  3693,  3789,  3862,  3909,  3947,},
/*60��*/  {160,   184,   301,   457,   739,   1035,  1711,  2209,  2662,  2999,  3275,  3456,  3611,  3728,  3807,  3871,  3918,  3957,},
};
#define TEMP_INTERVAL_VAL   5
#define HUMI_INTERVAL_VAL   5
#define HUMI_MAX_VAL        95
#define HUMI_MIN_VAL        10
#define TEMP_MAX_VAL        60

static void GpioInit(void)
{
	rcu_periph_clock_enable(RCU_GPIOC);
	gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_MAX, GPIO_PIN_4);    // ADCͨ��14
	
	rcu_periph_clock_enable(RCU_GPIOB);
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1);  // ������Դ����
	gpio_bit_write(GPIOB, GPIO_PIN_0, (bit_status)1);
	gpio_bit_write(GPIOB, GPIO_PIN_1, (bit_status)0);
}

static void AdcInit(void)
{
	/* ʹ��ʱ�ӣ�*/
	rcu_periph_clock_enable(RCU_ADC1);
	/* ���÷�Ƶϵ����*/
	rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV6);  // 6��Ƶ��120MHz / 6 = 20MHz
	/* ���ö���ģʽ��*/
	adc_mode_config(ADC_MODE_FREE);
	/* ���õ���ģʽ��*/ 
	adc_special_function_config(ADC1, ADC_CONTINUOUS_MODE, DISABLE);
	/* �������ݶ��룻*/
	adc_data_alignment_config(ADC1, ADC_DATAALIGN_RIGHT);
	/* ����ת��ͨ��������*/ 
	adc_channel_length_config(ADC1, ADC_REGULAR_CHANNEL, 1);
	/* ����ת����һ��ͨ���Լ���������λ�ã�*/ 
	adc_regular_channel_config(ADC1, 0, ADC_CHANNEL_14, ADC_SAMPLETIME_71POINT5);  // PC4��Ӧͨ��14���������мĴ�����0�����У�71.5������
	/* ����ѡ����һ���ⲿ����Դ��*/ 
	adc_external_trigger_source_config(ADC1, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE);
	/* ʹ���ⲿ������*/ 
	adc_external_trigger_config(ADC1, ADC_REGULAR_CHANNEL, ENABLE);
	/* ʹ��Ӳ���˲���*/ 
//	adc_oversample_mode_config(ADC1, ADC_OVERSAMPLING_ALL_CONVERT, ADC_OVERSAMPLING_SHIFT_4B, ADC_OVERSAMPLING_RATIO_MUL16);
//	adc_oversample_mode_enable(ADC1);
	/* ʹ��ADC��*/ 
	adc_enable(ADC1);
	/* �ڲ�У׼��*/ 
	DelayNus(50);
	adc_calibration_enable(ADC1);
}

static void TimerInit(uint32_t periodUs)
{
	/* ʹ�ܶ�ʱ��ʱ�ӣ�*/
	rcu_periph_clock_enable(RCU_TIMER4);
	/* ��λ��ʱ����*/
	timer_deinit(TIMER4);
	
	timer_parameter_struct timerInitPara;
	timer_struct_para_init(&timerInitPara);
	/* ����Ԥ��Ƶ��ֵ��*/
	timerInitPara.prescaler = 120 - 1;     // �������������ʱ��Ƶ��Ϊ1Mhz������1us
	/* �����Զ���װ��ֵ��*/ 
	timerInitPara.period = periodUs - 1;
	timer_init(TIMER4, &timerInitPara);
	
	/* ʹ�ܶ�ʱ���ļ��������жϣ�*/
	timer_interrupt_enable(TIMER4, TIMER_INT_UP);
	/* ʹ�ܶ�ʱ���жϺ����ȼ���*/
	nvic_irq_enable(TIMER4_IRQn, 0, 0);
	/* ʹ�ܶ�ʱ����*/ 
	timer_enable(TIMER4);
}


/**
***********************************************************
* @brief ʪ�ȴ�����������ʼ��
* @param
* @return 
***********************************************************
*/
void HumiDrvInit(void)
{
	GpioInit();
	AdcInit();
	TimerInit(1000);
}

static uint16_t GetAdcVal(void)
{
	adc_software_trigger_enable(ADC1, ADC_REGULAR_CHANNEL);
    /* �ȴ�ת����ɱ�־ */
    while(!adc_flag_get(ADC1, ADC_FLAG_EOC));

    /* ��ȡ���ݼĴ��� */
    return (adc_regular_data_read(ADC1));
}

static void AcPowerProc(void)
{
	gpio_bit_write(GPIOB, GPIO_PIN_0, (FlagStatus)((FlagStatus)1 - gpio_output_bit_get(GPIOB, GPIO_PIN_0)));
	gpio_bit_write(GPIOB, GPIO_PIN_1, (FlagStatus)((FlagStatus)1 - gpio_output_bit_get(GPIOB, GPIO_PIN_1)));
}


void TIMER4_IRQHandler(void)
{
	static uint16_t g_timCount = 0;
	if (timer_interrupt_flag_get(TIMER4, TIMER_INT_FLAG_UP) == SET)
	{
		timer_interrupt_flag_clear(TIMER4, TIMER_INT_FLAG_UP);
		g_timCount++;
//		if (g_timCount == 1)
//		{
//			g_adcVal = GetAdcVal();
//		}
//		if (g_timCount == 2)
//		{
//			AcPowerProc();
//		}
//		if (g_timCount == 4)
//		{
//			AcPowerProc();
//			g_timCount = 0;
//		}
		if (g_timCount % 2 == 0)     // ÿ2ms��תIO��
		{
			AcPowerProc();
		}
		if (g_timCount % 400 == 1)  // ÿ��400ms���ڸߵ�ƽ�м�λ�û�ȡһ��ADC������
		{
			g_adcVal = GetAdcVal();
		}
	}
}

static void CalcHumiAdcByTemp(uint16_t *arr, uint8_t temp)
{
	uint8_t tempRow = temp / TEMP_INTERVAL_VAL;
	if (temp % TEMP_INTERVAL_VAL == 0)
	{
		memcpy(arr, &g_rhADCVal[tempRow][0], HUMI_LEVEL_NUM * sizeof(uint16_t));
		return;
	}
	

	for (uint8_t i = 0; i < HUMI_LEVEL_NUM; i++)
	{
		arr[i] = g_rhADCVal[tempRow][i] + 
		(g_rhADCVal[tempRow + 1][i] - g_rhADCVal[tempRow][i]) * (temp % TEMP_INTERVAL_VAL) / TEMP_INTERVAL_VAL;
	}

}

static int32_t AsceBinarySearch(const uint16_t *arr, int32_t size, uint16_t key) 
{
	int32_t left = 0;              			
	int32_t right = size - 1;       		
	int32_t mid;
	int32_t index = size - 1;
	while (left <= right)             		
	{
		mid = left + (right - left) / 2; 
		if (key <= arr[mid])
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

static uint8_t AdcToHumi(uint16_t *arr, uint16_t adcVal)
{
	int32_t index = AsceBinarySearch(arr, HUMI_LEVEL_NUM, adcVal);
	if (index == 0)
	{
		return HUMI_MIN_VAL;
	}
	uint8_t humi = HUMI_MIN_VAL + (index - 1) * HUMI_INTERVAL_VAL +  
					HUMI_INTERVAL_VAL * (adcVal - arr[index - 1]) / (arr[index] - arr[index - 1]);
	return humi;
}
/**
***********************************************************
* @brief ��������ת��ʪ�ȴ���������
* @param
* @return 
***********************************************************
*/
void HumiSensorProc(uint8_t temp)
{
	if (temp > TEMP_MAX_VAL)
	{
		temp = TEMP_INTERVAL_VAL;
	}
	
	uint16_t humiAdcBuf[HUMI_LEVEL_NUM];
	CalcHumiAdcByTemp(humiAdcBuf, temp);
	g_humiData = AdcToHumi(humiAdcBuf, g_adcVal);
}

/**
***********************************************************
* @brief ��ȡʪ�ȴ���������
* @param
* @return ʪ�����ݣ�����
***********************************************************
*/
float GetHumiData(void)
{
	return g_humiData;
}
