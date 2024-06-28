#include "ff.h"
#include "kernel/kmalloc.h"
#include "kernel/string.h"
#include "drivers/fat32.h"
#include "drivers/console.h"

FATFS os_fs;
FIL init_fp;
uint8_t init_data[MAX_INIT_SIZE];

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

    rc = f_open(&init_fp, "0:INIT", FA_READ);
    if(rc != FR_OK) goto fs_error;

    rc = f_read(&init_fp, init_data, sizeof(init_data), &br);
    if(rc != FR_OK) goto fs_error;

    kprintf("[*] %d bytes read\n", br);
    f_close(&init_fp);

    return;

    fs_error:
        kprintf("[!] FAT FS Error: %d\n", rc);
        PANIC("Cannot open init file")
}