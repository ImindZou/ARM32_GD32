/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-14 00:34:45
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-24 17:56:20
 * @FilePath: \6.2 ???????????????????\App\main.c
 * @Description: ???????????,??????`customMade`, ??koroFileHeader?????? ????????: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdint.h>
#include <stdio.h>
#include "led_drv.h"
#include "key_drv.h"
#include "systick.h"
#include "usb2com_drv.h"
#include "delay.h"
#include "usb2com_app.h"
#include "hmi_app.h"
#include "vres_drv.h"

typedef struct
{
	uint8_t run;                // ���ȱ�־��1�����ȣ�0������
	uint16_t timCount;          // ʱ��Ƭ����ֵ
	uint16_t timRload;          // ʱ��Ƭ����ֵ
	void (*pTaskFuncCb)(void);  // ����ָ���������������ҵ����ģ�麯����ַ
} TaskComps_t;

static TaskComps_t g_taskComps[] = 
{
	{0, 5,   5,   HmiTask},
	{0, 200, 200, Usb2ComTask},
	/* ���ҵ����ģ�� */
};

#define TASK_NUM_MAX   (sizeof(g_taskComps) / sizeof(g_taskComps[0]))

static void TaskHandler(void)
{
	for (uint8_t i = 0; i < TASK_NUM_MAX; i++)
	{
		if (g_taskComps[i].run)                  // �ж�ʱ��Ƭ��־
		{
			g_taskComps[i].run = 0;              // ��־����
			g_taskComps[i].pTaskFuncCb();        // ִ�е���ҵ����ģ��
		}
	}
}

/**
***********************************************************
* @brief �ڶ�ʱ���жϷ������б���ӵ��ã�����ʱ��Ƭ��ǣ�
         ��Ҫ��ʱ��1ms����1���ж�
* @param
* @return 
***********************************************************
*/
static void TaskScheduleCb(void)
{
	for (uint8_t i = 0; i < TASK_NUM_MAX; i++)
	{
		if (g_taskComps[i].timCount)
		{
			g_taskComps[i].timCount--;
			if (g_taskComps[i].timCount == 0)
			{
				g_taskComps[i].run = 1;
				g_taskComps[i].timCount = g_taskComps[i].timRload;
			}
		}
	}
}

static void DrvInit(void)
{
	SystickInit();
	LedDrvInit();
	KeyDrvInit();
	DelayInit();
	Usb2ComDrvInit();
	VresDrvInit();
}
static void AppInit(void)
{
	Usb2ComAppInit();
	TaskScheduleCbReg(TaskScheduleCb);
}

#include <stdlib.h>

typedef struct
{
	uint16_t id;
	uint8_t score;
} StuInfo_t;

// /**
// ***************************************
// *@brief ���������㷨������
// *@param _a,Ŀ��Ԫ��1
// *@param _b,Ŀ��Ԫ��2
// *@return val,�Ƚ�ֵ
// ***************************************
// */
// int32_t CmpCb(const void *_a, const void *_b)	//����
// {
// 	uint16_t *a = (uint16_t *)_a;
// 	uint16_t *b = (uint16_t *)_b;
// 	int32_t val = 0;
// 	if(*a > *b)
// 	{
// 		val = 1;
// 	}
// 	else if (*a < *b)
// 	{
// 		val = -1;
// 	}
// 	else
// 	{
// 		val = 0;
// 	}
// 	return val;
// }

// /**
// ***************************************
// *@brief ���������㷨������
// *@param _a,Ŀ��Ԫ��1
// *@param _b,Ŀ��Ԫ��2
// *@return val,�Ƚ�ֵ
// ***************************************
// */
// int32_t StuCmpCb(const void *_a, const void *_b)	//����
// {
// 	StuInfo_t *a = (StuInfo_t *)_a;
// 	StuInfo_t *b = (StuInfo_t *)_b;
// 	int32_t val = 0;
// 	if(a->score > b->score)
// 	{
// 		val = -1;
// 	}
// 	else if (a->score < b->score)
// 	{
// 		val = 1;
// 	}
// 	else
// 	{
// 		val = 0;
// 	}
// 	return val;
// }

/**
***************************************
*@brief ���ֲ��ҷ�������
*@param	arr���������� 
*@param size,�����Ԫ�ظ���
*@param key,Ŀ��ֵ
*@return mid,Ŀ��ֵ / -1��δ��ѯ��
***************************************
*/
int32_t AsecBinarySearch(int32_t *arr, int32_t size, int32_t key)   //��Ӧ��������
{
    int32_t left = 0;   //�������±�
    int32_t right = size - 1;   //�������±�
    int32_t mid;       
    while(left <= right)    //ѭ����ֹ������ left == right + 1,��������Ϊ�գ���ֹѭ��
    {
        mid = left + (right - left) / 2;    //�����м��±꣬��ֹ��left + right�����
        if(key < arr[mid])  //���ָ��Ԫ�ر��м��±��ֵС
            right = mid - 1;    //����м��±�-1�����±�
        else if(key > arr[mid])     //���ָ��Ԫ�ر��м��±��ֵ��
            left = mid + 1;
        else 
            return mid;
    }
    return -1;      //����-1���ʾ��������δ�ҵ���ָ��Ԫ��
}

/**
***************************************
*@brief ���ֲ��ҷ�������
*@param	arr���������� 
*@param size,�����Ԫ�ظ���
*@param key,Ŀ��ֵ
*@return mid,Ŀ��ֵ / -1��δ��ѯ��
***************************************
*/
int32_t DsecBinarySearch(int32_t *arr, int32_t size, int32_t key)   //��Ӧ��������
{
    int32_t left = 0;   //�������±�
    int32_t right = size - 1;   //�������±�
    int32_t mid;       
    while(left <= right)    //ѭ����ֹ������ left == right + 1,��������Ϊ�գ���ֹѭ��
    {
        mid = left + (right - left) / 2;    //�����м��±꣬��ֹ��left + right�����
        if(key > arr[mid])  //���ָ��Ԫ�ر��м��±��ֵС
            right = mid - 1;    //����м��±�-1�����±�
        else if(key < arr[mid])     //���ָ��Ԫ�ر��м��±��ֵ��
            left = mid + 1;
        else 
            return mid;
    }
    return -1;      //����-1���ʾ��������δ�ҵ���ָ��Ԫ��
}

/**
***************************************
*@brief ���ֲ��ҷ�������,���ҽӽ�ֵ
*@param	arr���������� 
*@param size,�����Ԫ�ظ���
*@param key,Ŀ��ֵ
*@return index,�ӽ�ֵ
***************************************
*/
int32_t AsecNear(int32_t *arr, int32_t size, int32_t key)	//��Ӧ��������
{
	int32_t left = 0;
	int32_t right = size - 1;
	int32_t mid;
	int32_t index = size - 1;
	while (left <= right)
	{
		mid = left + (right - left) / 2;
		if(key <= arr[mid])
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

/**
***************************************
*@brief ���ֲ��ҷ�������,���ҽӽ�ֵ
*@param	arr���������� 
*@param size,�����Ԫ�ظ���
*@param key,Ŀ��ֵ
*@return index,�ӽ�ֵ
***************************************
*/
int32_t DsecNear(int32_t *arr, int32_t size, int32_t key)	 //��Ӧ��������
{
	int32_t left = 0;
	int32_t right = size - 1;
	int32_t mid;
	int32_t index = size - 1;
	while (left <= right)
	{
		mid = left + (right - left) / 2;
		if(key >= arr[mid])
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

int main(void)
{	
	DrvInit();
// 	uint16_t num[10] = {5,7,8,1,26,52,9,3,14,56};
// 	uint16_t sz = sizeof(num) / sizeof(num[0]);
// //	qsort(num,sz,sizeof(num[0]),CmpCb);
// //	for(uint8_t i = 0; i < 10; i++)
// //	{
// //		printf("num[%d] = %d.\n",i,num[i]);
// //	}
	
	// StuInfo_t stuInfo[5] = {
	// 	{1,90},
	// 	{2,65},
	// 	{3,80},
	// 	{4,75},
	// 	{5,25}
	// };
	// qsort(stuInfo,5,sizeof(stuInfo[0]),StuCmpCb);
	// for(uint8_t i = 0; i < 5; i++)
	// {
	// 	printf("student id = %d, score = %d\n",stuInfo[i].id,stuInfo[i].score);
	// } 
	
	// AppInit();
	int32_t arr[6] = {1,3,7,10,14,20};
	int32_t index;
	// index = AsecBinarySearch(arr,6,20);
	index = AsecNear(arr,6,21);
	printf("20 index = %d.\n",index);
	index = AsecNear(arr, 6, 11);
	printf("14 index = %d.\n", index);
	index = AsecNear(arr, 6, 0);
	printf("0 index = %d.\n", index);
	int32_t arr2[6] = {20,14,10,7,3,1};
	// index = DsecBinarySearch(arr2,6,20);
	// index = DsecNear(arr2,6,21);
	// printf("20 index = %d.\n",index);
	// index = DsecNear(arr2, 6, 15);
	// printf("14 index = %d.\n", index);
	// index = DsecNear(arr2, 6, 0);
	// printf("0 index = %d.\n", index);
	while (1)
	{
		// TaskHandler();
		// VresDrvTest();
	}
}

