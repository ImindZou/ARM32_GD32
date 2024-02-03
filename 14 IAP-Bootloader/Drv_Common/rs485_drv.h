#ifndef _RS485_DRV_H_
#define _RS485_DRV_H_

#include <stdint.h>
#include <stdbool.h>

void RS485DrvInit(void);
bool GetKeyPressed(uint8_t *key);
int32_t ReceiveByteTimeout(uint8_t *c, uint32_t timeout);
void SendByte(uint8_t c);
#endif
