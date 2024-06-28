#pragma once
#include "drivers/ff15/ff.h"

#define INIT_ELF "INIT"
#define MAX_INIT_SIZE 0x10000

void mount_fs(void);
void load_init(void);

extern FATFS os_fs;
extern FIL init_fp;
extern uint8_t init_data[MAX_INIT_SIZE];