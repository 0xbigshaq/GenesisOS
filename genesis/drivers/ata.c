#include "drivers/ata.h"


void delay() {
    for (int i = 0; i < 1000; i++) {
        __asm__("nop");
    }
}

int ata_wait_ready() {
    while (inb(ATA_STATUS_REG) & ATA_STATUS_BSY);
    return (inb(ATA_STATUS_REG) & ATA_STATUS_DRDY);
}

DRESULT ata_disk_read(BYTE *buff, LBA_t sector, UINT count) {
        // Wait for the drive to be ready
    if (!ata_wait_ready()) return RES_NOTRDY;

    for (UINT i = 0; i < count; i++) {
        // Set up the registers for the read operation
        outb(ATA_SECTOR_COUNT_REG, 1);
        outb(ATA_LBA_LOW_REG, (sector & 0xFF));
        outb(ATA_LBA_MID_REG, ((sector >> 8) & 0xFF));
        outb(ATA_LBA_HIGH_REG, ((sector >> 16) & 0xFF));
        outb(ATA_DEVICE_REG, 0xE0 | ((sector >> 24) & 0x0F));
        outb(ATA_COMMAND_REG, ATA_CMD_READ_SECTORS);

        // Wait for the drive to be ready to transfer data
        if (!ata_wait_ready()) return RES_NOTRDY;

        // Read data from the data register
        for (int j = 0; j < 256; j++) {
            ((WORD*)buff)[j] = inw(ATA_DATA_REG);
        }

        buff += 512;
        sector++;
    }

    return RES_OK;
}


DRESULT ata_disk_write(const BYTE *buff, LBA_t sector, UINT count) {
    // Wait for the drive to be ready
    if (!ata_wait_ready()) return RES_NOTRDY;

    for (UINT i = 0; i < count; i++) {
        // Set up the registers for the write operation
        outb(ATA_SECTOR_COUNT_REG, 1);
        outb(ATA_LBA_LOW_REG, (sector & 0xFF));
        outb(ATA_LBA_MID_REG, ((sector >> 8) & 0xFF));
        outb(ATA_LBA_HIGH_REG, ((sector >> 16) & 0xFF));
        outb(ATA_DEVICE_REG, 0xE0 | ((sector >> 24) & 0x0F));
        outb(ATA_COMMAND_REG, ATA_CMD_WRITE_SECTORS);

        // Wait for the drive to be ready to accept data
        if (!ata_wait_ready()) return RES_NOTRDY;

        // Write data to the data register
        for (int j = 0; j < 256; j++) {
            outw(ATA_DATA_REG, ((WORD*)buff)[j]);
        }

        buff += 512;
        sector++;
    }

    return RES_OK;
}