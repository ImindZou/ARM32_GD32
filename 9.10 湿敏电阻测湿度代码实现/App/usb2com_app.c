#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "usb2com_drv.h"
#include "led_drv.h"
#include "queue.h"

/**
***********************************************************************
包格式：帧头0  帧头1  数据长度  功能字   LED编号  亮/灭  异或校验数据
        0x55   0xAA    0x03      0x06     0x00     0x01      0xFB
***********************************************************************
*/
#define FRAME_HEAD_0           0x55  
#define FRAME_HEAD_1           0xAA
#define PACKET_DATA_LEN_MIN    7                  //最小包长度
#define PACKET_DATA_LEN_MAX    11                 //最大包长度
#define FUNC_DATA_IDX          3                  //功能字数组下标
#define LED_CTRL_CODE          0x06               //功能字

#define MAX_BUF_SIZE           77
static uint8_t g_rcvDataBuf[MAX_BUF_SIZE];
static QueueType_t g_rcvQueue;

typedef struct
{
	uint8_t ledNo;
	uint8_t ledState;
} LedCtrlInfo_t;

static void ProcUartData(uint8_t data)
{
	QueuePush(&g_rcvQueue, data);
}

/**
***********************************************************
* @brief 对数据进行异或运算
* @param data, 存储数组的首地址
* @param len, 要计算的元素的个数
* @return 异或运算结果
***********************************************************
*/
static uint8_t CalXorSum(const uint8_t *data, uint32_t len)
{
	uint8_t xorSum = 0;
	for (uint32_t i = 0; i < len; i++)
	{
		xorSum ^= data[i];
	}
	return xorSum;
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
	ctrlData->ledState != 0 ? TurnOnLed(ctrlData->ledNo) : TurnOffLed(ctrlData->ledNo);
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
	uint8_t readBuf[PACKET_DATA_LEN_MAX] = {0};
	
	while (QueuePop(&g_rcvQueue, &readBuf[0]) == QUEUE_OK)
	{
		if (readBuf[0] != FRAME_HEAD_0)
		{
			continue;
		}
		if ((QueuePop(&g_rcvQueue, &readBuf[1]) == QUEUE_EMPTY) || (readBuf[1] != FRAME_HEAD_1))
		{
			continue;
		}
		if ((QueuePop(&g_rcvQueue, &readBuf[2]) == QUEUE_EMPTY) || (readBuf[2] > PACKET_DATA_LEN_MAX))
	    {
			continue;
		}
		if (QueuePopArray(&g_rcvQueue, &readBuf[3], readBuf[2] + 1) != readBuf[2] + 1)
		{
			continue;
		}
		if (CalXorSum(readBuf, readBuf[2] + 3) != readBuf[readBuf[2] + 3])
		{
			continue;
		}
		if (g_rcvDataBuf[FUNC_DATA_IDX] == LED_CTRL_CODE)
		{
			CtrlLed((LedCtrlInfo_t *)(&readBuf[FUNC_DATA_IDX + 1]));
		}
		
		for (uint32_t i = 0; i < readBuf[2] + 4; i++)
		{
			printf("%02x ", readBuf[i]);
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
	QueueInit(&g_rcvQueue, g_rcvDataBuf, MAX_BUF_SIZE);
}
