#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "drivers/ata.h"
#include "drivers/fat32.h"

/* Definitions of physical drive number for each drive */
#define DEV_SDA		0	
#define DEV_MMC		1	
#define DEV_USB		2	

/**
 * @brief Check if disk is ready
 * 
 * @param pdrv Physical drive number to identify the drive
 * @return The current status of the disk (`RES_OK`, `STA_NOINIT`)
 */
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

/**
 * @brief Initialize the disk
 * 
 * @param pdrv Physical drive number to identify the drive
 * @return The status of the disk (`RES_OK`, `RES_NOTRDY`)
 */
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

/**
 * @brief Read data from the disk
 * 
 * @param pdrv Physical drive number to identify the drive
 * @param buff Pointer to the data buffer to store the read data
 * @param sector Start sector number to read from
 * @param count Number of sectors to read
 * @return The result of the read operation (`RES_OK`, `RES_ERROR`, `RES_NOTRDY`, `RES_PARERR`)
 */
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

/**
 * @brief Write data to the disk
 * 
 * @param pdrv Physical drive number to identify the drive
 * @param buff Pointer to the data buffer containing the data to write
 * @param sector Start sector number to write to
 * @param count Number of sectors to write
 * @return The result of the write operation (`RES_OK`, `RES_ERROR`, `RES_NOTRDY`, `RES_PARERR`)
 */
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

/**
 * @brief Perform disk I/O control operation
 * 
 * @param pdrv Physical drive number to identify the drive
 * @param cmd Control command to execute
 * @param buff Pointer to the command-specific buffer
 * @return The result of the I/O control operation (`RES_OK`, `RES_ERROR`, `RES_NOTRDY`, `RES_PARERR`)
 */
DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void *buff) {
	DRESULT rc = RES_PARERR;

	switch (pdrv) {
		case DEV_SDA:
			rc = handle_fat32_ioctl(pdrv, cmd, buff);
			return rc;

		case DEV_MMC :
			// Process of the command for the MMC/SD card
			return rc;

		case DEV_USB :
			// Process of the command the USB drive
			return rc;
	}

	return rc;
}

