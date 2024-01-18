/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-18 16:19:57
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-18 20:09:31
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
    /*使能时钟*/
    rcu_periph_clock_enable(RCU_ADC0);
    /*设置分频系数*/
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV6);    //8???120MHz / 6 = 20 MHz
    /*设置独立模式*/ 
    adc_mode_config(ADC_MODE_FREE);
    /*设置单次模式*/ 
    adc_special_function_config(ADC0,ADC_CONTINUOUS_MODE,ENABLE);   //continue convert mode
    /*设置数据对齐*/ 
    adc_data_alignment_config(ADC0,ADC_DATAALIGN_RIGHT);
    /*设置转换通道个数*/
    adc_channel_length_config(ADC0,ADC_REGULAR_CHANNEL,1);
    /*设置转换哪一个通道以及所处序列位置*/ 
    adc_regular_channel_config(ADC0,0,ADC_CHANNEL_12,ADC_SAMPLETIME_239POINT5); //PC2????12?????????0????239.5???
    /*设置选择哪一个外部触发源*/
    adc_external_trigger_source_config(ADC0,ADC_REGULAR_CHANNEL,ADC0_1_2_EXTTRIG_REGULAR_NONE);
    /*使能外部触发*/
    adc_external_trigger_config(ADC0,ADC_REGULAR_CHANNEL,ENABLE);
    /*使能ADC*/
    adc_enable(ADC0);
    /*内部校准*/
    DelayNus(50);
    adc_calibration_enable(ADC0);
    adc_software_trigger_enable(ADC0,ADC_REGULAR_CHANNEL);

}

/**
***************************************
*@brief ADC硬件初始化
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
    // adc_software_trigger_enable(ADC0,ADC_REGULAR_CHANNEL);
    while (!adc_flag_get(ADC0,ADC_FLAG_EOC));    //连续转换模式下无需每次查询跟触发
    return adc_regular_data_read(ADC0);     //但是加上查询第一次读不是为0
}

void VresDrvTest(void)
{
    uint16_t Adcval = GetAdcVal();
    float Voltage = (float)Adcval / 4095 *3.3;
    printf("AdcVal = %d, Voltage = %.1f.\n",Adcval,Voltage);
    DelayNms(1000);
}



