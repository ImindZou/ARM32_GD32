#include <stdint.h>
#include "mb.h"
#include "mbutils.h"
#include "modbus_slave.h"

static ModbusFuncCb_t g_modbusFuncCb;

eMBErrorCode eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
	eMBErrorCode state;

    if (eMode == MB_REG_READ){
        state = g_modbusFuncCb.ReadRegs(usAddress, usNRegs, pucRegBuffer);
    } 
	else
	{
        state = g_modbusFuncCb.WriteRegs(usAddress, usNRegs, pucRegBuffer);
    }

    return state;
}

void ModbusSlaveInit(ModbusSlaveInstance_t *mbInstance)
{
	eMBInit(MB_RTU, mbInstance->slaveAddr, 0, mbInstance->baudRate, MB_PAR_NONE);
	eMBEnable();
	//g_modbusFuncCb.ReadRegs = mbInstance->cb.ReadRegs;
	//g_modbusFuncCb.WriteRegs = mbInstance->cb.WriteRegs;
	g_modbusFuncCb = mbInstance->cb;
}
