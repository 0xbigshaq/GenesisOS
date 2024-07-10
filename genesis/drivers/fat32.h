#ifndef FAT32_H
#define FAT32_H

#include "drivers/ff15/ff.h"
#include "drivers/ff15/diskio.h"
#include <stddef.h>

#define INIT_ELF "INIT"
#define MAX_INIT_SIZE 0x10000

void mount_fs(void);
void load_init(void);
DRESULT handle_fat32_ioctl(BYTE pdrv, BYTE cmd, void *buff);

typedef FIL FILE;
#define SEEK_SET	0	/* Seek from beginning of file.  */
#define SEEK_CUR	1	/* Seek from current position.  */
#define SEEK_END	2	/* Seek from end of file.  */

FILE *fopen(const char *path, const char *mode);
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
int fclose(FILE *stream);
size_t ftell(FILE *stream);

extern FATFS os_fs;
extern FIL init_fp;
extern uint8_t *init_data;

#endif // FAT32_H