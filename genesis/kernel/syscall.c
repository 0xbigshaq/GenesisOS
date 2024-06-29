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
    [SYS_stat]  = sys_stat,
    [SYS_exit]  = sys_exit,
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

    if(fd >= NOFILE) {
        return -EBADF;
    }

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

    if(fd >= NOFILE) {
        return -EBADF;
    }

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
    int fd_num = alloc_fd(proc); // TODO: add call to `stat()`
    int rc = OK;
    if(!fd_num) {
        kprintf("cannot allocate fd, fd_num = %d\n", fd_num);
        return -1;
    }
    file_descriptor_t *fd = malloc(sizeof(file_descriptor_t));

    rc = f_open(fd, pathname, flags);
    if(rc != FR_OK) {
        rc = -EPERM;
    } else {
        proc->ofile[fd_num].fd = fd;
        proc->ofile[fd_num].type = FD_FILE;
        rc = fd_num;
    }

    return rc;
}


int sys_close(void) {
    int rc = OK;
    uint32_t fd = arg_word(0);
    task_t *proc = cur_proc();

    if(fd >= NOFILE) {
        return -EBADF;
    }

    if(proc->ofile[fd].type == FD_FILE && proc->ofile[fd].refcount) {
        f_close(proc->ofile[fd].fd);
        free(proc->ofile[fd].fd);
        proc->ofile[fd].fd = NULL;
        proc->ofile[fd].refcount = 0;
        rc = OK;
    }

    return rc;
}


int sys_stat(void) {
    // not implemented yet
    return 0;
}

int sys_exit(void) {
    uint32_t ret_code = arg_word(0);
    task_t *proc = cur_proc();
    kprintf("SYS_exit was called, ret_code = %d\n", ret_code);

    if(proc->pid == 1) {
        kprintf("[*] pid 1 exited, shutting down...\n");
        outw(0x604, 0x2000); // Shutdown, https://wiki.osdev.org/Shutdown
    }
    while(1) { /* ... spin forever, todo: implement shutting down a process ... */ }
    
    return OK;
}

// TODO:
// https://manpages.debian.org/unstable/manpages-dev/getdents.2.en.html