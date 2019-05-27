/*
 * bsp_flash.h
 *
 *  Created on: 2019年5月15日
 *      Author: Administrator
 */

#ifndef INC_BSP_FLASH_H_
#define INC_BSP_FLASH_H_

#include "main.h"

#define XX25X8  8
#define XX25X16 16
#define XX25X32 32
#define XX25X64 64
#define XX25X128 128
#define XX25X256 256



#define FLASH_PAGE_LEN   0x100
#define FLASH_SECORT_LEN 0x1000
#define FLASH_SECORT_COUNT  0x1000
#define FLASH_BLOCK_LEN		0x10000
#define FLASH_BLOCK_COUNT	(0x100)
#define FLASH_TOTAL_LEN  (FLASH_SECORT_LEN*FLASH_SECORT_COUNT)

#define PAGEPROGRAMTIME 5
#define SECTORERASETIME 300
#define BLOCKERASETIME (2*1000)
#define CHIPERASETIME (140*1000)
#define WRITESTATUSREGISTERTIME 50

#define SPI1_CS_HIGH  HAL_GPIO_WritePin(SPI1_CS_GPIO_Port,  SPI1_CS_Pin, GPIO_PIN_SET )
#define SPI1_CS_LOW  HAL_GPIO_WritePin(SPI1_CS_GPIO_Port,  SPI1_CS_Pin, GPIO_PIN_RESET )

#define BIG2LITTLE16(x) do{	x =					\
		x = (( ((uint16_t)(A) & 0xff00) >> 8)	|\
		(( (uint16_t)(A) & 0x00ff) << 8));		\
}while(0);

#define BIG2LITTLE32(x) do{	x =					\
		(( ((uint32_t)(x) & 0xff000000) >> 24)	|\
		(( (uint32_t)(x) & 0x00ff0000) >> 8)	|\
		(( (uint32_t)(x) & 0x0000ff00) << 8)	|\
		(( (uint32_t)(x) & 0x000000ff) << 24));	\
}while(0);


#endif /* INC_BSP_FLASH_H_ */
