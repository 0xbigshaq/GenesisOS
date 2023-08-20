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


#pragma pack(1)

struct FAT32BPB {
    // BPB (BIOS Parameter Block)
    uint8_t jump[3];
    uint8_t oemName[8];
    uint16_t bytesPerSector;
    uint8_t sectorsPerCluster;
    uint16_t reservedSectors;
    uint8_t numFATs;
    uint16_t rootDirEntries;
    uint16_t totalSectors16;
    uint8_t mediaDescriptor;
    uint16_t sectorsPerFAT16;
    uint16_t sectorsPerTrack;
    uint16_t numHeads;
    uint32_t hiddenSectors;
    uint32_t totalSectors32;

    // Extended Boot Record
    uint32_t sectorsPerFAT32;
    uint16_t flags;
    uint16_t version;
    uint32_t rootCluster;
    uint16_t FSInfoSector;
    uint16_t backupBootSector;
    uint8_t reserved[12];
    uint8_t driveNumber;
    uint8_t reserved1;
    uint8_t bootSignature;
    uint32_t volumeID;
    uint8_t volumeLabel[11];
    uint8_t fileSystemType[8];
    uint8_t bootCode[420];
    uint16_t bootSignature2;
}  __attribute__((packed))  ;

#define MSDOS_NAME 11 /* maximum name length */
struct msdos_dir_entry {
        uint8_t    name[MSDOS_NAME];/* name and extension */
        uint8_t    attr;           /* attribute bits */
        uint8_t    lcase;          /* Case for base and extension */
        uint8_t    ctime_cs;       /* Creation time, centiseconds (0-199) */
        uint16_t   ctime;          /* Creation time */
        uint16_t   cdate;          /* Creation date */
        uint16_t   adate;          /* Last access date */
        uint16_t   starthi;        /* High 16 bits of cluster in FAT32 */
        uint16_t   time,date,start;/* time, date and first cluster */
        uint32_t   size;           /* file size (in bytes) */
} __attribute__((packed));

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

void list_root(struct FAT32BPB *bpb) {
    uint32_t data_offset = bpb->bytesPerSector * (bpb->reservedSectors + bpb->numFATs * bpb->sectorsPerFAT32);
    uint32_t data_sector = data_offset / bpb->bytesPerSector;
    kprintf("data offset: 0x%x\n", data_offset);
    kprintf("data sector: 0x%x\n", data_sector);


    uint8_t buf[512];
    struct msdos_dir_entry* entry;
    ata_read_sector(data_sector, buf);
    entry = (struct msdos_dir_entry*)buf;
    if(entry->attr & 0x20) kprintf("%s \t %d\n", entry->name, entry->size);
    entry++;
    if(entry->attr & 0x20) kprintf("%s \t %d\n", entry->name, entry->size);
    entry++;
    if(entry->attr & 0x20) kprintf("%s \t %d\n", entry->name, entry->size);
    entry++;
    if(entry->attr & 0x20) kprintf("%s \t %d\n", entry->name, entry->size);
    // xx 
}

void dump_fat32_header(struct FAT32BPB *bpb) {
    kprintf("\n----- fat32 header dump -----\n");
    kprintf("OEM Name: %s\n", bpb->oemName);
    kprintf("Bytes per sector: %d\n", bpb->bytesPerSector);
    kprintf("Sectors per cluster: %d\n", bpb->sectorsPerCluster);
    kprintf("Reserved sectors: %d\n", bpb->reservedSectors);
    kprintf("Number of FATs: %d\n", bpb->numFATs);
    kprintf("Root directory entries: %d\n", bpb->rootDirEntries);
    kprintf("Total sectors (16-bit): %d\n", bpb->totalSectors16);
    kprintf("Media descriptor: 0x%x\n", bpb->mediaDescriptor);
    kprintf("Sectors per FAT (16-bit): %d\n", bpb->sectorsPerFAT16);
    kprintf("Sectors per track: %d\n", bpb->sectorsPerTrack);
    kprintf("Number of heads: %d\n", bpb->numHeads);
    kprintf("Hidden sectors: %d\n", bpb->hiddenSectors);
    kprintf("Total sectors (32-bit): %d\n", bpb->totalSectors32);
    kprintf("Sectors per FAT (32-bit): %d\n", bpb->sectorsPerFAT32);
    kprintf("Flags: 0x%x\n", bpb->flags);
    kprintf("Version: 0x%x\n", bpb->version);
    kprintf("Root cluster: %d\n", bpb->rootCluster);
    kprintf("FSInfo sector: %d\n", bpb->FSInfoSector);
    kprintf("Backup boot sector: %d\n", bpb->backupBootSector);
    kprintf("Reserved: ");
    for (int i = 0; i < 12; i++) {
        kprintf("%x ", bpb->reserved[i]);
    }
    kprintf("\n");
    kprintf("Drive number: 0x%x\n", bpb->driveNumber);
    kprintf("Reserved1: 0x%x\n", bpb->reserved1);
    kprintf("Boot signature: 0x%x\n", bpb->bootSignature);
    kprintf("Volume ID: 0x%x\n", bpb->volumeID);
    kprintf("Volume label: %s\n", bpb->volumeLabel);
    kprintf("Filesystem type: %s\n", bpb->fileSystemType);
    kprintf("Boot code: ");
    for (int i = 0; i < 420; i++) {
        kprintf("%x ", bpb->bootCode[i]);
    }
    kprintf("\n");
    kprintf("Boot signature 2: 0x%x\n", bpb->bootSignature2);
    kprintf("\n");

    list_root(bpb);
    kprintf("-----------------------------\n");
}

