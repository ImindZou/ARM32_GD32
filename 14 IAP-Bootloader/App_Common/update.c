#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rs485_drv.h"
#include "delay.h"
#include "flash_drv.h"
#include "update.h"

#define IS_AF(c)  					((c >= 'A') && (c <= 'F'))
#define IS_af(c)  					((c >= 'a') && (c <= 'f'))
#define IS_09(c)  					((c >= '0') && (c <= '9'))
#define ISVALIDHEX(c)  				(IS_AF(c) || IS_af(c) || IS_09(c))
#define ISVALIDDEC(c)  				IS_09(c)
#define CONVERTDEC(c)  				(c - '0')

#define CONVERTHEX_alpha(c)  (IS_AF(c) ? (c - 'A'+10) : (c - 'a'+10))
#define CONVERTHEX(c)   (IS_09(c) ? (c - '0') : CONVERTHEX_alpha(c))

/**
  * @brief  Convert an Integer to a string
  * @param  str: The string
  * @param  intnum: The intger to be converted
  * @retval None
  */
void Int2Str(uint8_t *str, int32_t intnum)
{
	uint32_t i, div = 1000000000, j = 0, Status = 0;

	for (i = 0; i < 10; i++)
	{
		str[j++] = (intnum / div) + 0x30;
		intnum = intnum % div;
		div /= 10;
		if ((str[j-1] == '0') & (Status == 0))
		{
			j = 0;
		}
		else
		{
			Status++;
		}
	}
}

/**
  * @brief  Convert a string to an integer
  * @param  inputstr: The string to be converted
  * @param  intnum: The intger value
  * @retval 1: Correct
  *         0: Error
  */
uint32_t Str2Int(uint8_t *inputstr, int32_t *intnum)
{
	uint32_t i = 0, res = 0;
	uint32_t val = 0;

	if (inputstr[0] == '0' && (inputstr[1] == 'x' || inputstr[1] == 'X'))
	{
		if (inputstr[2] == '\0')
		{
			return 0;
		}
		for (i = 2; i < 11; i++)
		{
			if (inputstr[i] == '\0')
			{
				*intnum = val;
				/* return 1; */
				res = 1;
				break;
			}
			if (ISVALIDHEX(inputstr[i]))
			{
				val = (val << 4) + CONVERTHEX(inputstr[i]);
			}
			else
			{
				/* return 0, Invalid input */
				res = 0;
				break;
			}
		}
		/* over 8 digit hex --invalid */
		if (i >= 11)
		{
			res = 0;
		}
	}
	else /* max 10-digit decimal input */
	{
		for (i = 0;i < 11;i++)
		{
			if (inputstr[i] == '\0')
			{
				*intnum = val;
				/* return 1 */
				res = 1;
				break;
			}
			else if ((inputstr[i] == 'k' || inputstr[i] == 'K') && (i > 0))
			{
				val = val << 10;
				*intnum = val;
				res = 1;
				break;
			}
			else if ((inputstr[i] == 'm' || inputstr[i] == 'M') && (i > 0))
			{
				val = val << 20;
				*intnum = val;
				res = 1;
				break;
			}
			else if (ISVALIDDEC(inputstr[i]))
			{
				val = val * 10 + CONVERTDEC(inputstr[i]);
			}
			else
			{
				/* return 0, Invalid input */
				res = 0;
				break;
			}
		}
		/* Over 10 digit decimal --invalid */
		if (i >= 11)
		{
			res = 0;
		}
	}

	return res;
}

/******************************************************************************
 * Name:    CRC-16/XMODEM       x16+x12+x5+1
 * Poly:    0x1021
 * Init:    0x0000
 * Refin:   False
 * Refout:  False
 * Xorout:  0x0000
 * Alias:   CRC-16/ZMODEM,CRC-16/ACORN
 *****************************************************************************/
uint16_t Crc16Ymodem(uint8_t *data, uint16_t length)
{
    uint8_t i;
    uint16_t crc = 0;            // Initial value
    while (length--)
    {
        crc ^= (uint16_t)(*data++) << 8;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 0x8000)
                crc = (crc << 1) ^ 0x1021;
            else
                crc <<= 1;
        }
    }
    return crc;
}

#define PACKET_SEQNO_INDEX      (1)
#define PACKET_SEQNO_COMP_INDEX (2)

#define PACKET_HEADER           (3)
#define PACKET_TRAILER          (2)
#define PACKET_OVERHEAD         (PACKET_HEADER + PACKET_TRAILER)
#define PACKET_SIZE             (128)
#define PACKET_1K_SIZE          (1024)

#define SOH                     (0x01)  //128字节数据包开始
#define STX                     (0x02)  //1024字节的数据包开始
#define EOT                     (0x04)  //结束传输
#define ACK                     (0x06)  //回应
#define NAK                     (0x15)  //没回应
#define CA                      (0x18)  //这两个相继中止转移
#define CREQ                    (0x43)  //'C' == 0x43, 请求数据

#define ABORT1                  (0x41)  //'A' == 0x41, 用户终止 
#define ABORT2                  (0x61)  //'a' == 0x61, 用户终止

#define NAK_TIMEOUT             (0x100000)
#define MAX_ERRORS              (5)


#define YMODEM_PACKET_LENGTH        1024
static uint8_t g_packetBuffer[YMODEM_PACKET_LENGTH];

#define FILE_NAME_LENGTH           	256
#define FILE_SIZE_LENGTH           	16

static char g_imageName[FILE_NAME_LENGTH];

static int32_t ReceivePacket(uint8_t *data, int32_t *length, uint32_t timeout)
{
	uint16_t i, packetSize;
	uint8_t c;
	*length = 0;
	if (ReceiveByteTimeout(&c, timeout) != 0)
	{
		return -1;
	}
	switch (c)
	{
		case SOH:
			packetSize = PACKET_SIZE;
			break;
		case STX:
			packetSize = PACKET_1K_SIZE;
			break;
		case EOT:
			return 0;
		case CA:
			if ((ReceiveByteTimeout(&c, timeout) == 0) && (c == CA))
			{
				*length = -1;
				return 0;
			}
			else
			{
				return -1;
			}
		case ABORT1:
		case ABORT2:
			return 1;
		default:
			return -1;
	}
	*data = c;
	for (i = 1; i < (packetSize + PACKET_OVERHEAD); i++)
	{
		if (ReceiveByteTimeout(data + i, timeout) != 0)
		{
		  return -1;
		}
	}
	if ((data[PACKET_SEQNO_INDEX] | data[PACKET_SEQNO_COMP_INDEX]) != 0xff)
	{
		return -1;
	}

	uint16_t crc16 = Crc16Ymodem(&data[3], packetSize);
	uint16_t raw_crc16 = (uint16_t)(data[packetSize + PACKET_OVERHEAD - 2] << 8) | data[packetSize + PACKET_OVERHEAD - 1];
	if (crc16 != raw_crc16)
	{
		return -1;
	}

	*length = packetSize;
	return 0;
}


/**
  * @brief  Receive a file using the ymodem protocol
  * @param  buf: Address of the first byte
  * @retval The size of the file
  */
int32_t YmodemReceive (uint8_t *buf)
{
	uint8_t packetData[PACKET_1K_SIZE + PACKET_OVERHEAD], fileSize[FILE_SIZE_LENGTH], *filePtr, *bufPtr;
	int32_t i, packetLength, sessionDone, fileDone, packetsReceived, errors, sessionBegin, size = 0;

	/* Initialize FlashDestination variable */
	uint32_t flashDestination = APP_ADDR_IN_FLASH;

	for (sessionDone = 0, errors = 0, sessionBegin = 0; ; )  // 这一次任务，可能会接收到多个文件
	{
		for (packetsReceived = 0, fileDone = 0, bufPtr = buf; ;) // 这一个文件
		{
			switch (ReceivePacket(packetData, &packetLength, NAK_TIMEOUT))
			{
				case 0:
					errors = 0;
					switch (packetLength)
					{
						/* Abort by sender */
						case -1:
							SendByte(ACK);
							return 0;
							/* End of transmission */
						case 0:
							SendByte(ACK);
							fileDone = 1;
							break;
						/* Normal packet */
						default:
							if (packetsReceived == 0) //第一个包
							{
								/* Filename packet */
								if (packetData[PACKET_HEADER] != 0)
								{
									/* Filename packet has valid data */
									for (i = 0, filePtr = packetData + PACKET_HEADER; (*filePtr != 0) && (i < FILE_NAME_LENGTH);)
									{
										g_imageName[i++] = *filePtr++;
									}
									g_imageName[i++] = '\0';
									for (i = 0, filePtr ++; (*filePtr != ' ') && (i < FILE_SIZE_LENGTH);)
									{
										fileSize[i++] = *filePtr++;
									}
									fileSize[i++] = '\0';
									Str2Int(fileSize, &size);

									/* Test the size of the image to be sent */
									/* Image size is greater than Flash size */
									if (size > FLASH_APP_SIZE)  //文件大小，大于flash APP的容量
									{
										/* End session */
										SendByte(CA);
										SendByte(CA);
										return -1;
									}
									FlashErase(flashDestination, size); 
							
									SendByte(ACK);
									SendByte(CREQ);
								}
								/* Filename packet is empty, end session */
								else
								{
									SendByte(ACK);
									fileDone = 1;
									sessionDone = 1;
									break;
								}
							}
							/* Data packet */
							else
							{
								memcpy(bufPtr, packetData + PACKET_HEADER, packetLength);
								FlashWrite(flashDestination, bufPtr, packetLength);
								flashDestination += packetLength;
								SendByte(ACK);
							}
							packetsReceived++;
							sessionBegin = 1;
							break;
					}
					break;
				case 1:
					SendByte(CA);
					SendByte(CA);
					return -3;
				default:
					if (sessionBegin > 0)
					{
						errors++;
					}
					if (errors > MAX_ERRORS)
					{
						SendByte(CA);
						SendByte(CA);
						return 0;
					}
					SendByte(CREQ);
					break;
			}
			if (fileDone != 0)
			{
				break;
			}
		}
		if (sessionDone != 0)
		{
			break;
		}
	}
	return (int32_t)size;
}

void UpdateApp(void)
{
	uint8_t strBuffer[10] = "";
	int32_t imageSize = 0;

	printf("Waiting for the file to be sent ... (press 'a' to abort)\n\r");
	imageSize = YmodemReceive(g_packetBuffer);
	
	DelayNms(50);	// 需要等待一会，否则securecrt不能显示下面的打印信息

	if (imageSize > 0)
	{
		printf("\n\n\r Programming Completed Successfully!\n\r\r\n");
		printf("[ Name: %s", g_imageName);
		Int2Str(strBuffer, imageSize);
		printf(",imageSize: ");
		printf("%s", strBuffer);
		printf(" Bytes]\r\n");
	}
	else if (imageSize == -1)
	{
		printf("\n\n\rThe image size is higher than the allowed space memory!\n\r");
	}
	else if (imageSize == -2)
	{
		printf("\n\n\rVerification failed!\n\r");
	}
	else if (imageSize == -3)
	{
		printf("\r\n\nAborted by user.\n\r");
	}
	else
	{
		printf("\n\rFailed to receive the file!\n\r");
	}
}
