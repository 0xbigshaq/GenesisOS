#include "ata.h"
#include "console.h"

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

    // kprintf("Read sector %d successfully\n", lba);  // Add this line for debugging


    return 0;
}