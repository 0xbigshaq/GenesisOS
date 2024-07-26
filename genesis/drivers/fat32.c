#include "kernel/kmalloc.h"
#include "kernel/string.h"
#include "drivers/fat32.h"
#include "drivers/console.h"

FATFS os_fs;
FIL init_fp;
uint8_t *init_data;

/**
 * @brief   Mount a FAT32 image.
*/
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

/**
 * @brief   Load the INIT binary(=first user process) from the FAT32 image.
 * 
 */
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

/**
 * @brief   Handle ioctl commands for the FAT32 filesystem(Note: not implemented).
 */
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

/**
 * @brief   Open a file.
 * 
 * @param path      The path to the file
 * @param mode      The mode to open the file(Note: It is always read-only for now)
 * @return FILE*    The file pointer
 */
FILE *fopen(const char *path, const char *mode) {
    FRESULT rc;
    FILE *fp = malloc(sizeof(FILE));
    // FIXME: check if null

    rc = f_open(fp, path, FA_READ);
    if(rc != FR_OK) goto fs_error;
    kprintf("[*] File `%s` opened\n", path);

    return fp;

    fs_error:
        kprintf("[!] FAT FS Error: %d\n", rc);
        free(fp);
        PANIC("Cannot open file")

    return -1;
}

/**
 * @brief   Read data from a file.
 * 
 * @param ptr       The buffer to store the read data
 * @param size      size of each element
 * @param nmemb     number of elements
 * @param stream    The file pointer
 * @return size_t   The number of bytes read
 */
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    FRESULT rc;
    FIL *fp = stream;;
    UINT br;
    
    rc = f_read(fp, ptr, size * nmemb, &br);
    if(rc < FR_OK) goto fs_error;
    kprintf("[*] %d bytes read\n", br);

    return br;

    fs_error:
        kprintf("[!] FAT FS Error: %d\n", rc);
        PANIC("Cannot read file")

    return -1;
}

/**
 * @brief   Close a file.
 * 
 * @param stream    The file pointer
 * @return int      0 on success, -1 on failure
 */
int fclose(FILE *stream) {
    FRESULT rc;
    FIL *fp = stream;

    rc = f_close(fp);
    if(rc != FR_OK) goto fs_error;
    kprintf("[*] File closed\n");

    return 0;

    fs_error:
        kprintf("[!] FAT FS Error: %d\n", rc);
        PANIC("Cannot close file")

    return -1;
}

/**
 * @brief   Get the current file position.
 * 
 * @param stream    The file pointer
 * @return size_t   The current file position
 */
size_t ftell(FILE *stream) {
    size_t pos;
    FIL *fp = stream;

    pos = f_tell(fp);
    if(pos < 0) goto fs_error;
    kprintf("[*] File position: %d\n", pos);

    return pos;

    fs_error:
        kprintf("[!] FAT FS Error: %d\n", pos);
        PANIC("Cannot get file position")

    return -1;
}
