#include "kernel/syscall.h"
#include "ff.h"
#include "kernel/proc.h"
#include "kernel/types.h" // IWYU pragma: keep
#include "kernel/file.h"
#include "drivers/console.h" // IWYU pragma: keep
#include "drivers/uart.h" // IWYU pragma: keep
#include "kernel/kmalloc.h" // IWYU pragma: keep
#include <errno.h>

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
    task_t *proc = cur_proc();
    int rc = 0;

    if(proc->ofile[fd].type == FD_DEVICE) {
        rc = devices[proc->ofile[fd].devno].read((uint8_t*)out, count);
    }

    else if(proc->ofile[fd].type == FD_FILE) {
        f_read(proc->ofile[fd].fd, out, count, (UINT*)&rc);
    }
    else {
        rc = -EBADF;
    }

    return rc;
}


int sys_write(void) {
    uint32_t fd = arg_word(0);
    uint8_t *buf = (uint8_t*)arg_ptr(1);
    uint32_t count = arg_word(2);
    task_t *proc = cur_proc();
    int rc = OK;
    UINT bytes_out;

    if(proc->ofile[fd].type == FD_DEVICE) {
        rc = devices[proc->ofile[fd].devno].write(buf, count);
    }

    else if(proc->ofile[fd].type == FD_FILE) {
        rc = f_write(proc->ofile[fd].fd, buf, count, &bytes_out);
        if(rc == FR_OK) {
            rc = bytes_out;
        } else {
            rc = -EIO;
        }
    }

    else {
        // fallback
        rc = -EINVAL;
    }
    return rc;
}


int sys_open(void) {
    char *pathname = (char*)arg_ptr(0);
    uint32_t flags = arg_word(1);
    task_t *proc = cur_proc();
    int newFD = 3; // FIXME: should impl & add a call to `alloc_fd() `
    int rc = OK;
    file_descriptor_t *fd = malloc(sizeof(file_descriptor_t));

    rc = f_open(fd, pathname, flags);
    if(rc != FR_OK) {
        rc = -EPERM;
    } else {
        proc->ofile[newFD].fd = fd;
        proc->ofile[newFD].type = FD_FILE;
        rc = newFD;
    }

    return rc;
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