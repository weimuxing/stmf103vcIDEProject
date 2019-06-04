/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"		/* Obtains integer types */
#include "diskio.h" /* Declarations of disk functions */

/* Definitions of physical drive number for each drive */
#define DEV_RAM 0 /* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC 1 /* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB 2 /* Example: Map USB MSD to physical drive 2 */
#define DEV_SPI_FLASH "3:"

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(
	BYTE pdrv /* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv)
	{
	case DEV_RAM:
		printf("DEV_RAM:disk_status\r\n");
		//result = RAM_disk_status();

		// translate the reslut code here

		return stat;

	case DEV_MMC:
		printf("DEV_MMC:disk_status\r\n");
		//result = MMC_disk_status();

		// translate the reslut code here

		return stat;

	case DEV_USB:
		//result = USB_disk_status();

		// translate the reslut code here

		return stat;
	}
	return stat;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(
	BYTE pdrv /* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = RES_OK;
	int result;

	switch (pdrv)
	{
	case DEV_RAM:
		printf("DEV_RAM:disk_initialize\r\n");
		//result = RAM_disk_initialize();

		// translate the reslut code here

		return stat;

	case DEV_MMC:
		printf("DEV_MMC:disk_initialize\r\n");
		//result = MMC_disk_initialize();

		// translate the reslut code here

		return stat;

	case DEV_USB:
		//result = USB_disk_initialize();

		// translate the reslut code here

		return stat;
	}
	return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(
	BYTE pdrv,	/* Physical drive nmuber to identify the drive */
	BYTE *buff,   /* Data buffer to store read data */
	DWORD sector, /* Start sector in LBA */
	UINT count	/* Number of sectors to read */
)
{
	DRESULT res;
	int result;

	switch (pdrv)
	{
	case DEV_RAM:
		printf("DEV_RAM:disk_read\r\n");
		read_Data(buff, sector * 4096, count * 4096);
		// translate the arguments here

		//result = RAM_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_MMC:
		printf("DEV_MMC:disk_read\r\n");
		// translate the arguments here

		//result = MMC_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_USB:
		// translate the arguments here

		//result = USB_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write(
	BYTE pdrv,		  /* Physical drive nmuber to identify the drive */
	const BYTE *buff, /* Data to be written */
	DWORD sector,	 /* Start sector in LBA */
	UINT count		  /* Number of sectors to write */
)
{
	DRESULT res;
	int result;
	printf("disk_write\r\n");
	switch (pdrv)
	{
	case DEV_RAM:
		flash_Write(pdrv, buff, sector * 4096, count * 4096);
		// translate the arguments here

		//result = RAM_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_MMC:
		// translate the arguments here

		//result = MMC_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_USB:
		// translate the arguments here

		//result = USB_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
}

#endif

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(
	BYTE pdrv, /* Physical drive nmuber (0..) */
	BYTE cmd,  /* Control code */
	void *buff /* Buffer to send/receive control data */
)
{
	DRESULT res;
	int result;
	printf("disk_ioctl\r\n");
	switch (pdrv)
	{
	case DEV_RAM:

		// Process of the command for the RAM drive
		if (cmd == GET_SECTOR_COUNT)
		{
			*(WORD *)buff = (4096 - 1);
		}
		else if (cmd == GET_SECTOR_SIZE)
		{
			*(WORD *)buff = 4096;
		}
		return res;

	case DEV_MMC:

		// Process of the command for the MMC/SD card

		return res;

	case DEV_USB:

		// Process of the command the USB drive

		return res;
	}

	return RES_PARERR;
}
