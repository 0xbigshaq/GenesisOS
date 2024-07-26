/**
 * @file syscall.h
 * @brief System call definitions.
 */
#ifndef SYSCALL_H
#define SYSCALL_H

#include "kernel/types.h" // IWYU pragma: keep

#define SYS_read    0
#define SYS_write   1
#define SYS_open    2
#define SYS_close   3
#define SYS_stat    4
#define SYS_exit    5

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
int sys_exit(void);

#endif // SYSCALL_H