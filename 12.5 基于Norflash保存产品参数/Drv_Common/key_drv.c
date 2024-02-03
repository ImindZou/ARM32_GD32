#include <stdint.h>
#include "gd32f30x.h"
#include "systick.h"

typedef struct
{
	rcu_periph_enum rcu;
	uint32_t gpio;
	uint32_t pin;
} Key_GPIO_t;

static Key_GPIO_t g_gpioList[] = 
{
	{RCU_GPIOA, GPIOA, GPIO_PIN_0},  // key1
	{RCU_GPIOG, GPIOG, GPIO_PIN_13}, // key2
	{RCU_GPIOG, GPIOG, GPIO_PIN_14}  // key3
};

#define KEY_NUM_MAX (sizeof(g_gpioList) / sizeof(g_gpioList[0]))

typedef enum
{
	KEY_RELEASE = 0, // 释放松开
	KEY_CONFIRM,     // 消抖确认
	KEY_SHORTPRESS,  // 短按
	KEY_LONGPRESS    // 长按
} KEY_STATE;

#define CONFIRM_TIME   10    // 按键消抖时间窗10ms
#define LONGPRESS_TIME 1000  // 长按时间窗1000ms

typedef struct
{
	KEY_STATE keyState;
	uint64_t prvSysTime;
} Key_Info_t;

static Key_Info_t g_keyInfo[KEY_NUM_MAX];
/**
***********************************************************
* @brief 按键硬件初始化
* @param
* @return 
***********************************************************
*/
void KeyDrvInit(void)
{
	for (uint8_t i = 0; i < KEY_NUM_MAX; i++)
	{
		rcu_periph_clock_enable(g_gpioList[i].rcu);
		gpio_init(g_gpioList[i].gpio, GPIO_MODE_IN_FLOATING,
				  GPIO_OSPEED_2MHZ, g_gpioList[i].pin);
	}
}

static uint8_t KeyScan(uint8_t keyIndex)
{
	uint64_t curSysTime;
	uint8_t keyPress;
	
	keyPress = gpio_input_bit_get(g_gpioList[keyIndex].gpio, g_gpioList[keyIndex].pin);

	switch (g_keyInfo[keyIndex].keyState)
	{
		case KEY_RELEASE:                                          //  释放状态：判断有无按键按下
			if (!keyPress)                                         //  有按键按下
			{ 
				g_keyInfo[keyIndex].keyState = KEY_CONFIRM;        //  获取系统运行时间
				g_keyInfo[keyIndex].prvSysTime = GetSysRunTime();
			}
			break;
			
		case KEY_CONFIRM:
			if (!keyPress)
			{
				curSysTime = GetSysRunTime();
				if (curSysTime - g_keyInfo[keyIndex].prvSysTime > CONFIRM_TIME) //  超过时间窗，切换到短按
				{
					g_keyInfo[keyIndex].keyState = KEY_SHORTPRESS;
				}
			}
			else
			{
				g_keyInfo[keyIndex].keyState = KEY_RELEASE;
			}
			break;
			
		case KEY_SHORTPRESS:           //  短按状态：继续判定是短按，还是长按
			if (keyPress)
			{
				g_keyInfo[keyIndex].keyState = KEY_RELEASE;
				return (keyIndex + 1); //  返回按键码值，三个按键短按对应0x01, 0x02, 0x03
			}
			else
			{
				curSysTime = GetSysRunTime();
				if (curSysTime - g_keyInfo[keyIndex].prvSysTime > LONGPRESS_TIME)
				{	
					g_keyInfo[keyIndex].keyState = KEY_LONGPRESS;
				}
			}
			break;
		case KEY_LONGPRESS:
			if (keyPress)
			{
				g_keyInfo[keyIndex].keyState = KEY_RELEASE;
				return (keyIndex + 0x81); //返回按键码值，三个按键长按对应0x81, 0x82, 0x83
			}
			break;
		default:
			g_keyInfo[keyIndex].keyState = KEY_RELEASE;
			break;
	}
	return 0;
}
/**
***********************************************************
* @brief 获取按键码值
* @param
* @return 三个按键码值，短按0x01 0x02 0x03，
			长按0x81 0x82 0x83，没有按下为0
***********************************************************
*/
uint8_t GetKeyVal(void)
{
	uint8_t res = 0;

	for (uint8_t i = 0; i < KEY_NUM_MAX; i++)
	{
		res = KeyScan(i);
		if (res != 0)
		{
			break;
		}
	}
	return res;
}

