#include <stdint.h>
#include <stdlib.h>
#include "modbus_slave.h"
#include "sensor_drv.h"
#include "led_drv.h"

#define R   (1 << 0)
#define W   (1 << 1)

typedef struct {
    uint8_t  property;				// 读写的属性
    const uint16_t address;			// 地址
    uint16_t minValue;				// 最小值
    uint16_t maxValue;				// 最大值
	void (*ReadCb)(uint16_t *value);
	void (*WriteCb)(uint16_t value);
} MbRegisterInstance_t;

static void ModbusGetTemp(uint16_t *value);
static void ModbusGetHumi(uint16_t *value);
static void ModbusSetLed1(uint16_t value);
static void ModbusSetLed2(uint16_t value);

static MbRegisterInstance_t g_regInstanceTab[] = {
    {
        .property = R,
        .address = 0x0000,          // 温度 01 03 00 00 00 02
		.ReadCb = ModbusGetTemp,
    },
    {
        .property = R,
        .address = 0x0001,          // 湿度
		.ReadCb = ModbusGetHumi,
    },
    {
        .property = R | W,
        .address = 0x0002,          // LED1开关 01 06 00 02 00 01  ,LED1 LED2 01 10 00 02 00 02 04 00 01 00 01
        .minValue = 0,
        .maxValue = 1,
		.WriteCb = ModbusSetLed1,
    },
    {
        .property = R | W,
        .address = 0x0003,          // LED2开关 01 06 00 03 00 01
        .minValue = 0,
        .maxValue = 1,
		.WriteCb = ModbusSetLed2,
    },
};
#define REG_TABLE_SIZE         (sizeof(g_regInstanceTab) / sizeof(g_regInstanceTab[0]))


static eMBErrorCode ReadRegsCb(uint8_t startAddr, uint8_t regNum, uint8_t *buf)
{
	if (buf == NULL)
	{
        return MB_EINVAL;
	}
	
    for (uint32_t i = 0; i < regNum; i++)
	{
		MbRegisterInstance_t *instance = NULL;
		for (uint32_t j = 0; j < REG_TABLE_SIZE; j++)
		{
			if (g_regInstanceTab[j].address != startAddr + i)
			{
				continue;
			}
			instance = &g_regInstanceTab[j];
			if((instance->property & R) == 0)  //读写检验
			{
				return MB_EINVAL;
			}

			if (instance->ReadCb != NULL)
			{
				instance->ReadCb((uint16_t *)&buf[2 * i]);
				uint8_t tmp;
				tmp = buf[2 * i];
				buf[2 * i] = buf[2 * i + 1];
				buf[2 * i + 1] = tmp;
			}
		}
		if (instance == NULL)
		{
			return MB_ENOREG;
		}
		
    }

    return MB_ENOERR;
}

static eMBErrorCode WriteRegsCb(uint8_t startAddr, uint8_t regNum, uint8_t *buf)
{
    if (buf == NULL)
	{
        return MB_EINVAL;
	}
	
    for (uint32_t i = 0; i < regNum; i++)
	{
		
		MbRegisterInstance_t *instance = NULL;
		for (uint32_t j = 0; j < REG_TABLE_SIZE; j++)
		{
			if (g_regInstanceTab[j].address != startAddr + i)
			{
				continue;
			}
			instance = &g_regInstanceTab[j];
			
		    if((instance->property & W) == 0)  //读写检验
			{
				return MB_EINVAL;
			}
			
			uint16_t setValue = ((buf[2 * i] << 8) & 0xFF00) | (buf[2 * i + 1] & 0xFF);
			if ((setValue < instance->minValue) || (setValue > instance->maxValue))
			{
				return MB_EINVAL;
			}
			
			if (instance->WriteCb != NULL)
			{
				instance->WriteCb(setValue);
			}
		}
		
		if (instance == NULL)
		{
			return MB_ENOREG;
		}
	}	
	return MB_ENOERR;
}

static void ModbusGetTemp(uint16_t *value)
{
	SensorData_t sensorData;
	GetSensorData(&sensorData);
	*value = (uint16_t)(sensorData.temp * 10);
}

static void ModbusGetHumi(uint16_t *value)
{
	SensorData_t sensorData;
	GetSensorData(&sensorData);
	*value = (uint16_t)(sensorData.humi * 10);
}

static void ModbusSetLed1(uint16_t value)
{
	value == 0 ? TurnOffLed(LED1) : TurnOnLed(LED1);
}

static void ModbusSetLed2(uint16_t value)
{
	value == 0 ? TurnOffLed(LED2) : TurnOnLed(LED2);
}

void ModbusAppInit(void)
{
	ModbusSlaveInstance_t mbInstace = {
        .baudRate = 9600,
        .cb.ReadRegs = ReadRegsCb,
        .cb.WriteRegs = WriteRegsCb,
        .slaveAddr = 0x01,
    };
	ModbusSlaveInit(&mbInstace);
}

void ModbusTask(void)
{
	(void)eMBPoll();
}
