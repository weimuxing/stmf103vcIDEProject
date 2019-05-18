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

void page_Program(uint8_t *buf, uint32_t blk_addr, uint32_t blk_len)
{
	uint8_t commond = 0x06;
	SPI1_CS_LOW;
	HAL_SPI_Transmit(&hspi1, &commond, 1, 0xFFFF);
	HAL_SPI_Transmit(&hspi1, (uint8_t*) &blk_addr, 3, 0xFFFF);
	HAL_SPI_Transmit(&hspi1, (uint8_t*) &buf, blk_len, 0xFFFFFF);
	SPI1_CS_HIGH;
}

void read_Data(uint8_t *buf, uint32_t blk_addr, uint32_t blk_len)
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

	curSector = blk_addr / FLASH_SECORT_LEN;
	remainSector = FLASH_SECORT_LEN - blk_addr % FLASH_SECORT_LEN;

	if (blk_len < remainSector)
	{
		blk_len = remainSector;
	}

	while (blk_len)
	{
		if (blk_len < FLASH_SECORT_LEN)
		{
			read_Data(eeprom_Cache, curSector * FLASH_SECORT_LEN,
			FLASH_SECORT_LEN);
			memcpy((eeprom_Cache + blk_addr % FLASH_SECORT_LEN), buf,
					remainSector);
			sector_Erase(blk_addr);
		}
		else if (remainSector == FLASH_BLOCK_LEN)
		{
			//TODO: erase block function
		}
		else
		{
			sector_Erase(blk_addr);
		}

		if (remainSector == FLASH_SECORT_LEN)
		{
			flash_Write_Block(lun, buf, blk_addr, remainSector);
		}
		else if (remainSector < FLASH_SECORT_LEN)
		{
			flash_Write_Block(lun, eeprom_Cache, blk_addr, remainSector);
		}
		else
		{
			flash_Write_Block(lun, buf, blk_addr, remainSector);
		}

		buf += remainSector;
		blk_len -= remainSector;

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
}

void flash_Test()
{
	uint16_t tmp;
	for (tmp = 0; tmp < sizeof(eeprom_Cache); tmp++)
	{
		eeprom_Cache[tmp] = tmp % 0x100;
	}

	flash_Write(0x00, eeprom_Cache, 0x1010, sizeof(eeprom_Cache));
	memset(eeprom_Cache, 0x00, sizeof(eeprom_Cache));
	read_Data(eeprom_Cache, 0x1010, sizeof(eeprom_Cache));

	for (tmp = 0; tmp < sizeof(eeprom_Cache); tmp++)
	{
		if (tmp % 0x20 == 0)
		{
			printf("\r\n");
		}
		printf("%x ", eeprom_Cache[tmp]);

	}
}

