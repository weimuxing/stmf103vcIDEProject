/*
 * fatfs_user.c
 *
 *  Created on: May 27, 2019
 *      Author: Administrator
 */
#if 1
#include "main.h"
#include "fatfs_user.h"
#include "diskio.h"
#include "ff.h"

FATFS USERFatFS; /* File system object for USER logical drive */
FIL USERFile; /* File object for USER */

//FATFS fs;                 // Work area (file system object) for logical drive
//FIL fil;                  // file objects

uint32_t byteswritten; /* File write counts */
uint32_t bytesread; /* File read counts */
uint8_t wtext[] = "STM32 FatFs 测试"; /* File write buffer */
uint8_t rtext[25]; /* File read buffers */
char filename[] = "stm32fatfs。txt";
char filename1[] = "test.txt";

TCHAR dev_ram = 0;
TCHAR dev_mmc = 1;

void mount_disk(void)
{
	printf("\r\n ****** FatFs Example ******\r\n\r\n");
	FRESULT retSD;
	/*##-1- Register the file system object to the FatFs module ##############*/
	retSD = f_mount(&USERFatFS, "", 1);
	if (retSD)
	{
		printf(" mount error : %d \r\n", retSD);
		Error_Handler();
	}
	else
		printf(" mount sucess!!! \r\n");

	/*##-2- Create and Open new text file objects with write access ######*/
	retSD = f_open(&USERFile, filename, FA_CREATE_ALWAYS | FA_WRITE);
	if (retSD)
		printf(" open file error : %d\r\n", retSD);
	else
		printf(" open file sucess!!! \r\n");

	/*##-3- Write data to the text files ###############################*/
	retSD = f_write(&USERFile, wtext, sizeof(wtext), (void *) &byteswritten);
	if (retSD)
		printf(" write file error : %d\r\n", retSD);
	else
	{
		printf(" write file sucess!!! \r\n");
		printf(" write Data : %s\r\n", wtext);
	}

	/*##-4- Close the open text files ################################*/
	retSD = f_close(&USERFile);
	if (retSD)
		printf(" close error : %d\r\n", retSD);
	else
		printf(" close sucess!!! \r\n");

	/*##-5- Open the text files object with read access ##############*/
	retSD = f_open(&USERFile, filename, FA_READ);
	if (retSD)
		printf(" open file error : %d\r\n", retSD);
	else
		printf(" open file sucess!!! \r\n");

	/*##-6- Read data from the text files ##########################*/
	retSD = f_read(&USERFile, rtext, sizeof(rtext), (UINT*) &bytesread);
	if (retSD)
		printf(" read error!!! %d\r\n", retSD);
	else
	{
		printf(" read sucess!!! \r\n");
		printf(" read Data : %s\r\n", rtext);
	}

	/*##-7- Close the open text files ############################*/
	retSD = f_close(&USERFile);
	if (retSD)
		printf(" close error!!! %d\r\n", retSD);
	else
		printf(" close sucess!!! \r\n");

	retSD = f_open(&USERFile, filename1, FA_READ);
	if (retSD)
		printf(" open file1 error : %d\r\n", retSD);
	else
		printf(" open file1 sucess!!! \r\n");

	/*##-6- Read data from the text files ##########################*/
	retSD = f_read(&USERFile, rtext, sizeof(rtext), (UINT*) &bytesread);
	if (retSD)
		printf(" read1 error!!! %d\r\n", retSD);
	else
	{
		printf(" read1 sucess!!! \r\n");
		printf(" read1 Data : %s\r\n", rtext);
	}

	/*##-7- Close the open text files ############################*/
	retSD = f_close(&USERFile);
	if (retSD)
		printf(" close1 error!!! %d\r\n", retSD);
	else
		printf(" close1 sucess!!! \r\n");

	/*##-8- Compare read data with the expected data ############*/
	if (bytesread == byteswritten)
	{
		printf(" FatFs is working well!!!\r\n");
	}
}
#endif
