#pragma once
#include "drivers/ff15/ff.h"
#include "drivers/ff15/diskio.h"

#define INIT_ELF "INIT"
#define MAX_INIT_SIZE 0x10000

void mount_fs(void);
void load_init(void);
DRESULT handle_fat32_ioctl(BYTE pdrv, BYTE cmd, void *buff);

extern FATFS os_fs;
extern FIL init_fp;
extern uint8_t *init_data;