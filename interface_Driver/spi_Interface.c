/*
 * spi_Interface.c
 *
 *  Created on: May 8, 2019
 *      Author: Administrator
 */

#include "spi_Interface.h"

//HAL_SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout)
//HAL_StatusTypeDef HAL_SPI_Receive(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout)
//HAL_SPI_Receive(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout)
//HAL_StatusTypeDef HAL_SPI_TransmitReceive(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size, uint32_t Timeout)
//

//static uint16_t sSpiLockBit = 0;

SPI_HandleTypeDef sspi1;
extern SPI_HandleTypeDef hspi1;

#if 0
void MX_SPI1_Init(void)
{

  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }

}
#endif

void soft_SPI_Init()
{
	{
		sspi1.Init.DataSize = SPI_DATASIZE_8BIT;
		sspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
		sspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
		sspi1.Init.BaudRatePrescaler = 0x01;

//		sspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
//	  	sspi1.Init.TIMode = SPI_TIMODE_DISABLE;
//	  	sspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
//	  	sspi1.Init.CRCPolynomial = 10;
//		sspi1.Instance = SSPI1;
//		sspi1.Init.Mode = SPI_MODE_MASTER;
//		sspi1.Init.Direction = SPI_DIRECTION_2LINES;
//		sspi1.Init.NSS = SPI_NSS_SOFT;
	}

}

void soft_SPI_Transmit(soft_SPI_HandleTypeDef *sspi1, uint8_t* pTxData,
		uint32_t Size)
{
	uint16_t u16sSpiPos;
	GPIO_PinState CLKPolarity;
	uint32_t CLKPhase = sspi1->Init.CLKPhase;

	if (sspi1->Init.CLKPolarity == SPI_POLARITY_LOW)
	{
		CLKPolarity = GPIO_PIN_RESET;
	}
	else
	{
		CLKPolarity = GPIO_PIN_SET;
	}

	while (Size)
	{
		while (u16sSpiPos)
		{
			if (CLKPhase == SPI_PHASE_1EDGE)
			{
				if (*pTxData & u16sSpiPos)
				{
					HAL_GPIO_WritePin(SOFT_SPI0_MOSI_GPIO_Port,
					SOFT_SPI0_MOSI_Pin, GPIO_PIN_SET);
				}
				else
				{
					HAL_GPIO_WritePin(SOFT_SPI0_MOSI_GPIO_Port,
					SOFT_SPI0_MOSI_Pin, GPIO_PIN_RESET);
				}
				u16sSpiPos >>= 1;
			}
			else
			{
				CLKPhase == SPI_PHASE_1EDGE;
			}

			HAL_GPIO_WritePin(SOFT_SPI_CLK_GPIO_Port, SOFT_SPI_CLK_Pin,
					!CLKPolarity);

			HAL_GPIO_WritePin(SOFT_SPI_CLK_GPIO_Port, SOFT_SPI_CLK_Pin,
					CLKPolarity);

		}
		//else
		{
			Size--;
			CLKPhase = sspi1->Init.CLKPhase;

			if (sspi1->Init.DataSize == SPI_DATASIZE_16BIT)
			{
				pTxData += sizeof(uint16_t);

				u16sSpiPos = 0x8000;
			}
			else
			{
				pTxData += sizeof(uint8_t);
				u16sSpiPos = 0x0080;
			}
		}
	}
}

void soft_SPI_Receive(soft_SPI_HandleTypeDef *sspi1, uint8_t* pRxData,
		uint32_t Size)
{
	uint16_t u16sSpiPos = 0x8000;
	uint16_t u16sSpiRecDat = 0x00;
	uint32_t CLKPhase = sspi1->Init.CLKPhase;
	GPIO_PinState CLKPolarity;

	if (sspi1->Init.CLKPolarity == SPI_POLARITY_LOW)
	{
		CLKPolarity = GPIO_PIN_RESET;
	}
	else
	{
		CLKPolarity = GPIO_PIN_SET;
	}

	while (Size)
	{
		while (u16sSpiPos)
		{
			if (CLKPhase == SPI_PHASE_1EDGE)
			{
				if (HAL_GPIO_ReadPin(SOFT_SPI0_MISO_GPIO_Port,
						SOFT_SPI0_MISO_Pin))
				{
					u16sSpiRecDat |= u16sSpiPos;
				}
			}

			HAL_GPIO_WritePin(SOFT_SPI_CLK_GPIO_Port, SOFT_SPI_CLK_Pin,
					!CLKPolarity);

			HAL_GPIO_WritePin(SOFT_SPI_CLK_GPIO_Port, SOFT_SPI_CLK_Pin,
					CLKPolarity);

			u16sSpiPos >>= 1;
			CLKPhase = SPI_PHASE_1EDGE;

		}
		//else
		{
			Size--;
			CLKPhase = sspi1->Init.CLKPhase;

			if (sspi1->Init.DataSize == SPI_DATASIZE_16BIT)
			{
				pRxData += sizeof(uint16_t);

				u16sSpiPos = 0x8000;
			}
			else
			{
				pRxData += sizeof(uint8_t);
				u16sSpiPos = 0x0080;
			}
		}
	}
}

void soft_SPI_TransmitReceive(soft_SPI_HandleTypeDef *sspi1, uint8_t *pTxData,
		uint8_t *pRxData, uint16_t Size)
{
	uint16_t u16sSpiPos = 0x8000;
	uint16_t u16sSpiRecDat = 0x00;
	uint32_t CLKPhase = sspi1->Init.CLKPhase;
	GPIO_PinState CLKPolarity;

	if (sspi1->Init.CLKPolarity == SPI_POLARITY_LOW)
	{
		CLKPolarity = GPIO_PIN_RESET;
	}
	else
	{
		CLKPolarity = GPIO_PIN_SET;
	}

	while (Size)
	{
		while (u16sSpiPos)
		{
			if (CLKPhase == SPI_PHASE_1EDGE)
			{
				if (*pTxData & u16sSpiPos)
				{
					HAL_GPIO_WritePin(SOFT_SPI0_MOSI_GPIO_Port,
							SOFT_SPI0_MOSI_Pin, GPIO_PIN_SET);
				}
				else
				{
					HAL_GPIO_WritePin(SOFT_SPI0_MOSI_GPIO_Port,
							SOFT_SPI0_MOSI_Pin, GPIO_PIN_RESET);
				}

				if (HAL_GPIO_ReadPin(SOFT_SPI0_MISO_GPIO_Port,
						SOFT_SPI0_MISO_Pin))
				{
					u16sSpiRecDat |= u16sSpiPos;
				}
			}

			HAL_GPIO_WritePin(SOFT_SPI_CLK_GPIO_Port, SOFT_SPI_CLK_Pin,
					!CLKPolarity);

			HAL_GPIO_WritePin(SOFT_SPI_CLK_GPIO_Port, SOFT_SPI_CLK_Pin,
					CLKPolarity);

			u16sSpiPos >>= 1;
			CLKPhase == SPI_PHASE_1EDGE;
		}
		//else
		{
			Size--;
			CLKPhase = sspi1->Init.CLKPhase;

			if (sspi1->Init.DataSize == SPI_DATASIZE_16BIT)
			{
				pTxData += sizeof(uint16_t);

				u16sSpiPos = 0x8000;
			}
			else
			{
				pTxData += sizeof(uint8_t);
				u16sSpiPos = 0x0080;
			}
		}
	}

}

void SPI_Transmit(SPI_HandleTypeDef *hspi , uint8_t* pData, uint16_t Size, uint32_t Timeout)
{
	HAL_SPI_Transmit(hspi, pData, Size, Timeout);
}

void SPI_Receive(SPI_HandleTypeDef *hspi , uint8_t* pData, uint16_t Size, uint32_t Timeout)
{
	HAL_SPI_Receive(hspi, pData, Size, Timeout);
}

void SPI_TransmitReceive(SPI_HandleTypeDef *hspi , uint8_t* pTxData, uint8_t* pRxData, uint16_t Size,
		uint32_t Timeout)
{
	HAL_SPI_TransmitReceive(hspi, pTxData, pRxData, Size, Timeout);
}

