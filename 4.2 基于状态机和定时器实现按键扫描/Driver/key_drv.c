#include <stdint.h>
#include "gd32f30x.h"
#include "led_drv.h"
#include "systick.h"

#define CONFIRM_TIME 10 //��������ʱ�䴰��10ms
#define LONGPRESS_TIME 1000 //����ʱ�䴰��1000ms
typedef struct 
{
    rcu_periph_enum rcu;
    uint32_t gpio;
    uint32_t pin;
}Key_GPIO_t;

static Key_GPIO_t g_gpioList[] = 
{
    {RCU_GPIOA,GPIOA,GPIO_PIN_0},   //KEY1
    {RCU_GPIOG,GPIOG,GPIO_PIN_13},  //KEY2
    {RCU_GPIOG,GPIOG,GPIO_PIN_14},  //KEY3
    {RCU_GPIOG,GPIOG,GPIO_PIN_15}   //KEY4
};

#define KEY_MUM_MAX (sizeof(g_gpioList) / sizeof(g_gpioList[0]))

#if 0
static void KeyGpioInit(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIOA,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_2MHZ,GPIO_PIN_0);

    rcu_periph_clock_enable(RCU_GPIOG);
    gpio_init(GPIOG, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_2MHZ, GPIO_PIN_13);
}

static void KeyExtiInit(void)
{
    /*ʹ��EXTIʱ��*/
    rcu_periph_clock_enable(RCU_AF);
    /*I/O���ӵ�EXTI��*/
    gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOA,GPIO_PIN_SOURCE_0);
    /*��������/�½���*/
    exti_init(EXTI_0,EXTI_INTERRUPT,EXTI_TRIG_FALLING);
    /*�����־*/
    exti_interrupt_flag_clear(EXTI_0);
    /*ʹ���ж�*/
    nvic_irq_enable(EXTI0_IRQn,1,1);
	
		/*I/O���ӵ�EXTI��*/
    gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOG,GPIO_PIN_SOURCE_13);
    /*��������/�½���*/
    exti_init(EXTI_13,EXTI_INTERRUPT,EXTI_TRIG_FALLING);
    /*�����־*/
    exti_interrupt_flag_clear(EXTI_13);

    /*I/O���ӵ�EXTI��*/
    gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOG, GPIO_PIN_SOURCE_14);
    /*��������/�½���*/
    exti_init(EXTI_14, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    /*�����־*/
    exti_interrupt_flag_clear(EXTI_14);
    /*ʹ���ж�*/
    nvic_irq_enable(EXTI10_15_IRQn, 1, 0);
}
#endif
#if 0
/**
***************************************
*@brief ����Ӳ����ʼ��
*@param
*@return
***************************************
*/
void KeyDrvInit(void)
{
	KeyGpioInit();
	KeyExtiInit();
}
#endif
#if 0
/**
***************************************
*@brief EXTI0�жϺ���,��ӦPA0
*@param 
*@return 
***************************************
*/
void EXTI0_IRQHandler(void)
{
	if(exti_interrupt_flag_get(EXTI_0) != RESET)
    {
        ToggleLed(LED1);
        exti_interrupt_flag_clear(EXTI_0);
//			while(1);//�����ȼ��޷���ռ
    }
}

/**
***************************************
*@brief EXTI10_15�жϺ���,��ӦPA0
*@param 
*@return 
***************************************
*/
void EXTI10_15_IRQHandler(void)
{
	if(exti_interrupt_flag_get(EXTI_13) != RESET)
    {
        ToggleLed(LED2);
        exti_interrupt_flag_clear(EXTI_13);
    }
		if(exti_interrupt_flag_get(EXTI_14) != RESET)
    {
        ToggleLed(LED3 );
        exti_interrupt_flag_clear(EXTI_14);
    }
}
#endif
/**
***************************************
*@brief ��Ӳ����ʼ��
*@param 
*@return 
***************************************
*/
void KeyDrvInit(void)
{
    for(uint8_t i = 0; i < KEY_MUM_MAX; i++)
    {
        rcu_periph_clock_enable(g_gpioList[i].rcu);
        gpio_init(g_gpioList[i].gpio,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_2MHZ,g_gpioList[i].pin); 
    }
}

typedef enum
{
    KEY_RELEASE = 0,    //�ͷ��ɿ�
    KEY_CONFIRM,        //����ȷ��
    KEY_SHORTPRESS,     //�̰�
    KEY_LONGPRESS       //����
}KEY_STATE;

typedef struct 
{
    KEY_STATE keyState;
    uint64_t  prvSysTime;
}Key_Info_t;

static Key_Info_t g_keyInfo[KEY_MUM_MAX];

static uint8_t KeyScan(uint8_t KeyIndex)
{
    volatile uint64_t curSysTime;
    uint8_t KeyPress;

    KeyPress = gpio_input_bit_get(g_gpioList[KeyIndex].gpio, g_gpioList[KeyIndex].pin);

    switch (g_keyInfo[KeyIndex].keyState)
    {
    case KEY_RELEASE:
        if (!KeyPress)
        {
            g_keyInfo[KeyIndex].keyState = KEY_CONFIRM;
            g_keyInfo[KeyIndex].prvSysTime = GetSysRunTime();
        }
        break;
    case KEY_CONFIRM:
        if (!KeyPress)
        {
            curSysTime = GetSysRunTime();
            if (curSysTime - g_keyInfo[KeyIndex].prvSysTime > CONFIRM_TIME)
            {
                g_keyInfo[KeyIndex].keyState = KEY_SHORTPRESS;
            }
        }
        else
        {
            g_keyInfo[KeyIndex].keyState = KEY_RELEASE;
        }
        break;
    case KEY_SHORTPRESS:
        if (KeyPress) // �Ƿ��ɿ����ߵ�ƽ
        {
            g_keyInfo[KeyIndex].keyState = KEY_RELEASE;
            return (KeyIndex + 1); // ���ذ�����ֵ������������ֵ�ֱ��Ӧ0x01 0x02 0x03 0x04
        }
        else
        {
            curSysTime = GetSysRunTime();
            if (curSysTime - g_keyInfo[KeyIndex].prvSysTime > LONGPRESS_TIME)
            {
                g_keyInfo[KeyIndex].keyState = KEY_LONGPRESS;
            }
        }
        break;
    case KEY_LONGPRESS:
        if (KeyPress)
        {
            g_keyInfo[KeyIndex].keyState = KEY_RELEASE;
            return (KeyIndex + 0x81); // ���ذ�����ֵ������������ֵ�ֱ��Ӧ0x01 0x02 0x03 0x04,����0x81 0x82 0x83 0x84
        }
        break;
    default:
        g_keyInfo[KeyIndex].keyState = KEY_RELEASE;
        break;
    }
    return 0;
}

/**
***************************************
*@brief ��ȡ������ֵ
*@param 
*@return �ĸ�������ֵ���̰�0x01 0x02 0x03 0x04������0x81 0x82 0x83 0x84
***************************************
*/
uint8_t GetKeyVal(void)
{
    uint8_t res = 0;
    for(uint8_t i = 0; i < KEY_MUM_MAX; i++)
    {
        res = KeyScan(i);
        if(res != 0)
        {
            break;
        }
    }
        return res;
}

