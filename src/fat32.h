#pragma once
#include "types.h"

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

void dump_fat32_header(struct FAT32BPB *bpb);
void list_root(struct FAT32BPB *bpb);