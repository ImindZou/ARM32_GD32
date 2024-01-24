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
	uint8_t run;                // 调度标志，1：调度，0：挂起
	uint16_t timCount;          // 时间片计数值
	uint16_t timRload;          // 时间片重载值
	void (*pTaskFuncCb)(void);  // 函数指针变量，用来保存业务功能模块函数地址
} TaskComps_t;

static TaskComps_t g_taskComps[] = 
{
	{0, 5,   5,   HmiTask},
	{0, 200, 200, Usb2ComTask},
	/* 添加业务功能模块 */
};

#define TASK_NUM_MAX   (sizeof(g_taskComps) / sizeof(g_taskComps[0]))

static void TaskHandler(void)
{
	for (uint8_t i = 0; i < TASK_NUM_MAX; i++)
	{
		if (g_taskComps[i].run)                  // 判断时间片标志
		{
			g_taskComps[i].run = 0;              // 标志清零
			g_taskComps[i].pTaskFuncCb();        // 执行调度业务功能模块
		}
	}
}

/**
***********************************************************
* @brief 在定时器中断服务函数中被间接调用，设置时间片标记，
         需要定时器1ms产生1次中断
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
// *@brief 快速排序算法（升序）
// *@param _a,目标元素1
// *@param _b,目标元素2
// *@return val,比较值
// ***************************************
// */
// int32_t CmpCb(const void *_a, const void *_b)	//升序
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
// *@brief 快速排序算法（降序）
// *@param _a,目标元素1
// *@param _b,目标元素2
// *@return val,比较值
// ***************************************
// */
// int32_t StuCmpCb(const void *_a, const void *_b)	//降序
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
*@brief 二分查找法（升序）
*@param	arr，升序数组 
*@param size,数组的元素个数
*@param key,目标值
*@return mid,目标值 / -1，未查询到
***************************************
*/
int32_t AsecBinarySearch(int32_t *arr, int32_t size, int32_t key)   //对应升序数组
{
    int32_t left = 0;   //设置左下标
    int32_t right = size - 1;   //设置右下标
    int32_t mid;       
    while(left <= right)    //循环终止条件： left == right + 1,搜索区间为空，终止循环
    {
        mid = left + (right - left) / 2;    //设置中间下标，防止（left + right）溢出
        if(key < arr[mid])  //如果指定元素比中间下表的值小
            right = mid - 1;    //则把中间下标-1给右下标
        else if(key > arr[mid])     //如果指定元素比中间下标的值大
            left = mid + 1;
        else 
            return mid;
    }
    return -1;      //返回-1则表示该数组总未找到该指定元素
}

/**
***************************************
*@brief 二分查找法（降序）
*@param	arr，升序数组 
*@param size,数组的元素个数
*@param key,目标值
*@return mid,目标值 / -1，未查询到
***************************************
*/
int32_t DsecBinarySearch(int32_t *arr, int32_t size, int32_t key)   //对应降序数组
{
    int32_t left = 0;   //设置左下标
    int32_t right = size - 1;   //设置右下标
    int32_t mid;       
    while(left <= right)    //循环终止条件： left == right + 1,搜索区间为空，终止循环
    {
        mid = left + (right - left) / 2;    //设置中间下标，防止（left + right）溢出
        if(key > arr[mid])  //如果指定元素比中间下表的值小
            right = mid - 1;    //则把中间下标-1给右下标
        else if(key < arr[mid])     //如果指定元素比中间下标的值大
            left = mid + 1;
        else 
            return mid;
    }
    return -1;      //返回-1则表示该数组总未找到该指定元素
}

/**
***************************************
*@brief 二分查找法（升序）,查找接近值
*@param	arr，升序数组 
*@param size,数组的元素个数
*@param key,目标值
*@return index,接近值
***************************************
*/
int32_t AsecNear(int32_t *arr, int32_t size, int32_t key)	//对应升序数组
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
*@brief 二分查找法（降序）,查找接近值
*@param	arr，升序数组 
*@param size,数组的元素个数
*@param key,目标值
*@return index,接近值
***************************************
*/
int32_t DsecNear(int32_t *arr, int32_t size, int32_t key)	 //对应降序数组
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

