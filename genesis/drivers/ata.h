/**
 * @file ata.h
 * @brief ATA driver for disk I/O.
 * @details This file contains the necessary functions and definitions for the ATA driver.
 */
#ifndef _ATA_H_
#define _ATA_H_

#include "kernel/string.h"
#include "kernel/x86.h" // IWYU pragma: keep

#define ATA_DATA_REG 0x1F0              //!< Data register
#define ATA_ERROR_REG 0x1F1             //!< Error register
#define ATA_SECTOR_COUNT_REG 0x1F2      //!< Sector count register
#define ATA_LBA_LOW_REG 0x1F3           //!< LBA low register
#define ATA_LBA_MID_REG 0x1F4           //!< LBA mid register
#define ATA_LBA_HIGH_REG 0x1F5          //!< LBA high register
#define ATA_DEVICE_REG 0x1F6            //!< Device register
#define ATA_STATUS_REG 0x1F7            //!< Status register
#define ATA_COMMAND_REG 0x1F7           //!< Command register

#define ATA_STATUS_BSY 0x80             //!< Busy status
#define ATA_STATUS_DRDY 0x40            //!< Ready status
#define ATA_STATUS_DF 0x20              //!< Device fault status
#define ATA_STATUS_DRQ 0x08             //!< Data request status
#define ATA_STATUS_ERR 0x01             //!< Error status

#define ATA_CMD_READ_SECTORS 0x20       //!< Read sectors command
#define ATA_CMD_WRITE_SECTORS 0x30      //!< Write sectors command

void delay();
int ata_wait_ready();

// glue functions for ff15
#include "drivers/ff15/diskio.h"
DRESULT ata_disk_read(BYTE *buff, LBA_t sector, UINT count);
DRESULT ata_disk_write(const BYTE *buff, LBA_t sector, UINT count);

#endif // _ATA_H_