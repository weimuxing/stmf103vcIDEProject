/*
 * iic_Interface.c
 *
 *  Created on: May 8, 2019
 *      Author: Administrator
 */

#include "main.h"
#include "iic_Interface.h"

typedef enum
{
	IICA = 0, IICB, IIC_MAX,
} IICPort;

typedef struct
{
	GPIO_TypeDef *SCLGPIOx;
	GPIO_TypeDef *SDAGPIOx;
	uint16_t SCLGPIO_Pin;
	uint16_t SDAGPIO_Pin;
} __IIC_Pin_Struct;

static __IIC_Pin_Struct iicPortConfig[IIC_MAX] =
{
{ IIC_PORT_A },
{ IIC_PORT_B }, };

static uint8_t iicPortSel = IICA;

void iicPortSelSet(uint8_t portx)
{
	if (portx >= IIC_MAX)
	{
		printf("error:IICPORT SET ERR\r\n");
	}
	else
	{
		iicPortSel = portx;
	}
}

void iic_Delay()
{
	uint16_t tim = 10 * 5;
	while (tim--)
	{

	}
}

bool iic_Start(void)
{
	bool iSFlag = true;

	IIC_SDA_SET(PIN_HIGH);
	IIC_SCL_SET(PIN_HIGH);
	iic_Delay();

	if ((IIC_SCL_GET == PIN_LOW) || (ICI_SDA_GET == PIN_LOW))
	{
		printf("ERR:IIC start fail\r\n");
		iSFlag = false;
		return iSFlag;
	}
	else
	{
		IIC_SDA_SET(PIN_LOW);
		iic_Delay();
		IIC_SCL_SET(PIN_LOW);
	}

	return iSFlag;
}

bool iic_Write(uint8_t data)
{
	uint8_t wFlag = 0x80;
	bool wAck = true;
	while (wFlag)
	{
		if (data & wFlag)
		{
			IIC_SDA_SET(PIN_HIGH);
		}
		else
		{
			IIC_SDA_SET(PIN_LOW);
		}

		iic_Delay();
		IIC_SCL_SET(PIN_HIGH);
		iic_Delay();
		IIC_SCL_SET(PIN_LOW);
		wFlag >>= 1;
	}

	IIC_SDA_INPUT;
	iic_Delay();
	IIC_SCL_SET(PIN_HIGH);
	iic_Delay();

	if (ICI_SDA_GET == PIN_HIGH)
	{
		wAck = false;
	}

	IIC_SCL_SET(PIN_LOW);
	return wAck;
}

uint8_t iic_Read(uint32_t rAck)
{
	uint8_t wFlag = 0x80;
	uint8_t rData = 0x00;

	IIC_SDA_INPUT;
	while (wFlag)
	{
		if (ICI_SDA_GET == PIN_HIGH)
		{
			rData |= wFlag;
		}

		iic_Delay();
		IIC_SCL_SET(PIN_HIGH);
		iic_Delay();
		IIC_SCL_SET(PIN_LOW);
		wFlag >>= 1;
	}

	if (rAck == 0)
	{
		printf("T\r\n");
		IIC_SDA_SET(PIN_HIGH);
	}
	else
	{
		printf("N\r\n");
		IIC_SDA_SET(PIN_LOW);
	}
	iic_Delay();

	IIC_SCL_SET(PIN_HIGH);
	iic_Delay();
	IIC_SCL_SET(PIN_LOW);
	return rData;
}

void iic_Stop(void)
{
	IIC_SDA_SET(PIN_LOW);
	iic_Delay();
	IIC_SCL_SET(PIN_HIGH);
	iic_Delay();
	IIC_SDA_SET(PIN_HIGH);
	iic_Delay();
}

