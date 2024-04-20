#include "drivers/fat32.h"
#include "kernel/kmalloc.h"
#include "kernel/mmu.h"
#include "kernel/types.h"
#include "drivers/console.h"
#include "drivers/ata.h"
#include "kernel/string.h"

/*
 * 
 * Number of FATs: 2
 * Sectors per FAT (32-bit): 16
 * Bytes per sector: 512
 *
*/

fat_region_t file_tbl;
struct FAT32BPB bios_param_block;
struct msdos_dir_entry init_file;
uint8_t init_data[MAX_INIT_SIZE];

void list_root(struct FAT32BPB *bpb, uint32_t tbl_sector, uint32_t data_sector) {
    // listing the root directory, located in the beginning of the `Data` sector.
    uint8_t buf[512];
    struct msdos_dir_entry *entry;
    uint32_t content_sector, next_sector;
    ata_read_sector(data_sector, buf);
    entry = (struct msdos_dir_entry*)buf;
    for(int idx = 0 ; idx < (8*2) ; idx++) {
        if(entry->attr & 0x20) {
            // kprintf("[idx=%d] %s \t %d \t 0x%x \t\n", idx, entry->name, entry->size, ENTRY_SECTOR(entry));
            content_sector = ENTRY_SECTOR(entry);
            next_sector = file_tbl.info.raw.entry[content_sector];
            // kprintf("%s \t %d \t 0x%x \t 0x%x\n", entry->name, entry->size, content_sector, next_sector);
            populate_init(entry);
        }
        entry++;
    }

    next_sector = file_tbl.info.meta.entry[0];
    kprintf("root directory next_sector=0x%p , eoc=0x%x, fat_id=0x%x\n", next_sector, file_tbl.info.meta.eoc, file_tbl.info.meta.fat_id);

    if(next_sector != 0xffffff8) { // end of cluster (for directories, mkdosfs/Linux)
        follow_dir_chain(bpb, data_sector, next_sector);
    }
}

void follow_dir_chain(struct FAT32BPB *bpb, uint32_t data_sector, uint32_t cur_sector) {
    // kprintf("follow_dir_chain called! data_sector = 0x%x , cur_sector = 0x%x \n", data_sector, cur_sector);
    uint8_t buf[512];
    struct msdos_dir_entry *entry;
    // uint32_t content_sector, next_sector;

    ata_read_sector((data_sector+cur_sector-2), buf); // we subtract 2 bc the first two values in the file_tbl are metadata.
    entry = (struct msdos_dir_entry*)buf;
    for(int idx = 0 ; idx < (8*2) ; idx++) {
        if(entry->attr & 0x20) {
            // kprintf("[idx=%d] %s \t %d \t 0x%x \t\n", idx, entry->name, entry->size, ENTRY_SECTOR(entry));
            // content_sector = ENTRY_SECTOR(entry);
            // next_sector = file_tbl.info.raw.entry[content_sector];
            // kprintf("%s \t %d \t 0x%x \t 0x%x\n", entry->name, entry->size, content_sector, next_sector);
            populate_init(entry);
        }
        entry++;
    }

    if(file_tbl.info.meta.eoc != file_tbl.info.raw.entry[cur_sector]) {
        // end of chain detected, enter recursion.
        follow_dir_chain(bpb, data_sector, file_tbl.info.raw.entry[cur_sector]);
    }
}

void follow_file_chain(struct FAT32BPB *bpb, uint32_t data_sector, uint32_t cur_sector) {
    kprintf("follow_file_chain called! data_sector = 0x%x , cur_sector = 0x%x \n", data_sector, cur_sector);
    uint8_t buf[512];

    ata_read_sector((data_sector+cur_sector-2), buf); // we subtract 2 bc the first two values in the file_tbl are metadata.
    if(file_tbl.info.meta.eoc != file_tbl.info.raw.entry[cur_sector]) {
        // end of chain detected, enter recursion.
        follow_file_chain(bpb, data_sector, file_tbl.info.raw.entry[cur_sector]);
    }
}

void dump_fat32_header(struct FAT32BPB *bpb) {
    kprintf("[*] Parsing FAT32 Header Dump\n");
    /*
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
    */

    // more data
    uint32_t data_offset = FAT32_DATA(bpb);
    uint32_t tbl_offset = FAT32_TBL(bpb);

    uint32_t data_sector = data_offset / bpb->bytesPerSector;
    uint32_t tbl_sector = tbl_offset / bpb->bytesPerSector;
    uint32_t tbl_size_bytes = bpb->bytesPerSector * bpb->sectorsPerFAT32;
    uint32_t tbl_entries = tbl_size_bytes / 4; /* 4 bytes, 32bit per entry. */

    kprintf(" >data offset: 0x%x\n", data_offset);
    kprintf(" >data sector: 0x%x\n", data_sector);
    kprintf(" >tbl offset: 0x%x\n", tbl_offset);
    kprintf(" >tbl sector: 0x%x\n", tbl_sector);
    kprintf(" >tbl entries: 0x%x\n", tbl_entries);

    init_file_table(bpb, tbl_sector); // Copy the whole FAT table from disk to .bss/.data(`file_tbl` global), for later use. 
    list_root(bpb, tbl_sector, data_sector);   // list root 
    kprintf("-----------------------------\n");
}

void init_file_table(struct FAT32BPB *bpb, uint32_t tbl_sector) {
    uint8_t* cursor = (uint8_t*)&file_tbl; // `file_tbl` is a global
    uint32_t sector_idx = tbl_sector;
    for(int i = 0; i < bpb->sectorsPerFAT32; i++) {
        // kprintf("reading FAT tbl idx: %d\n", i);
        ata_read_sector(sector_idx, cursor);
        sector_idx++;
        cursor += bpb->bytesPerSector;
    }
}

void populate_init(struct msdos_dir_entry *entry) {
    if(strncmp((char*)entry->name, INIT_ELF, strlen(INIT_ELF))) {
        // kprintf("[!] '%s' != '%s'\n", INIT_ELF, entry->name);
        return ;
    }
    memcpy(&init_file, entry, sizeof(init_file));
}

void dump_file(void) {
    if(strncmp((char*)init_file.name, INIT_ELF, strlen(INIT_ELF))) {
        kprintf("[!] Error: init file cannot be found\n");
        return ;
    }

    kprintf("[+] Dumping init ELF file now....\n");
    struct msdos_dir_entry *init_ptr = &init_file;
    uint32_t content_sector, next_sector;
    struct FAT32BPB* bpb = &bios_param_block;
    uint32_t data_offset, data_sector, dst_sector;
    uint8_t *cursor;

    data_offset = FAT32_DATA(bpb);
    data_sector = data_offset / bpb->bytesPerSector;

    content_sector = ENTRY_SECTOR(init_ptr);
    next_sector = file_tbl.info.raw.entry[content_sector];

    dst_sector = (data_sector+content_sector-2);
    cursor = init_data;
    ata_read_sector(dst_sector, cursor);
    cursor += bpb->bytesPerSector;

    kprintf("[*] size = 0x%x \n", init_ptr->size);
    kprintf("[*] sector = 0x%x \n", content_sector);
    kprintf("[*] next sector = 0x%x \n", next_sector);
    // kprintf("[*] content = %s \n", init_data);

    while(1) {
        if (next_sector == file_tbl.info.meta.eoc)
            break;
        if (cursor >= &init_data[sizeof(init_data)])
            break;
        
        content_sector = next_sector;
        dst_sector = (data_sector+content_sector-2);

        ata_read_sector(dst_sector, cursor);
        kprintf(" > next sector/cluster = 0x%x \n", next_sector);
        cursor += bpb->bytesPerSector;
        next_sector = file_tbl.info.raw.entry[content_sector];
    };

    kprintf("[+] Finished reading init from disk\n");
}