#ifndef _MODBUS_SLAVE_H_
#define _MODBUS_SLAVE_H_

#include <stdint.h>
#include "mb.h"

typedef struct {
    eMBErrorCode (*ReadRegs)(uint8_t startAddr, uint8_t regNum, uint8_t *buf);
    eMBErrorCode (*WriteRegs)(uint8_t startAddr, uint8_t regNum, uint8_t *buf);
} ModbusFuncCb_t;

typedef struct {
    uint8_t slaveAddr;      //从机地址
    uint32_t baudRate;      //波特率
    ModbusFuncCb_t cb;      //回调函数
} ModbusSlaveInstance_t;

void ModbusSlaveInit(ModbusSlaveInstance_t *mbInstance);

#endif
