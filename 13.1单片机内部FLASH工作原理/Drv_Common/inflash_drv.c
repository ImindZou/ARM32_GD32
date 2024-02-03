#include "gd32f30x.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define FLASH_PAGE_SIZE					0x800		//	2K
#define FLASH_END_ADDRESS				0x0807FFFF	//	512K

/**
***********************************************************
* @brief 指定地址开始读出指定个数的数据
* @param readAddr，读取地址
* @param pBuffer，数组首地址
* @param numToRead，要读出的数据个数
* @return 
***********************************************************
*/
bool FlashRead(uint32_t readAddr, uint8_t *pBuffer, uint32_t numToRead)
{
	if((readAddr + numToRead) > FLASH_END_ADDRESS)
	{
		return false;
	}
	
	uint32_t addr = readAddr;
	for (uint32_t i = 0; i < numToRead; i++)
	{
		*pBuffer = *(uint8_t *)addr;
		addr = addr + 1;
		pBuffer++;
	}
	return true;
}

/**
***********************************************************
* @brief 指定地址开始写入指定个数的数据
* @param writeAddr，读写入址
* @param pBuffer，数组首地址
* @param numToRead，要写入的数据个数
* @return 
***********************************************************
*/
bool FlashWrite(uint32_t writeAddr, uint8_t *pBuffer, uint32_t numToWrite)
{
	if ((writeAddr + numToWrite) > FLASH_END_ADDRESS)
	{
		return false;
	}
	if (writeAddr % 2 == 1)		//半字(2字节)写入，地址要对齐
	{
		return false;
	}
	uint16_t temp;
	
	fmc_state_enum fmcState = FMC_READY;
	
	fmc_unlock();
	
	for (uint32_t i = 0; i < numToWrite / 2; i++)
	{
		fmc_flag_clear(FMC_FLAG_BANK0_END | FMC_FLAG_BANK0_WPERR | FMC_FLAG_BANK0_PGERR);
		fmcState = fmc_halfword_program(writeAddr, *(uint16_t *)pBuffer);
		if (fmcState != FMC_READY)
		{
			fmc_lock();
			return false;
		}
		
		pBuffer += 2;
		writeAddr += 2;
	}
	if (numToWrite % 2)
	{
		fmc_flag_clear(FMC_FLAG_BANK0_END | FMC_FLAG_BANK0_WPERR | FMC_FLAG_BANK0_PGERR);
		temp = *pBuffer | 0xff00;
		fmcState = fmc_halfword_program(writeAddr,temp);
	}
	
	fmc_lock();
	return true;
}

/**
***********************************************************
* @brief 擦除从eraseAddr开始到eraseAddr + numToErase的页
* @param eraseAddr，地址
* @param numToErase，对应写入数据时的个数
* @return 
***********************************************************
*/
bool FlashErase(uint32_t eraseAddr, uint32_t numToErase)
{
	if (numToErase == 0 || (eraseAddr + numToErase) > FLASH_END_ADDRESS)
	{
		return false;
	}
	
	uint8_t pageNum;
	uint8_t addrOffset = eraseAddr % FLASH_PAGE_SIZE;	//mod运算#求余在一页内的偏移，若eraseAddr是FLASH——PAGE——SIZE整数倍，运算结果为0
	
	fmc_state_enum fmcState = FMC_READY;
	fmc_unlock();
	
	if (numToErase > (FLASH_PAGE_SIZE - addrOffset))	//跨页
	{
		fmc_flag_clear(FMC_FLAG_BANK0_END | FMC_FLAG_BANK0_WPERR | FMC_FLAG_BANK0_PGERR);
		fmcState = fmc_page_erase(eraseAddr);	//擦除本页
		if (fmcState != FMC_READY)
		{
			goto erase_err;
		}
		
		eraseAddr += FLASH_PAGE_SIZE - addrOffset;	//对齐到页地址
		numToErase -= FLASH_PAGE_SIZE - addrOffset;
		pageNum = numToErase / FLASH_PAGE_SIZE;
		
		while (pageNum--)
		{
			fmc_flag_clear(FMC_FLAG_BANK0_END | FMC_FLAG_BANK0_WPERR | FMC_FLAG_BANK0_PGERR);
			fmcState = fmc_page_erase(eraseAddr);
			if (fmcState != FMC_READY)
			{
				goto erase_err;
			}
			eraseAddr += FLASH_PAGE_SIZE;
		}
		if (numToErase % FLASH_PAGE_SIZE != 0)
		{
			fmc_flag_clear(FMC_FLAG_BANK0_END | FMC_FLAG_BANK0_WPERR | FMC_FLAG_BANK0_PGERR);
			fmcState = fmc_page_erase(eraseAddr);
			if(fmcState != FMC_READY)
			{
				goto erase_err;
			}
		}
		
	}
	else	//没有跨页
	{
		fmc_flag_clear(FMC_FLAG_BANK0_END | FMC_FLAG_BANK0_WPERR | FMC_FLAG_BANK0_PGERR);
		fmcState = fmc_page_erase(eraseAddr);
		if(fmcState != FMC_READY)
		{
			goto erase_err;
		}
	}
	/* lock the main FMC after the erase operation */
	fmc_lock();
	return true;
	
erase_err:
	/* lock the main FMC after the erase operation */
	fmc_lock();
	return false;
}

#define BUFFER_SIZE						3000
#define FLASH_TEST_ADDRESS				0x0807F000

void FlashDrvTest(void)
{
	uint8_t bufferWrite[BUFFER_SIZE];
	uint8_t bufferRead[BUFFER_SIZE];
	
	printf("flash writing ata: \n");
	for (uint16_t i = 0; i < BUFFER_SIZE; i++)
	{
		bufferWrite[i] = i + 1;
		printf("0x%02X ", bufferWrite[i]);
	}
	printf("\nStart writing...\n");
	
	if (!FlashErase(FLASH_TEST_ADDRESS,BUFFER_SIZE))
	{
		printf("Flash write data failed, please checkout!!!\n");
		return;
	}
	
	if (!FlashWrite(FLASH_TEST_ADDRESS, bufferWrite, BUFFER_SIZE))
	{
		printf("Flash writing failed , Please checkout!!!\n");
	}
	
	printf("start reading...\n");
	if (!FlashRead(FLASH_TEST_ADDRESS,bufferRead,BUFFER_SIZE))
	{
		printf("Flash read error,please checkout!!!\n");
		return;
	}
	for (uint16_t i = 0; i < BUFFER_SIZE; i++)
	{
		if(bufferRead[i] != bufferWrite[i]){
			printf("0x%02X ",bufferRead[i]);
			printf("Flash test error,please checkout!!!\n");
			return;
		}
		printf("0x%02X ",bufferRead[i]);
	}
	printf("\nFlash pass test\n");
}




