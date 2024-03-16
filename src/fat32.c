#include "fat32.h"
#include "types.h"
#include "console.h"
#include "ata.h"

/*
 * 
 * Number of FATs: 2
 * Sectors per FAT (32-bit): 16
 * Bytes per sector: 512
 *
*/

void list_root(struct FAT32BPB *bpb) {
    uint32_t data_offset = FAT32_DATA(bpb);
    uint32_t tbl_offset = FAT32_TBL(bpb);

    uint32_t data_sector = data_offset / bpb->bytesPerSector;
    uint32_t tbl_sector = tbl_offset / bpb->bytesPerSector;
    
    kprintf("data offset: 0x%x\n", data_offset);
    kprintf("data sector: 0x%x\n", data_sector);
    kprintf("tbl offset: 0x%x\n", tbl_offset);
    kprintf("tbl sector: 0x%x\n", tbl_sector);


    uint8_t buf[512];
    struct msdos_dir_entry* entry;
    ata_read_sector(data_sector, buf);
    entry = (struct msdos_dir_entry*)buf;
    for(int idx = 0 ; idx < (8*2) ; idx++) {
        if(entry->attr & 0x20) {
            kprintf("[idx=%d] %s \t %d \t 0x%x \t 0x%x\n", idx, entry->name, entry->size, ENTRY_SECTOR(entry), entry->start);
            // to get the file contents:
            uint32_t content_sector = data_sector + (ENTRY_SECTOR(entry) * bpb->sectorsPerCluster) - bpb->numFATs;
            kprintf("content_sector = 0x%x, offset=0x%p\n", content_sector, content_sector*bpb->bytesPerSector);
        }
        entry++;
    }

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
