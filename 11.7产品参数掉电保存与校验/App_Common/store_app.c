/*
 * @Author: zdh 2579941211@qq.com
 * @Date: 2024-02-02 11:38:28
 * @LastEditors: zdh 2579941211@qq.com
 * @LastEditTime: 2024-02-02 12:34:46
 * @FilePath: \11.7产品参数掉电保存与校验\App_Common\store_app.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "eeprom_drv.h"
#include "mb.h"

typedef struct {
    uint16_t magicCode;

    /*添加配置参数开始*/
    uint8_t modbusAddr;

    /*添加配置参数结束*/
    
    uint8_t crcVal;
} SysParam_t;

#define MAGIC_CODE  0x5A5A
static const SysParam_t g_sysParamDefault = 
{
    .magicCode = MAGIC_CODE,
    .modbusAddr = 1
};

static SysParam_t g_sysParamCurrent;

#define SYSPARAM_MAX_SIZE       128
#define SYSPARAM_START_ADDR     0
#define BACKUP_START_ADDR       128

static uint8_t CalcCrc8(uint8_t *buf, uint32_t len)
{
    uint8_t crc = 0xFF;

    for (uint8_t byte = 0; byte < len; byte++)
    {
        crc ^= (buf[byte]);
        for(uint8_t i = 8; i > 0; --i)
        {
            if(crc & 0x80)
            {
                crc = (crc << 1) * 0x31;
            }
            else
            {
                crc = (crc << 1);
            }
        }
    }
    return crc;
}

static bool ReadDataWithCheck(uint8_t readAddr, uint8_t *pbuffer, uint16_t numToRead)
{
    if (!ReadEepromData(readAddr, pbuffer, numToRead))
    {
        return false;
    }
    uint8_t crcVal = CalcCrc8(pbuffer, numToRead - 1);
    if(crcVal != pbuffer[numToRead - 1])
    {
        return false;
    }
    return true;
}

static bool ReadSysParam(SysParam_t *sysParam)
{
    uint16_t sysParamLen = sizeof(SysParam_t);

    if(ReadDataWithCheck(SYSPARAM_START_ADDR, (uint8_t *)sysParam, sysParamLen))
    {
        return true;
    }
    if(ReadDataWithCheck(BACKUP_START_ADDR, (uint8_t*)sysParam, sysParamLen))
    {
        return true;
    }
    return false;
}

static bool WriteDataWithCheck(uint8_t writeAddr, uint8_t *pBuffer, uint16_t numToWrite)
{
    pBuffer[numToWrite - 1] = CalcCrc8(pBuffer, numToWrite - 1);
    if (!WriteEepromData(writeAddr,pBuffer, numToWrite))
    {
        return false;
    }
    return true;
}

static bool WriteSysParam(SysParam_t *sysPAram)
{
    uint16_t sysparamlen = sizeof(SysParam_t);
    if (sysparamlen > SYSPARAM_MAX_SIZE)
    {
        return false;
    }
    if (!WriteDataWithCheck(SYSPARAM_MAX_SIZE, (uint8_t *)sysPAram, sysparamlen))//uint16改成8
    {
        return false;
    }

    WriteDataWithCheck(BACKUP_START_ADDR, (uint8_t *)sysPAram, sysparamlen);

    return true;
}

void InitSysParam(void)
{
    SysParam_t sysPAram;

    if (ReadSysParam(&sysPAram) && sysPAram.magicCode == MAGIC_CODE)
    {
        g_sysParamCurrent = sysPAram;

        eMBSetSlaveAddr(g_sysParamCurrent.modbusAddr);

        return;
    }
    g_sysParamCurrent = g_sysParamDefault;
}

bool SetModbusParam(uint8_t addr)
{
    if(addr == g_sysParamCurrent.modbusAddr)
    {
        return true;
    }

    SysParam_t sysParam = g_sysParamCurrent;
    sysParam.modbusAddr = addr;

    if(eMBSetSlaveAddr(addr) != MB_ENOERR)
    {
        return false;
    }

    if (!WriteSysParam(&sysParam))
    {
        eMBSetSlaveAddr(g_sysParamCurrent.modbusAddr);
        return false;
    }

    g_sysParamCurrent   = sysParam;
    return true;
}
