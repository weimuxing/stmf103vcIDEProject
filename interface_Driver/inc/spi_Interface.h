/*
 * spi_Interface.h
 *
 *  Created on: May 8, 2019
 *      Author: Administrator
 */

#ifndef INC_SPI_INTERFACE_H_
#define INC_SPI_INTERFACE_H_

#include "main.h"

typedef struct __soft_SPI_HandleTypeDef
{
	SPI_InitTypeDef Init; /*!< SPI communication parameters */

	HAL_LockTypeDef Lock; /*!< Locking object                 */

	__IO HAL_SPI_StateTypeDef State; /*!< SPI communication state */

	__IO uint32_t TimeCount;

	__IO uint32_t ErrorCode; /*!< SPI Error code */

} soft_SPI_HandleTypeDef;


#endif /* INC_SPI_INTERFACE_H_ */
