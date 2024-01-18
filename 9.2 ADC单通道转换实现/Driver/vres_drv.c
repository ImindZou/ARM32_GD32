/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-18 16:19:57
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-18 19:44:55
 * @FilePath: \9.2 ADC???????\Driver\vres_drv.c
 * @Description: ??????,???`customMade`, ??koroFileHeader???? ????: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdint.h>
#include "gd32f30x.h"
#include "delay.h"
#include <stdio.h>

static void GpioInit(void)
{
    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_init(GPIOC,GPIO_MODE_AIN,GPIO_OSPEED_10MHZ,GPIO_PIN_2);
}

static void AdcInit(void)
{
    /*????*/
    rcu_periph_clock_enable(RCU_ADC0);
    /*??????*/
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV6);    //8???120MHz / 6 = 20 MHz
    /*??????*/
    adc_mode_config(ADC_MODE_FREE);
    /*??????*/
    adc_special_function_config(ADC0,ADC_CONTINUOUS_MODE,DISABLE);
    /*??????*/
    adc_data_alignment_config(ADC0,ADC_DATAALIGN_RIGHT);
    /*????????*/
    adc_channel_length_config(ADC0,ADC_REGULAR_CHANNEL,1);
    /*?????????????????*/
    adc_regular_channel_config(ADC0,0,ADC_CHANNEL_12,ADC_SAMPLETIME_239POINT5); //PC2????12?????????0????239.5???
    /*????????????*/
    adc_external_trigger_source_config(ADC0,ADC_REGULAR_CHANNEL,ADC0_1_2_EXTTRIG_REGULAR_NONE);
    /*????????*/
    adc_external_trigger_config(ADC0,ADC_REGULAR_CHANNEL,ENABLE);
    /*??ADC*/
    adc_enable(ADC0);
    /*????*/
    DelayNus(50);
    adc_calibration_enable(ADC0);
}

/**
***************************************
*@brief ADC?????
*@param 
*@return 
***************************************
*/
void VresDrvInit(void)
{
    GpioInit();
    AdcInit();
}


static uint16_t GetAdcVal(void)
{
    adc_software_trigger_enable(ADC0,ADC_REGULAR_CHANNEL);
    while (!adc_flag_get(ADC0,ADC_FLAG_EOC));
    return adc_regular_data_read(ADC0);
}

void VresDrvTest(void)
{
    uint16_t Adcval = GetAdcVal();
    float Voltage = (float)Adcval / 4095 *3.3;
    printf("AdcVal = %d, Voltage = %.1f.\n",Adcval,Voltage);
    DelayNms(1000);
}



