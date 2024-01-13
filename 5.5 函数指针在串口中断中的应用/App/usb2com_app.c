#include "usb2Com_drv.h"
#include <stdio.h>
#include <stdint.h>
#include "gd32f30x.h"
#include "led_drv.h"
#include <stdbool.h>

/**
***********************************************************************
����ʽ��֡ͷ0  ֡ͷ1  ���ݳ���  ������   LED���  ��/��  ���У������
        0x55   0xAA    0x03      0x06     0x00     0x01      0xFB
***********************************************************************
*/
#define FRAME_HEAD_0 		 0x55  
#define FRAME_HEAD_1 		 0xAA
#define CTRL_DATA_LEN 		 3     //�����򳤶�
#define PACKET_DATA_LEN 	 (CTRL_DATA_LEN + 4)  //������
#define FUNC_DATA_IDX 		 3     //�����������±�
#define LED_CTRL_CODE		 0x06  //������

#define MAX_BUFF_SIZE 20
static uint8_t g_rcvDataBuff[MAX_BUFF_SIZE];
static uint8_t g_pktRcvd = false;

typedef struct 
{
    uint8_t LedNo;
    uint8_t LedState;
}LedCtrlInfo_t;


static void ProcUartData(uint8_t data)
{
    static uint8_t index = 0;
    g_rcvDataBuff[index++] = data;
    switch (index)
    {
    case 1:
        if(g_rcvDataBuff[0] != FRAME_HEAD_0)
        {
            index = 0;
        }
        break;
    case 2:
        if(g_rcvDataBuff[1] != FRAME_HEAD_1)
        {
            index = 0;
        }
        break;
    case PACKET_DATA_LEN:
        g_pktRcvd = true;
        index = 0;
        break;
    default:
        break;
    }
}

/**
***********************************************************
* @brief �����ݽ����������
* @param data,�洢������׵�ַ
* @param len,Ҫ����Ԫ�صĸ���
* @return ���������
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
* @brief LED���ƴ�����
* @param ctrlData���ṹ��ָ�룬����LED�ı�ź�״̬
* @return 
***********************************************************
*/
static void CtrlLed(LedCtrlInfo_t *ctrlData)
{
   ctrlData->LedState != 0 ? TurnOnLed(ctrlData->LedNo) : TurnOffLed(ctrlData->LedNo);
}

/**
***********************************************************
* @brief USBת������������
* @param
* @return 
***********************************************************
*/
void Usb2ComTask(void)
{
    if(!g_pktRcvd)
    {
        return;
    }
    g_pktRcvd = false;

    if(CalXorSum(g_rcvDataBuff,PACKET_DATA_LEN - 1) != g_rcvDataBuff[PACKET_DATA_LEN - 1])
    {
        return;
    }
    if(g_rcvDataBuff[FUNC_DATA_IDX] == LED_CTRL_CODE)
    {
        CtrlLed((LedCtrlInfo_t *)(&g_rcvDataBuff[FUNC_DATA_IDX + 1]));
    }

}

/**
***********************************************************
* @brief USBת����Ӧ�ó�ʼ������
* @param
* @return 
***********************************************************
*/
void Usb2ComAppInit(void)
{
	regUsb2ComCb(ProcUartData);
}
