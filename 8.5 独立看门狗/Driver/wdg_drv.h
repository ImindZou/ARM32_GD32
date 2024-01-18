/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-18 14:49:12
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-18 14:58:43
 * @FilePath: \8.4 RTC驱动层代码实现d:\Study\ARM32单片机实战课程\06自己动手\8.5 独立看门狗\Driver\wdg_drv.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __WDG_DRV_H_
#define __WDG_DRV_H_

/**
***********************************************************
* @brief 独立看门狗初始化配置
* @param
* @return 
***********************************************************
*/
void WdgDrvInit(void); 

/**
***********************************************************
* @brief 喂狗 
* @param
* @return 
***********************************************************
*/
void FeedDog(void);

#endif

