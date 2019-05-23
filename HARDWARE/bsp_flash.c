/*
 * bsp_flash.c
 *
 *  Created on: 2019年5月15日
 *      Author: Administrator
 */

#include "bsp_flash.h"
#include <string.h>

extern SPI_HandleTypeDef hspi1;

#define BIG2LITTLE16(x) do{	x =					\
		x = (( ((uint16_t)(A) & 0xff00) >> 8)	|\
		(( (uint16_t)(A) & 0x00ff) << 8));		\
}while(0)

#define BIG2LITTLE32(x) do{	x =					\
		(( ((uint32_t)(x) & 0xff000000) >> 24)	|\
		(( (uint32_t)(x) & 0x00ff0000) >> 8)	|\
		(( (uint32_t)(x) & 0x0000ff00) << 8)	|\
		(( (uint32_t)(x) & 0x000000ff) << 24));	\
}while(0)

#define PAGEPROGRAMTIME 5
#define SECTORERASETIME 300
#define BLOCKERASETIME (2*1000)
#define CHIPERASETIME (140*1000)
#define WRITESTATUSREGISTERTIME 50

#define SPI1_CS_HIGH  HAL_GPIO_WritePin(SPI1_CS_GPIO_Port,  SPI1_CS_Pin, GPIO_PIN_SET )
#define SPI1_CS_LOW  HAL_GPIO_WritePin(SPI1_CS_GPIO_Port,  SPI1_CS_Pin, GPIO_PIN_RESET )

static uint32_t _flashWaitTime = PAGEPROGRAMTIME;
static uint32_t curSysTick;
static uint8_t eeprom_Cache[0x1000];

void SPI_Transmit(uint8_t* pData, uint16_t Size, uint32_t Timeout)
{
	HAL_SPI_Transmit(&hspi1, pData, Size, Timeout);
}

void SPI_Receive(uint8_t* pData, uint16_t Size, uint32_t Timeout)
{
	HAL_SPI_Receive(&hspi1, pData, Size, Timeout);
}

void SPI_TransmitReceive(uint8_t* pTxData, uint8_t* pRxData, uint16_t Size,
		uint32_t Timeout)
{
	HAL_SPI_TransmitReceive(&hspi1, pTxData, pRxData, Size, Timeout);
}

void write_Enable()
{
	uint8_t data = 0x06;

	SPI1_CS_LOW;
	SPI_Transmit(&data, 1, 0xFF);
	SPI1_CS_HIGH;
}

void write_Disable()
{
	uint8_t data = 0x04;
	SPI1_CS_LOW;
	SPI_Transmit(&data, 1, 0xFF);
	SPI1_CS_HIGH;
}

void RSTEN()
{
	uint8_t data = 0x66;
	SPI1_CS_LOW;
	SPI_Transmit(&data, 1, 0xFF);
	SPI1_CS_HIGH;
}

void read_Status_Register_1(uint8_t *status_Register)
{
	uint8_t commond = 0x05;

	SPI1_CS_LOW;
	SPI_Transmit(&commond, 1, 0xFF);
	SPI_TransmitReceive(status_Register, status_Register, 1, 0xFFFFFF);
	SPI1_CS_HIGH;
}

bool status_Register_WIP_Wait()
{
	uint8_t status_Register;
	curSysTick = HAL_GetTick();
	printf("_flashWaitTime:%d \r\n", _flashWaitTime);
	while (_flashWaitTime)
	{
		read_Status_Register_1(&status_Register);
		if ((status_Register & 0x01) == 0x00)
		{
			return true;
		}

		if (HAL_GetTick() - curSysTick > _flashWaitTime)
		{
			break;
		}

	}
	printf("Warring WIP_Wait TimeOut %u\r\n",
			(uint32_t) (HAL_GetTick() - curSysTick));
	return false;
}

void sector_Erase(uint32_t blk_addr)
{
	uint8_t commond = 0x20;

	status_Register_WIP_Wait();
	write_Enable();
	BIG2LITTLE32(blk_addr);

	SPI1_CS_LOW;
	SPI_Transmit((uint8_t*) &commond, 1, 0xFF);
	SPI_Transmit((uint8_t*) &blk_addr + 1, 3, 0xFF);
	SPI1_CS_HIGH;

	_flashWaitTime = SECTORERASETIME;

}

void block_Erase(uint32_t blk_addr)
{
	uint8_t commond = 0xD8;

	status_Register_WIP_Wait();
	write_Enable();
	BIG2LITTLE32(blk_addr);

	SPI1_CS_LOW;
	SPI_Transmit((uint8_t*) &commond, 1, 0xFF);
	SPI_Transmit((uint8_t*) &blk_addr + 1, 3, 0xFF);
	SPI1_CS_HIGH;

	_flashWaitTime = BLOCKERASETIME;

}

void deep_power_Down()
{
	uint8_t commond = 0xB9;
	SPI1_CS_LOW;
	SPI_Transmit((uint8_t*) &commond, 1, 0xFF);
	SPI1_CS_HIGH;
}

void page_Program(uint8_t *buf, uint32_t blk_addr, uint32_t blk_len)
{
	uint8_t commond = 0x02;

	printf("ppaddr:%x\r\n", blk_addr);
	BIG2LITTLE32(blk_addr);
	status_Register_WIP_Wait();
	write_Enable();
	printf("ppaddr:%x\r\n", blk_addr);

	SPI1_CS_LOW;
	SPI_Transmit(&commond, 1, 0xFF);
	SPI_Transmit((uint8_t*) &blk_addr + 1, 3, 0xFF);
	SPI_Transmit(buf, blk_len, 0xFFFF);
	SPI1_CS_HIGH;

	_flashWaitTime = PAGEPROGRAMTIME;

}

void read_Data(uint8_t *buf, uint32_t blk_addr, uint32_t blk_len)
{
	uint8_t commond = 0x03;

	BIG2LITTLE32(blk_addr);
	status_Register_WIP_Wait();

	SPI1_CS_LOW;
	SPI_Transmit(&commond, 1, 0xFF);
	SPI_Transmit((uint8_t*) &blk_addr + 1, 3, 0xFF);
	SPI_TransmitReceive(buf, buf, blk_len, 0xFF);
	SPI1_CS_HIGH;
}

void read_Manufacturer_Device_ID()
{
	uint8_t pRxTxData[3];
	uint8_t data = 0x90;

	SPI1_CS_LOW;
	SPI_Transmit(&data, 1, 0xFF);
	SPI_Transmit(pRxTxData, 2, 0xFF);
	SPI_TransmitReceive(pRxTxData, pRxTxData, 3, 0xFF);
	SPI1_CS_HIGH;
	printf("flashID:%x %x %x\r\n", pRxTxData[0], pRxTxData[1], pRxTxData[2]);
}

void read_Identification()
{
	uint8_t data = 0x9F;
	uint8_t pRxTxData[3];

	SPI1_CS_LOW;
	SPI_Transmit(&data, 1, 0xFF);
	SPI_TransmitReceive(pRxTxData, pRxTxData, 3, 0xFF);
	SPI1_CS_HIGH;
	printf("flashID:%x %x %x\r\n", pRxTxData[0], pRxTxData[1], pRxTxData[2]);
}

void flash_Write_Block(uint8_t lun, uint8_t *buf, uint32_t blk_addr,
		uint32_t blk_len)
{
	uint16_t remainPage;

	remainPage = FLASH_PAGE_LEN - blk_addr % FLASH_PAGE_LEN;

	if (remainPage > blk_len)
	{
		remainPage = blk_len;
	}

	while (blk_len)
	{
		page_Program(buf, blk_addr, remainPage);

		buf += remainPage;
		blk_addr += remainPage;
		blk_len -= remainPage;

		if (blk_len > FLASH_PAGE_LEN)
		{
			remainPage = FLASH_PAGE_LEN;
		}
		else
		{
			remainPage = blk_len;
		}
	}
}

void flash_Write(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint32_t blk_len)
{
	uint32_t curSector;
	uint32_t remainSector = 0;
	uint32_t tmp;

	tmp = tmp;
	curSector = blk_addr / FLASH_SECORT_LEN;
	remainSector = FLASH_SECORT_LEN - blk_addr % FLASH_SECORT_LEN;

	if (remainSector > blk_len)
	{
		remainSector = blk_len;
	}

	while (blk_len)
	{
		if (remainSector < FLASH_SECORT_LEN)
		{
			read_Data(eeprom_Cache, curSector * FLASH_SECORT_LEN,
			FLASH_SECORT_LEN);

			memcpy((eeprom_Cache + blk_addr % FLASH_SECORT_LEN), buf,
					remainSector);
			sector_Erase(blk_addr);
			printf("0x00\r\n");

		}
		else if (remainSector == FLASH_BLOCK_LEN)
		{
			//TODO: erase block function
			block_Erase(blk_addr);
			printf("0x01\r\n");
		}
		else
		{
			sector_Erase(blk_addr);
			printf("0x02\r\n");
		}

		if (remainSector == FLASH_SECORT_LEN)
		{
			printf("0x03\r\n");
			flash_Write_Block(lun, buf, (curSector * FLASH_SECORT_LEN),
			FLASH_SECORT_LEN);
		}
		else if (remainSector < FLASH_SECORT_LEN)
		{
			printf("0x04\r\n");
			flash_Write_Block(lun, eeprom_Cache, (curSector * FLASH_SECORT_LEN),
			FLASH_SECORT_LEN);
		}
		else
		{
			printf("0x05\r\n");
			flash_Write_Block(lun, buf, blk_addr, remainSector);
		}

		printf("w25qxx Debug(4)\r\n");
		blk_addr += remainSector;
		buf += remainSector;
		blk_len -= remainSector;
		curSector += 1;

		if (blk_len >= FLASH_BLOCK_LEN)
		{
			remainSector = FLASH_BLOCK_LEN;
		}
		else if (blk_len > FLASH_SECORT_LEN)
		{
			remainSector = FLASH_SECORT_LEN;
		}
		else
		{
			remainSector = blk_len;
		}
	}
	printf("w25qxx Debug(end)\r\n");
}

uint8_t flash_Data[0x1000];
void flash_Test()
{
	uint16_t tmp;
	uint32_t addr = 0x1;

	printf("test2\r\n");

	for (tmp = 0; tmp < sizeof(flash_Data); tmp++)
	{
		flash_Data[tmp] = (tmp + 1) % 0x100;
	}

	flash_Write(0x00, flash_Data, addr, sizeof(flash_Data) - 1);

	read_Data(flash_Data, addr, sizeof(flash_Data));

	for (tmp = 0; tmp < sizeof(flash_Data); tmp++)
	{
		if (tmp % 0x20 == 0)
		{
			printf("\r\n");
		}
		printf("%x ", flash_Data[tmp]);
	}
	printf("\r\n");
}

