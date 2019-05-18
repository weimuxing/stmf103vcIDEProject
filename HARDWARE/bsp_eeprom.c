/*
 * bsp_eeprom.c
 *
 *  Created on: May 9, 2019
 *      Author: Administrator
 */

#include "main.h"
#include "bsp_eeprom.h"
#include "iic_Interface.h"
#include <string.h>

#define EEPROM_DEBUG(x) x

#define AT24CXX_INFO_A 0xA0,32,(64/8*1024),AT24C64


typedef enum
{
	AT24CXX_A = 0, AT24C_MAX,
} AT24CXXChipPort;

typedef enum
{
	AT24C00 = 0,
	AT24C01,
	AT24C02,
	AT24C04,
	AT24C08,
	AT24C16,
	AT24C32,
	AT24C64,
	AT24C128,
	AT24C256,
	AT24C512,
	AT24C1024,
	AT24CMAX,
} AT24CXXChip;

typedef struct
{
	uint8_t chipAddr;
	uint16_t chipPageSize;
	uint32_t chipSize;
	AT24CXXChip at24cxxChip;
} AT24CXXInfo;

static AT24CXXChipPort at24cxxChipPort = AT24CXX_A;

static AT24CXXInfo at24cxxConfig[] =
{
{
AT24CXX_INFO_A }, };

bool at24cxxChipPortSet(AT24CXXChipPort portx)
{
	if (portx >= AT24C_MAX)
	{
		return false;
	}
	at24cxxChipPort = portx;
	return true;
}

static bool eeprom_Access_Start(AT24CXXChip at24cxxChip, uint8_t chipAddr,
		uint32_t blk_addr, I2cIoTransType trans_t)
{
	bool easFlag = true;

	easFlag = iic_Start();
	if (easFlag == false)
	{
		return easFlag;
	}

	if (at24cxxChip > AT24C16)
	{
		chipAddr |= (((blk_addr >> 16) << 1));
	}
	else
	{
		chipAddr |= (((blk_addr >> 8) << 1));
	}

	if (trans_t == I2C_TRANS_WRITE)
	{
		chipAddr &= (~0x01);
	}
	else
	{
		chipAddr |= 0x01;
	}

	easFlag = iic_Write(chipAddr);
	printf("chipAddr:%#x\r\n", chipAddr);
	return easFlag;
}

static bool eeprom_Write(AT24CXXChip at24cxxChip, uint8_t chipAddr,
		uint32_t blk_addr, uint8_t* buf, uint16_t blk_len)
{
	bool ewFlag = true;

	ewFlag = eeprom_Access_Start(at24cxxChip, chipAddr, blk_addr,
			I2C_TRANS_WRITE);
	if (ewFlag == false)
	{
		printf("ERR:ewAccess fail\r\n");
		goto EROMW_LOOP;
	}

	if (at24cxxChip > AT24C16)
	{
		ewFlag = iic_Write(blk_addr >> 8);
		if (ewFlag == false)
		{
			goto EROMW_LOOP;
		}
	}

	ewFlag = iic_Write(blk_addr);
	if (ewFlag == false)
	{
		goto EROMW_LOOP;
	}

	while (blk_len)
	{
		ewFlag = iic_Write(*buf);
		if (ewFlag == false)
		{
			goto EROMW_LOOP;
		}

		buf++;
		blk_len--;
	}

	EROMW_LOOP: iic_Stop();
	return ewFlag;
}

static bool eeprom_Read(AT24CXXChip at24cxxChip, uint8_t chipAddr,
		uint32_t blk_addr, uint8_t* buf, uint16_t blk_len)
{

	bool erFlag = true;

	erFlag = eeprom_Access_Start(at24cxxChip, chipAddr, blk_addr,
			I2C_TRANS_WRITE);
	if (erFlag == false)
	{
		goto EROMR_LOOP;
	}

	if (at24cxxChip > AT24C16)
	{
		erFlag = iic_Write(blk_addr >> 8);
		if (erFlag == false)
		{
			goto EROMR_LOOP;
		}
	}

	erFlag = iic_Write(blk_addr);
	if (erFlag == false)
	{
		goto EROMR_LOOP;
	}

	erFlag = eeprom_Access_Start(at24cxxChip, chipAddr, blk_addr,
			I2C_TRANS_READ);
	if (erFlag == false)
	{
		goto EROMR_LOOP;
	}

	while (blk_len)
	{
		blk_len--;
		*buf = iic_Read( blk_len);
		buf++;
	}

	EROMR_LOOP: iic_Stop();

	if (erFlag == false)
	{
		printf("Err:erFlag false\r\n");
	}
	return erFlag;
}

void eeporm_Write_Size(uint8_t lun, uint8_t *buf, uint32_t blk_addr,
		uint16_t blk_len)
{
	bool ewsAck = true;
	uint8_t chipAddr;
	uint16_t chipPageSize;
	uint32_t chipSize;
	AT24CXXChip at24cxxChip;
	uint16_t remainSize;

	if (at24cxxChipPortSet((AT24CXXChipPort) lun) == false)
	{
		printf("ERR:ews chip error\r\n");
		return;
	}

	chipAddr = at24cxxConfig[AT24CXX_A].chipAddr;
	chipPageSize = at24cxxConfig[AT24CXX_A].chipPageSize;
	chipSize = at24cxxConfig[AT24CXX_A].chipSize;
	at24cxxChip = at24cxxConfig[AT24CXX_A].at24cxxChip;
	remainSize = chipPageSize - blk_addr % chipPageSize;

	if (blk_addr > chipSize)
	{
		printf("ERR:ews addr cross the border\r\n");
		return;
	}

	if(blk_addr + blk_len > chipSize)
	{
		blk_len = chipSize - blk_addr;
		printf("Warring: eeprom w size overflow\r\n");
	}

	if (remainSize >= blk_len)
	{
		remainSize = blk_len;
	}

	while (blk_len)
	{
		//TODO
		ewsAck = eeprom_Write(at24cxxChip, chipAddr, blk_addr, buf, remainSize);

		if (remainSize >= blk_len)
		{
			blk_len = 0;
			remainSize = blk_len;
		}
		else
		{
			blk_len -= remainSize;
			buf += remainSize;
			blk_addr += remainSize;
			remainSize = chipPageSize;
		}

		HAL_Delay(10);
		if (ewsAck == false)
		{
			break;
		}
	}

	if (ewsAck == false)
	{
		printf("ERR:ewsAckerrblk_addr:%#lx\r\n", blk_addr);
	}
}

void eeporm_Read_Size(uint8_t lun, uint8_t *buf, uint32_t blk_addr,
		uint16_t blk_len)
{
	uint8_t chipAddr;
	uint32_t chipSize;
	AT24CXXChip at24cxxChip;


	if (at24cxxChipPortSet((AT24CXXChipPort) lun) == false)
	{
		printf("ERR:ers chip error\r\n");
		return;
	}

	chipAddr = at24cxxConfig[AT24CXX_A].chipAddr;
	chipSize = at24cxxConfig[AT24CXX_A].chipSize;
	at24cxxChip = at24cxxConfig[AT24CXX_A].at24cxxChip;

	if (blk_addr > chipSize)
	{
		return;
	}

	if(blk_addr + blk_len > chipSize)
	{
		blk_len = chipSize - blk_addr;
		printf("Warring: eeprom r size overflow\r\n");
	}
#if 1
	eeprom_Read(at24cxxChip, chipAddr, blk_addr, buf, blk_len);
#else
	uint16_t chipPageSize;
	bool ersAck = true;
	uint16_t remainSize;
	chipPageSize = at24cxxConfig[AT24CXX_A].chipPageSize;
	if (remainSize >= blk_len)
	{
		remainSize = blk_len;
	}
	while (blk_len)
	{
		//TODO
		eeprom_Read(at24cxxChip, chipAddr, blk_addr, buf, remainSize);
		printf("Rblk_addr:%x\r\n\r\n", blk_addr);

		if (remainSize >= blk_len)
		{
			remainSize = blk_len;
			blk_len = 0;
		}
		else
		{
			blk_len -= remainSize;
			buf += remainSize;
			blk_addr += remainSize;
			remainSize = chipPageSize;
		}

		HAL_Delay(10);
	}
	if (ersAck == false)
	{
		printf("ERR:ewsAckerrblk_addr:%#lx\r\n", blk_addr);
	}
#endif
}

uint8_t eeprom_TestData[256] =
{ 0, };

void eeprom_Test()
{
	uint16_t etTmp = 0;
	printf("eeprom_Test\r\n");
	for (etTmp = 0; etTmp < sizeof(eeprom_TestData); etTmp++)
	{
		if (etTmp % 0x10 == 0)
		{
			printf("\r\n");
		}
		eeprom_TestData[etTmp] = etTmp;
		printf("%d ", eeprom_TestData[etTmp]);
	}
	printf("write\r\n");
	eeporm_Write_Size(AT24CXX_A, eeprom_TestData, 0x10,
			sizeof(eeprom_TestData));
	memset(eeprom_TestData, 0x00, sizeof(eeprom_TestData));
	printf("read\r\n");
	eeporm_Read_Size(AT24CXX_A, eeprom_TestData, 0x10, sizeof(eeprom_TestData));

	for (etTmp = 0; etTmp < sizeof(eeprom_TestData); etTmp++)
	{
		if (etTmp % 0x10 == 0)
		{
			printf("\r\n");
		}
		printf("%02x ", eeprom_TestData[etTmp]);
	}
}
