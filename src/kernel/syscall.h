#pragma once
#include "kernel/types.h"

#define SYS_read    0
#define SYS_write   1
#define SYS_open    2
#define SYS_close   3
#define SYS_stat    4

// helpers/utils
void sys_dispatch(void);
uint32_t arg_word(int n);
void* arg_ptr(int n);

// syscalls
int sys_read(void);
int sys_write(void);
int sys_open(void);
int sys_close(void);
int sys_stat(void);