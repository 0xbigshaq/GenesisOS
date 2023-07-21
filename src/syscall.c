#include "syscall.h"
#include "console.h"
#include "proc.h"
#include "types.h"

int (*syscalls[])(void) = {
    [SYS_read]  sys_read,
    [SYS_write] sys_write,
    [SYS_open]  sys_open,
    [SYS_close] sys_close,
    [SYS_stat]  sys_stat
};

uint32_t arg_word(int n) {
    uint32_t* esp = (uint32_t*)cur_proc()->trapframe->esp;
    uint32_t val = 0;
    val = *(esp+n);
    return val;
}

void sys_dispatch(void) {
    trap_ctx_t* ctx = cur_proc()->trapframe;
    syscalls[ctx->eax]();
}


int sys_read(void) {
    // not implemented yet
    return 0;
}


int sys_write(void) {
    // uint32_t fd = arg_word(0);
    char* str = (char*)arg_word(1);
    kprintf("[*] SYS_write called!, value: \"%s\"\n",str);
    return 0;
}


int sys_open(void) {
    // not implemented yet
    return 0;
}


int sys_close(void) {
    // not implemented yet
    return 0;
}


int sys_stat(void) {
    // not implemented yet
    return 0;
}