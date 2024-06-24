#include "kernel/syscall.h"
#include "kernel/proc.h"
#include "kernel/types.h"
#include "kernel/file.h"
#include "drivers/console.h"
#include "drivers/uart.h"

int (*syscalls[])(void) = {
    [SYS_read]  = sys_read,
    [SYS_write] = sys_write,
    [SYS_open]  = sys_open,
    [SYS_close] = sys_close,
    [SYS_stat]  = sys_stat
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
    ctx->eax = syscalls[ctx->eax](); // FIXME: add bounds-check lol
}


int sys_read(void) {
    uint32_t fd = arg_word(0);
    char* out = (char*)arg_ptr(1);
    uint32_t count = arg_word(2);
    char c = 0;
    int i = 0;
    task_t *proc = cur_proc();

    if(proc->ofile[fd].type == FD_DEVICE) {
        for(; i<count; ) {
            c = devices[proc->ofile[fd].devno].read();
            if (c == '\n' || c == '\r') {
                break;
            }
            else if(c == 0x7f && i>0) {
                i--;
            } else if( c != 0x7f) {
                out[i] = c;
                i++;
            } else {
                continue;
            }
        }
        out[i] = 0;
    }
    return i;
}


int sys_write(void) {
    uint32_t fd = arg_word(0);
    char *buf = (char*)arg_ptr(1);
    uint32_t count = arg_word(2);
    task_t *proc = cur_proc();

    if(proc->ofile[fd].type == FD_DEVICE) {
        for(int i=0; i<count; i++) {
            devices[proc->ofile[fd].devno].write(buf[i]);
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

// TODO:
// https://manpages.debian.org/unstable/manpages-dev/getdents.2.en.html