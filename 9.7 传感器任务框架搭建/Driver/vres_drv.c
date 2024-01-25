/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-18 16:19:57
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-18 21:56:35
 * @FilePath: \9.2 ADC???????\Driver\vres_drv.c
 * @Description: ??????,???`customMade`, ??koroFileHeader???? ????: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdint.h>
#include "gd32f30x.h"
#include "delay.h"
#include <stdio.h>
#include <string.h>

static void GpioInit(void)
{
    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_init(GPIOC,GPIO_MODE_AIN,GPIO_OSPEED_10MHZ,GPIO_PIN_2);
    // gpio_init(GPIOC,GPIO_MODE_AIN,GPIO_OSPEED_10MHZ,GPIO_PIN_3);
}

static void AdcInit(void)
{
    /*使能时钟*/
    rcu_periph_clock_enable(RCU_ADC0);
    /*设置分频系数*/
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV6);    //8???120MHz / 6 = 20 MHz
    /*设置独立模式*/ 
    adc_mode_config(ADC_MODE_FREE);
    /*设置连续模式*/ 
    adc_special_function_config(ADC0,ADC_CONTINUOUS_MODE,ENABLE);   //continue convert mode
     /*设置扫描模式*/ 
    adc_special_function_config(ADC0,ADC_SCAN_MODE,ENABLE);   //Scan Mode
    /*设置数据对齐*/ 
    adc_data_alignment_config(ADC0,ADC_DATAALIGN_RIGHT);
    /*设置转换通道个数*/
    adc_channel_length_config(ADC0,ADC_REGULAR_CHANNEL,1);
    /*设置转换哪一个通道以及所处序列位置*/ 
    adc_regular_channel_config(ADC0,0,ADC_CHANNEL_12,ADC_SAMPLETIME_239POINT5); //PC2????12?????????0????239.5???
    // adc_regular_channel_config(ADC0,1,ADC_CHANNEL_13,ADC_SAMPLETIME_239POINT5); //PC2????12?????????0????239.5???
    /*设置选择哪一个外部触发源*/
    adc_external_trigger_source_config(ADC0,ADC_REGULAR_CHANNEL,ADC0_1_2_EXTTRIG_REGULAR_NONE);
    /*使能外部触发*/
    adc_external_trigger_config(ADC0,ADC_REGULAR_CHANNEL,ENABLE);
    /*使能ADC的DMA功能*/
    adc_dma_mode_enable(ADC0);
    /*使能ADC*/
    adc_enable(ADC0);
    /*内部校准*/
    DelayNus(50);
    adc_calibration_enable(ADC0);
    adc_software_trigger_enable(ADC0,ADC_REGULAR_CHANNEL);

}

#define ADC0_RDATA_ADDR     (ADC0 + 0x4C);
#define MAX_BUF_SIZE        10

static uint16_t g_adcVal[2];

static void DmaInit(void)
{
	/* 使能DMA时钟；*/
	rcu_periph_clock_enable(RCU_DMA0);
	/* 复位DMA通道；*/
	dma_deinit(DMA0,DMA_CH0);
 
	dma_parameter_struct dmaStruct;
    dma_struct_para_init(&dmaStruct);
	/* 配置传输方向；*/ 
	dmaStruct.direction = DMA_PERIPHERAL_TO_MEMORY;
	/* 配置数据源地址；*/ 
	dmaStruct.periph_addr = ADC0_RDATA_ADDR;
	/* 配置源地址是固定的还是增长的；*/ 
	dmaStruct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	/* 配置源数据传输位宽；*/ 
	dmaStruct.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
	
	/* 配置数据目的地址；*/
	dmaStruct.memory_addr = (uint32_t)g_adcVal;
	/* 配置目的地址是固定的还是增长的；*/ 
	dmaStruct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	/* 配置目的数据传输位宽；*/ 
	dmaStruct.memory_width = DMA_MEMORY_WIDTH_16BIT;
	/* 配置数据传输最大次数；*/ 
	dmaStruct.number = MAX_BUF_SIZE;
	/* 配置DMA通道优先级；*/ 
	dmaStruct.priority = DMA_PRIORITY_HIGH;
	dma_init(DMA0,DMA_CH0, &dmaStruct);
	
	/* 使能UART接收数据使用DMA；*/ 
	dma_circulation_enable(DMA0,DMA_CH0);
	/* 使能DMA通道；*/ 
	dma_channel_enable(DMA0, DMA_CH0);
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
    DmaInit();
}

uint16_t ArithAvgFltr(uint16_t *arr,uint32_t len)
{
    uint32_t sum = 0;
    for(uint32_t i = 0; i < len; i++)
    {
        sum += arr[i];
    }
    return (uint16_t)(sum / len);
}

void VresDrvTest(void)
{
    uint16_t buf[MAX_BUF_SIZE];
    memcpy(buf,g_adcVal,sizeof(uint16_t) * MAX_BUF_SIZE);       //拷贝到要注意拷贝多少到缓存，刚刚没写他就瞎几把拷贝了，两个0
    for(uint8_t i = 0;i < MAX_BUF_SIZE;i++)
    {
        printf("AdcVal[%d]  =  %d.\n",i, buf[i]);
    }

    uint16_t res = ArithAvgFltr(buf, MAX_BUF_SIZE);
    printf("res =  %d.\n",res);
    DelayNms(1000);
}
