#include <stdio.h>
#include <stdint.h>
#include "gd32f30x.h"
#include "usb2Com_drv.h"
#include "led_drv.h"
#include "queue.h"

/**
***********************************************************************
包格式：帧头0  帧头1  数据长度  功能字   LED编号  亮/灭  异或校验数据
        0x55   0xAA    0x03      0x06     0x00     0x01      0xFB
***********************************************************************
*/
#define FRAME_HEAD_0 0x55
#define FRAME_HEAD_1 0xAA
#define PACKET_DATA_LEN_MIN 7  // 最小包长度
#define PACKET_DATA_LEN_MAX 11 // 最大包长度
#define FUNC_DATA_IDX 3      // 功能字数组下标
#define LED_CTRL_CODE 0x06   // 功能字 

#define MAX_BUFF_SIZE 77
static uint8_t g_rcvDataBuff[MAX_BUFF_SIZE];
static QueueType_t g_rcvQueue;

typedef struct 
{
    uint8_t LedNo;
    uint8_t LedState;
}LedCtrlInfo_t;


static void ProcUartData(uint8_t data)
{
 QueuePush(&g_rcvQueue,data);
}

/**
***********************************************************
* @brief 对数据进行异或运算
* @param data,存储数组的首地址
* @param len,要计算元素的个数
* @return 异或运算结果
***********************************************************
*/
static uint8_t CalXorSum(uint8_t *data,uint32_t len)
{
    uint8_t CalXorSum = 0;
    for(uint32_t i = 0; i < len; i++)
    {
        CalXorSum ^= data[i];
    }
    return CalXorSum;
}

/**
***********************************************************
* @brief LED控制处理函数
* @param ctrlData，结构体指针，传入LED的编号和状态
* @return 
***********************************************************
*/
static void CtrlLed(LedCtrlInfo_t *ctrlData)
{
   ctrlData->LedState != 0 ? TurnOnLed(ctrlData->LedNo) : TurnOffLed(ctrlData->LedNo);
}

/**
***********************************************************
* @brief USB转串口任务处理函数
* @param
* @return 
***********************************************************
*/
void Usb2ComTask(void)
{
    uint8_t readBuffer[PACKET_DATA_LEN_MAX] = {0};

    while (QueuePop(&g_rcvQueue,&readBuffer[0]) == QUEUE_OK)
    {
        if(readBuffer[0] != FRAME_HEAD_0)
        {
            continue;
        }
        if((QueuePop(&g_rcvQueue,&readBuffer[1]) == QUEUE_EMPTY) || (readBuffer[1] != FRAME_HEAD_1)) 
        {
            continue;
        }
        if((QueuePop(&g_rcvQueue,&readBuffer[2]) == QUEUE_EMPTY) || (readBuffer[2] > PACKET_DATA_LEN_MAX)) 
        {
            continue;
        }
        if(QueuePopArray(&g_rcvQueue,&readBuffer[3],readBuffer[2] + 1) != readBuffer[2] + 1)
        {
            continue;
        }
        if(CalXorSum(readBuffer,readBuffer[2] + 3) != readBuffer[readBuffer[2] + 3])        //这里要注意以下异或的位置
        {
            continue;
        }
        if (readBuffer[FUNC_DATA_IDX] == LED_CTRL_CODE)
        {
            CtrlLed((LedCtrlInfo_t *)(&readBuffer[FUNC_DATA_IDX + 1]));
        }
        for(uint32_t i = 0; i < readBuffer[2] + 4; i++)
        {
            printf("%02x ",readBuffer[i]);
        }
        printf("\n");
    }
}

/**
***********************************************************
* @brief USB转串口应用初始化函数
* @param
* @return 
***********************************************************
*/
void Usb2ComAppInit(void)
{
	regUsb2ComCb(ProcUartData);
    QueueInit(&g_rcvQueue,g_rcvDataBuff,MAX_BUFF_SIZE);
}
