#include "syscall.h"
#include "console.h"
#include "proc.h"
#include "types.h"
#include "uart.h"

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

void* arg_ptr(int n) {
    uint32_t* esp = (uint32_t*)cur_proc()->trapframe->esp;
    uint32_t val = 0;
    val = *(esp+n);
    return (void*)val;
}

void sys_dispatch(void) {
    trap_ctx_t* ctx = cur_proc()->trapframe;
    ctx->eax = syscalls[ctx->eax](); // FIXME: add bounds-check
}


/*
 * FIXME: dumb impl, should use real fd, not put a nullbyte at the end, 
 * should also stop at EOF(and not a newline)
 */
int sys_read(void) {
    uint32_t fd = arg_word(0);
    char* out = (char*)arg_ptr(1);
    uint32_t count = arg_word(2);
    char c = 0;
    int i = 0;

    kprintf("\n[*] SYS_read called!, fd=%d, buf=0x%p, count=%d\n", fd, out, count);

    if(fd == 0) { // stdin
        for(; i<count; i++) {
            while(c<=0)
                c = uart_getchar();
            uart_putchar(c);
            if (c == '\r' || c == '\n')
                break;
            out[i] = c;
            c = 0;
        }
        out[i] = 0;
    }

    return i;
}


int sys_write(void) {
    uint32_t fd = arg_word(0);
    char *buf = (char*)arg_ptr(1);
    uint32_t count = arg_word(2);
    kprintf("[*] SYS_write called! fd=%d, buf=0x%x, count=%d \n",fd, buf, count);
    for(int i=0; i<count; i++) {
        if(fd == 1) { // stdout
            uart_putchar(buf[i]);
        }
    }
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