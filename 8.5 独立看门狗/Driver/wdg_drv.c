/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-01-18 14:48:27
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-01-18 14:58:02
 * @FilePath: \8.4 RTC驱动层代码实现d:\Study\ARM32单片机实战课程\06自己动手\8.5 独立看门狗\Driver\wdg_drv.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdint.h>
#include "gd32f30x.h"

/**
***********************************************************
* @brief 独立看门狗初始化配置
* @param
* @return 
***********************************************************
*/
void WdgDrvInit(void)
{
	/*关闭FWDGT_PSC和FWDGT_RLD的写保护*/
	/*配置预分频器和重装载寄存器*/
    fwdgt_config(2500,FWDGT_PSC_DIV32);     //分频后40KHz / 32 = 1.25KHz，周期0.8ms，2500 * 0.8 = 2000ms
	/*开启看门狗*/
    fwdgt_enable();


}

/**
***********************************************************
* @brief 喂狗 
* @param
* @return 
***********************************************************
*/
void FeedDog(void)
{
    fwdgt_counter_reload();
}















