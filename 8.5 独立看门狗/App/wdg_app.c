/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-18 14:59:45
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-18 15:20:25
 * @FilePath: \8.4 RTC驱动层代码实现d:\Study\ARM32单片机实战课程\06自己动手\8.5 独立看门狗\App\wdg_app.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "wdg_drv.h"
#include <stdio.h>

/**
***********************************************************
* @brief 看门狗任务处理函数 
* @param
* @return 
***********************************************************
*/
void WdgTask(void)
{
    printf("************FeedDog*************\n");
    FeedDog();
}
