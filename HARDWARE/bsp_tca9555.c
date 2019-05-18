/*
 * bsp_TCA9555.c
 *
 *  Created on: 2019年5月15日
 *      Author: Administrator
 */

#include <bsp_tca9555.h>

#define TCA9555CHIPADDR  0x20

static uint8_t _tca9555chipAddr = TCA9555CHIPADDR;

void tca95555chipAddrSet(uint8_t chipAddr)
{
	_tca9555chipAddr = chipAddr;
}

void tca9555_init()
{

}

static bool tca9555_Access_Start(uint8_t chipAddr, I2cIoTransType trans_t)
{
	bool easFlag = true;

	easFlag = iic_Start();
	if (easFlag == false)
	{
		return easFlag;
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

void tca9555_Write(uint8_t *buf, uint8_t blk_addr, uint16_t blk_len)
{
	tca9555_Access_Start(_tca9555chipAddr, I2C_TRANS_WRITE);
	iic_Write(blk_addr);
	iic_Write((*buf) >> 8);
	iic_Write((uint8_t) (*buf));
}

void tca9555_Read(uint8_t *buf, uint8_t blk_addr, uint16_t blk_len)
{

	tca9555_Access_Start(_tca9555chipAddr, I2C_TRANS_READ);
	iic_Write(blk_addr);
	*buf = iic_Read(0x01);
	*buf <<= 8;
	*buf |= iic_Read(0x00);
}
