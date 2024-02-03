#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "norflash_drv.h"
#include "mb.h"

typedef struct {
    uint16_t  magicCode;
	
	/* 添加配置参数开始 */
    uint8_t modbusAddr;
	
	/* 添加配置参数结束 */
	
	uint8_t crcVal;
} SysParam_t;


#define MAGIC_CODE     0x5A5A
static const SysParam_t g_sysParamDefault =
{
    .magicCode = MAGIC_CODE,
	.modbusAddr = 1
};

static SysParam_t g_sysParamCurrent;

#define SYSPARAM_MAX_SIZE              4096
#define SYSPARAM_START_ADDR            0
#define BACKUP_START_ADDR              4096

static uint8_t CalcCrc8(uint8_t *buf, uint32_t len)
{
    uint8_t crc = 0xFF;

    for (uint8_t byte = 0; byte < len; byte++)
    {
        crc ^= (buf[byte]);
        for (uint8_t i = 8; i > 0; --i)
        {
			if (crc & 0x80)
			{
				crc = (crc << 1) ^ 0x31;
			}
			else 
			{	
				crc = (crc<<1);
			}
		}
    }
    return crc;
}

static bool ReadDataWithCheck(uint32_t readAddr, uint8_t *pBuffer, uint16_t numToRead)
{
	ReadNorflashData(readAddr, pBuffer, numToRead);

	uint8_t crcVal = CalcCrc8(pBuffer, numToRead - 1);
	if (crcVal != pBuffer[numToRead - 1])
	{
		return false;
	}
	return true;
}

static bool ReadSysParam(SysParam_t *sysParam)
{
	uint16_t sysParamLen = sizeof(SysParam_t);
	
	if (ReadDataWithCheck(SYSPARAM_START_ADDR, (uint8_t *)sysParam, sysParamLen))
	{
		return true;
	}
	if (ReadDataWithCheck(BACKUP_START_ADDR, (uint8_t *)sysParam, sysParamLen))
	{
		return true;
	}
	return false;
}

static bool WriteDataWithCheck(uint32_t writeAddr, uint8_t *pBuffer, uint16_t numToWrite)
{
	pBuffer[numToWrite - 1] = CalcCrc8(pBuffer, numToWrite - 1);
	
	EraseNorflashForWrite(writeAddr, numToWrite);
	WriteNorflashData(writeAddr, pBuffer, numToWrite);

	return true;
}
	
static bool WriteSysParam(SysParam_t *sysParam)
{
	uint16_t sysParamLen = sizeof(SysParam_t);
	if (sysParamLen > SYSPARAM_MAX_SIZE)
	{
		return false;
	}
	if (!WriteDataWithCheck(SYSPARAM_START_ADDR, (uint8_t *)sysParam, sysParamLen))
	{
		return false;
	}
	
	WriteDataWithCheck(BACKUP_START_ADDR, (uint8_t *)sysParam, sysParamLen);
	
	return true;
}

void InitSysParam(void)
{
	SysParam_t sysParam;
	
	if (ReadSysParam(&sysParam) && sysParam.magicCode == MAGIC_CODE)
	{	
		g_sysParamCurrent = sysParam;

		eMBSetSlaveAddr(g_sysParamCurrent.modbusAddr);
		
		return;
	}
	g_sysParamCurrent = g_sysParamDefault;
}

bool SetModbusParam(uint8_t addr)
{
	if (addr == g_sysParamCurrent.modbusAddr)
	{
		return true;
	}
	
	SysParam_t sysParam = g_sysParamCurrent;
	sysParam.modbusAddr = addr;
	
	if (eMBSetSlaveAddr(addr) != MB_ENOERR)
	{
		return false;
	}

	if (!WriteSysParam(&sysParam))
	{
		eMBSetSlaveAddr(g_sysParamCurrent.modbusAddr);
		return false;
	}

	g_sysParamCurrent = sysParam;
	return true;
}
