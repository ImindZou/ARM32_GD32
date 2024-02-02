/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-02-02 11:39:23
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-02-02 12:40:20
 * @FilePath: \11.7产品参数掉电保存与校验\App_Common\store_app.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AEn
 */
#ifndef __STORE_APP_H_
#define __STORE_APP_H_

#include <stdint.h>
#include <stdbool.h>

bool SetModbusParam(uint8_t addr);
void InitSysParam(void);


#endif
