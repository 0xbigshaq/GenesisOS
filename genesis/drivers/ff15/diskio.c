#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "drivers/ata.h"

/* Definitions of physical drive number for each drive */
#define DEV_SDA		0	
#define DEV_MMC		1	
#define DEV_USB		2	


DSTATUS disk_status(BYTE pdrv) {
	DSTATUS rc;

	switch (pdrv) {
	case DEV_SDA :
		rc = ata_wait_ready() ? RES_OK : STA_NOINIT;
		break;
	default:
		rc = STA_NOINIT;
		break;
	}
	return rc;
}

DSTATUS disk_initialize(BYTE pdrv) {
	DSTATUS rc;
	int lba = 0;

	switch (pdrv) {
	case DEV_SDA :
		// initialize
	    outb(ATA_SECTOR_COUNT_REG, 1);
	    // // Set block
		outb(ATA_LBA_LOW_REG, lba & 0xFF);
		outb(ATA_LBA_MID_REG, (lba >> 8) & 0xFF);
		outb(ATA_LBA_HIGH_REG, (lba >> 16) & 0xFF);
		outb(ATA_DEVICE_REG, 0xE0 | ((lba >> 24) & 0x0F));
		rc =  ata_wait_ready() ? RES_OK : RES_NOTRDY;
		break;
	default:
		rc = STA_NOINIT;
		break;
	}
	return rc;
}


DRESULT disk_read(BYTE pdrv, BYTE *buff, LBA_t sector, UINT count) {
	DRESULT rc;

	switch (pdrv) {
	case DEV_SDA :
		rc = ata_disk_read(buff, sector, count);
		break;
	default:
		rc = STA_NOINIT;
		break;
	}

	return rc;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

/* BYTE pdrv		Physical drive nmuber to identify the drive */
/* BYTE *buff		Data to be written */
/* LBA_t sector		Start sector in LBA */
/* UINT count		Number of sectors to write */
DRESULT disk_write(BYTE pdrv, const BYTE *buff, LBA_t sector, UINT count) {
	DRESULT rc;

	switch (pdrv) {
	case DEV_SDA :
		rc = ata_disk_write(buff, sector, count);
		break;
	default:
		rc = STA_NOINIT;
		break;
	}

	return rc;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

/* BYTE pdrv		Physical drive nmuber (0..) */
/* BYTE cmd			Control code */
/* void *buff		Buffer to send/receive control data */
DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void *buff) {
	// DRESULT res;
	// int result;

	// switch (pdrv) {
	// case DEV_RAM :

	// 	// Process of the command for the RAM drive

	// 	return res;

	// case DEV_MMC :

	// 	// Process of the command for the MMC/SD card

	// 	return res;

	// case DEV_USB :

	// 	// Process of the command the USB drive

	// 	return res;
	// }

	return RES_PARERR;
}

