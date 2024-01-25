#include <stdint.h>
#include "queue.h"

/**
****************************************************************
* @brief   初始化（创建）队列，每个队列须先执行该函数才能使用
* @param   queue, 队列变量指针
* @param   buffer, 队列缓存区地址
* @param   size, 队列缓存区长度
* @return 
****************************************************************
*/
void QueueInit(QueueType_t *queue, uint8_t *buffer, uint32_t size)
{
    queue->buffer = buffer;
    queue->size = size;
    queue->head = 0;
    queue->tail = 0;
}

/**
****************************************************************
* @brief   压入数据到队列中
* @param   queue, 队列变量指针
* @param   data, 待压入队列的数据
* @return  压入队列是否成功
****************************************************************
*/
QueueStatus_t QueuePush(QueueType_t *queue, uint8_t data)
{
    uint32_t index = (queue->tail + 1) % queue->size;

    if (index == queue->head)
    {
        return QUEUE_OVERLOAD;
    }
    queue->buffer[queue->tail] = data;
    queue->tail = index;
    return QUEUE_OK;
}

/**
****************************************************************
* @brief   从队列中弹出数据
* @param   queue, 队列变量指针
* @param   pdata, 待弹出队列的数据缓存地址
* @return  弹出队列是否成功
****************************************************************
*/
QueueStatus_t QueuePop(QueueType_t *queue, uint8_t *pdata)
{
    if(queue->head == queue->tail)
    {
        return QUEUE_EMPTY;
    }

    *pdata = queue->buffer[queue->head];
    queue->head = (queue->head + 1) % queue->size;
    return QUEUE_OK;
}

/**
****************************************************************
* @brief   压入一组数据到队列中
* @param   queue, 队列变量指针
* @param   pArray, 待压入队列的数组地址
* @param   len, 待压入队列的元素个数
* @return  实际压入到队列的元素个数
****************************************************************
*/
uint32_t QueuePushArray(QueueType_t *queue, uint8_t *pArray, uint32_t len)
{
    uint32_t i;
    for (i = 0; i < len; i++)
    {
        if(QueuePush(queue, pArray[i]) == QUEUE_OVERLOAD)
        {
            break;
        }
    }
    return i;
}

/**
****************************************************************
* @brief   从队列中弹出一组数据
* @param   queue, 队列变量指针
* @param   pArray, 待弹出队列的数据缓存地址
* @param   len, 待弹出队列的数据的最大长度
* @return  实际弹出数据的数量
****************************************************************
*/
uint32_t QueuePopArray(QueueType_t *queue, uint8_t *pArray, uint32_t len)
{
    uint32_t i;
    for(i = 0; i < len; i++)
    {
        if (QueuePop(queue, &pArray[i]) == QUEUE_EMPTY)
        {
            break;
        }
    }
    return i;
}

/**
****************************************************************
* @brief   获取队列中数据的个数
* @param   queue, 队列变量指针
* @return  队列中数据的个数
****************************************************************
*/
uint32_t QueueCount(QueueType_t *queue)
{
	if (queue->head <= queue->tail)
	{
		return queue->tail - queue->head;
	}
	
	return queue->size + queue->tail - queue->head;
}
