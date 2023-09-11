#pragma once 

#include "string.h"
#include "x86.h"

#define ATA_DATA_REG 0x1F0
#define ATA_ERROR_REG 0x1F1
#define ATA_SECTOR_COUNT_REG 0x1F2
#define ATA_LBA_LOW_REG 0x1F3
#define ATA_LBA_MID_REG 0x1F4
#define ATA_LBA_HIGH_REG 0x1F5
#define ATA_DEVICE_REG 0x1F6
#define ATA_STATUS_REG 0x1F7
#define ATA_COMMAND_REG 0x1F7

#define ATA_STATUS_BSY 0x80
#define ATA_STATUS_DRDY 0x40
#define ATA_STATUS_DF 0x20
#define ATA_STATUS_DRQ 0x08
#define ATA_STATUS_ERR 0x01

#define ATA_CMD_READ_SECTORS 0x20


void delay();
void ata_wait_ready();
int ata_read_sector(uint32_t lba, uint8_t *buffer);

