/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-09 16:59:20
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-10 00:37:15
 * @FilePath: \2.11 流水灯的软件架构和代码实现\Driver\led_drv.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdint.h>
#include "gd32f30x.h"

typedef struct
{
    rcu_periph_enum rcu;
    uint32_t gpio;
    uint32_t pin;
}Led_GPIO_t;

static Led_GPIO_t g_gpioList[] = 
{
    {RCU_GPIOA,GPIOA,GPIO_PIN_8},
    {RCU_GPIOE,GPIOE,GPIO_PIN_6},
    {RCU_GPIOF,GPIOF,GPIO_PIN_6}
};

/*获取数组占空间的大小*/
#define LED_NUM_MAX (sizeof(g_gpioList) / sizeof(g_gpioList[0]))

/*遍历每个gpio的元素进行初始化*/
void LedDrvInit(void)
{
    for(uint8_t i = 0; i < LED_NUM_MAX; i++)
    {
        rcu_periph_clock_enable(g_gpioList[i].rcu);
        gpio_init(g_gpioList[i].gpio,GPIO_MODE_OUT_PP,GPIO_OSPEED_2MHZ,g_gpioList[i].pin);
        gpio_bit_reset(g_gpioList[i].gpio,g_gpioList[i].pin);
    }
}

/**
***************************************
*@brief 点亮LED 
*@param ledNo,LED标号，0~2
*@return 
***************************************
*/
void TurnOnLed(uint8_t ledNo)
{
    if(ledNo >= LED_NUM_MAX)
    {
        return;
    }
    gpio_bit_set(g_gpioList[ledNo].gpio,g_gpioList[ledNo].pin);
}

/**
***************************************
*@brief 熄灭LED 
*@param ledNo,LED标号，0~2 
*@return 
***************************************
*/
void TurnOffLed(uint8_t ledNo)
{
    if(ledNo >= LED_NUM_MAX)
    {
        return;
    }
    gpio_bit_reset(g_gpioList[ledNo].gpio,g_gpioList[ledNo].pin);
}

/**
***************************************
*@brief LED状态取反 
*@param ledNo,LED标号，0~2 
*@return 
***************************************
*/
void ToggleLed(uint8_t ledNo)
{
    FlagStatus bit_status;
    bit_status = gpio_input_bit_get(g_gpioList[ledNo].gpio,g_gpioList[ledNo].pin);
    bit_status = (FlagStatus)(1 - bit_status);
    gpio_bit_write(g_gpioList[ledNo].gpio,g_gpioList[ledNo].pin,bit_status);
}



