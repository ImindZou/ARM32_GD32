#ifndef __MODBUS_SLAVE_H_
#define __MODBUS_SLAVE_H_



#include <stdint.h>
#include "mb.h"

typedef struct{
	eMBErrorCode (*ReadRegs)(uint8_t starAddr, uint8_t regNum, uint8_t *buf);
	eMBErrorCode (*WriteRegs)(uint8_t starAddr, uint8_t regNum, uint8_t *buf);
} ModbusFuncCb_t;

typedef struct{
	uint8_t slaveAddr;	//�ӻ���ַ
	uint32_t baudRate;	//������
	ModbusFuncCb_t cb;	//�ص�����
} ModbusSlaveInstance_t;


void ModbusSlaveInit(ModbusSlaveInstance_t *mbInstance);

#endif
