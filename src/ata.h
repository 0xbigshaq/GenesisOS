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

void delay() {
    for (int i = 0; i < 1000; i++) {
        __asm__("nop");
    }
}

void ata_wait_ready() {
  // Wait for disk ready.
  while((inb(0x1F7) & 0xC0) != 0x40)
    ;
}

int ata_read_sector(uint32_t lba, uint8_t *buffer) {
    outb(ATA_SECTOR_COUNT_REG, 1);
    // Set block
    outb(ATA_LBA_LOW_REG, lba & 0xFF);
    outb(ATA_LBA_MID_REG, (lba >> 8) & 0xFF);
    outb(ATA_LBA_HIGH_REG, (lba >> 16) & 0xFF);
    outb(ATA_DEVICE_REG, 0xE0 | ((lba >> 24) & 0x0F));

    // launch command
    outb(ATA_COMMAND_REG, ATA_CMD_READ_SECTORS);

    ata_wait_ready();

    for (int i = 0; i < 256; i++) {
        uint16_t data = inw(ATA_DATA_REG);
        memmove(buffer + (i * 2), &data, sizeof(uint16_t));
    }

    kprintf("Read sector %d successfully\n", lba);  // Add this line for debugging


    return 0;
}
