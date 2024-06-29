#include "kernel/kmalloc.h"
#include "kernel/string.h"
#include "drivers/fat32.h"
#include "drivers/console.h"

FATFS os_fs;
FIL init_fp;
uint8_t *init_data;

void mount_fs(void) {
    FRESULT rc;

    rc = f_mount(&os_fs, "0:", 1);
    if(rc != FR_OK) goto fs_error;
    kprintf("[*] Volume `0:` mounted\n");

    return;

    fs_error:
        kprintf("[!] FAT FS Error: %d\n", rc);
        PANIC("Cannot open init file")
}

void load_init(void) {
    FRESULT rc;
    UINT br;
    FILINFO fno;

    rc = f_stat("0:INIT", &fno);
    if(rc != FR_OK) goto fs_error;    
    kprintf("[*] init binary was found, size: %d\n", fno.fsize);
    init_data = malloc(fno.fsize);

    rc = f_open(&init_fp, "0:INIT", FA_READ);
    if(rc != FR_OK) goto fs_error;

    rc = f_read(&init_fp, init_data, fno.fsize, &br);
    if(rc != FR_OK) goto fs_error;

    kprintf("[*] %d bytes read\n", br);
    f_close(&init_fp);

    return;

    fs_error:
        kprintf("[!] FAT FS Error: %d\n", rc);
        PANIC("Cannot open init file")
}

DRESULT handle_fat32_ioctl(BYTE pdrv, BYTE cmd, void *buff) {
    DRESULT rc = RES_PARERR;
    
    switch(cmd) {
    /* Generic command (Used by FatFs) */
        case CTRL_SYNC:
            PANIC("[!] fat32 ioctl CTRL_SYNC not implemented");
            break;
        case GET_SECTOR_COUNT:
            PANIC("[!] fat32 ioctl GET_SECTOR_COUNT not implemented");
            break;
        case GET_SECTOR_SIZE:
            PANIC("[!] fat32 ioctl GET_SECTOR_SIZE not implemented");
            break;
        case GET_BLOCK_SIZE:
            PANIC("[!] fat32 ioctl GET_BLOCK_SIZE not implemented");
            break;
        case CTRL_TRIM:
            PANIC("[!] fat32 ioctl CTRL_TRIM not implemented");
            break;
    
    /* ATA/CF specific ioctl command */
        case ATA_GET_REV:
            PANIC("[!] ATA ioctl ATA_GET_REV not implemented");
            break;
        case ATA_GET_MODEL:
            PANIC("[!] ATA ioctl ATA_GET_MODEL not implemented");
            break;
        case ATA_GET_SN:
            PANIC("[!] ATA ioctl ATA_GET_SN not implemented");
            break;
        
        default:
            rc = RES_OK;
            break;
    }

    return rc;
}
