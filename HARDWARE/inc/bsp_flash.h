/*
 * bsp_flash.h
 *
 *  Created on: 2019年5月15日
 *      Author: Administrator
 */

#ifndef INC_BSP_FLASH_H_
#define INC_BSP_FLASH_H_

#include "main.h"

#define FLASH_PAGE_LEN   (0x100)
#define FLASH_SECORT_LEN (0x1000)
#define FLASH_SECORT_COUNT  (0x1000)
#define FLASH_BLOCK_LEN		(0x10000)
#define FLASH_BLOCK_COUNT	(0x100)
#define FLASH_TOTAL_LEN  (FLASH_SECORT_LEN*FLASH_SECORT_COUNT)




#endif /* INC_BSP_FLASH_H_ */
