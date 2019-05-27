/*
 * fatfs_user.c
 *
 *  Created on: May 27, 2019
 *      Author: Administrator
 */

#include "main.h"
#include "fatfs_user.h"
#include "user_diskio.h"
#include "ff.h"
#include "fatfs.h"

FATFS fs;                 // Work area (file system object) for logical drive
FIL fil;                  // file objects

uint32_t byteswritten;                /* File write counts */
uint32_t bytesread;                   /* File read counts */
uint8_t wtext[] = "This is STM32 working with FatFs"; /* File write buffer */
uint8_t rtext[100];                     /* File read buffers */
char filename[] = "STM32cube.txt";

void mount_disk(void)
{
	uint8_t res;

	res = f_mount(&fs, "", 1);
	if (res)
	{
		printf("mount error :%d\r\n", res);

		return;
	}
	else
	{
		printf("mount success!!!\r\n");
	}

	res = f_open(&fil, filename, FA_CREATE_ALWAYS | FA_WRITE);
	{
		if(res)
		{
			printf("open fail:%d\r\n",res);
		}
		{
			printf("open success:%d\r\n",res);
		}
	}


	res = f_write(&fil, wtext, sizeof(wtext), (void *)&byteswritten);
	{
		if(res)
		{
			printf("write fail:%d\r\n",res);
		}
		else
		{
			printf("write success:%d\r\n",res);
		}
	}

	res = f_read(&fil, rtext, sizeof(rtext), (UINT*)&bytesread);
	{
		if(res)
		{
			printf("read fail:%d\r\n",res);
		}
		else
		{
			printf("read success:%d\r\n",res);
		}
	}
	printf("%s\r\n",rtext);
	printf("fatfs test end\r\n");
}

