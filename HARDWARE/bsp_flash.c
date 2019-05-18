/*
 * bsp_flash.c
 *
 *  Created on: 2019年5月15日
 *      Author: Administrator
 */

#include "bsp_flash.h"
#include <string.h>

extern SPI_HandleTypeDef hspi1;

#define SPI1_CS_HIGH  HAL_GPIO_WritePin(SPI1_CS_GPIO_Port,  SPI1_CS_Pin, GPIO_PIN_SET )
#define SPI1_CS_LOW  HAL_GPIO_WritePin(SPI1_CS_GPIO_Port,  SPI1_CS_Pin, GPIO_PIN_RESET )

uint8_t eeprom_Cache[0x1000];

void write_Enable()
{
	uint8_t data = 0x06;
	SPI1_CS_LOW;
	HAL_SPI_Transmit(&hspi1, &data, 1, 0xFFFF);
	SPI1_CS_HIGH;
}

void write_Disable()
{
	uint8_t data = 0x04;
	SPI1_CS_LOW;
	HAL_SPI_Transmit(&hspi1, &data, 1, 0xFFFF);
	SPI1_CS_HIGH;
}

void RSTEN()
{
	uint8_t data = 0x66;
	SPI1_CS_LOW;
	HAL_SPI_Transmit(&hspi1, &data, 1, 0xFFFF);
	SPI1_CS_HIGH;
}

void read_Status_Register_1()
{
	uint8_t commond = 0x05;
	uint8_t status_Register = 0x00;
	SPI1_CS_LOW;
	HAL_SPI_Transmit(&hspi1, &commond, 1, 0xFFFF);
	HAL_SPI_TransmitReceive(&hspi1, &status_Register, &status_Register, 1,
			0xFFFFFF);
	SPI1_CS_HIGH;
}

void sector_Erase(uint32_t blk_addr)
{
	uint8_t commond = 0x20;
	SPI1_CS_LOW;
	HAL_SPI_Transmit(&hspi1, (uint8_t*) &commond, 3, 0xFFFF);
	SPI1_CS_HIGH;
}

void block_Erase()
{
	uint8_t commond = 0xD8;
	SPI1_CS_LOW;
	HAL_SPI_Transmit(&hspi1, (uint8_t*) &commond, 3, 0xFFFF);
	SPI1_CS_HIGH;
}

void deep_power_Down()
{
	uint8_t commond = 0xB9;
	SPI1_CS_LOW;
	HAL_SPI_Transmit(&hspi1, (uint8_t*) &commond, 1, 0xFFFF);
	SPI1_CS_HIGH;
}

void page_Program(uint32_t blk_addr, uint32_t blk_len, uint8_t *buf)
{
	uint8_t commond = 0x06;
	SPI1_CS_LOW;
	HAL_SPI_Transmit(&hspi1, &commond, 1, 0xFFFF);
	HAL_SPI_Transmit(&hspi1, (uint8_t*) &blk_addr, 3, 0xFFFF);
	HAL_SPI_Transmit(&hspi1, (uint8_t*) &buf, blk_len, 0xFFFFFF);
	SPI1_CS_HIGH;
}

void read_Data(uint32_t blk_addr, uint32_t blk_len, uint8_t *buf)
{
	uint8_t commond = 0x03;
	SPI1_CS_LOW;
	HAL_SPI_Transmit(&hspi1, &commond, 1, 0xFFFF);
	HAL_SPI_Transmit(&hspi1, (uint8_t*) &blk_addr, 3, 0xFFFF);
	HAL_SPI_TransmitReceive(&hspi1, buf, buf, blk_len, 0xFFFFFF);
	SPI1_CS_HIGH;
}

void read_Manufacturer_Device_ID()
{
	uint8_t pRxTxData[3];
	uint8_t data = 0x90;
	SPI1_CS_LOW;
	HAL_SPI_Transmit(&hspi1, &data, 1, 0xFFFF);
	HAL_SPI_Transmit(&hspi1, pRxTxData, 2, 0xFFFF);
	HAL_SPI_TransmitReceive(&hspi1, pRxTxData, pRxTxData, 3, 0xFFFF);
	SPI1_CS_HIGH;
	printf("flashID:%x %x %x\r\n", pRxTxData[0], pRxTxData[1], pRxTxData[2]);
}

void read_Identification()
{
	uint8_t data = 0x9F;
	uint8_t pRxTxData[3];
	SPI1_CS_LOW;
	HAL_SPI_Transmit(&hspi1, &data, 1, 0xFFFF);
	HAL_SPI_TransmitReceive(&hspi1, pRxTxData, pRxTxData, 3, 0xFFFF);
	SPI1_CS_HIGH;
	printf("flashID:%x %x %x\r\n", pRxTxData[0], pRxTxData[1], pRxTxData[2]);
}

// commond

void flash_Write(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint32_t blk_len)
{
uint16_t fwTmp = 0;
uint16_t remainSector = 0;
uint16_t curSector;

curSector = blk_addr / FLASH_SECORT_LEN;
remainSector = FLASH_SECORT_LEN - blk_addr % FLASH_SECORT_LEN;

if (blk_len < remainSector)
{
	blk_len = remainSector;
}

while (blk_len)
{
	if (remainSector < FLASH_SECORT_LEN)
	{
		read_Data(curSector * FLASH_SECORT_LEN, FLASH_SECORT_LEN, eeprom_Cache);
		memcpy((eeprom_Cache + blk_addr % FLASH_SECORT_LEN), buf, remainSector);
	}
	else
	{
		memcpy(eeprom_Cache, buf, remainSector);
	}
	sector_Erase(blk_addr);

	for (fwTmp = 0; fwTmp < FLASH_BLOCK_LEN / FLASH_SECORT_LEN; fwTmp++)
	{
		page_Program((blk_addr + fwTmp * FLASH_PAGE_LEN), FLASH_PAGE_LEN,
				(eeprom_Cache + fwTmp * FLASH_PAGE_LEN));
	}

	buf += remainSector;
	blk_len -= remainSector;
	remainSector = FLASH_SECORT_LEN;

}

}

