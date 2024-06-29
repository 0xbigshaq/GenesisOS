#include "userland/libs/std.h"
#include "kernel/syscall.h"
#include <stdint.h>
#include <unistd.h>

int write(int fd, char *buf, uint32_t count) {
    int sysno = SYS_write;
    int result;
    asm volatile(
        "mov eax, %1\n" 
        "push %2\n" 
        "push %3\n" 
        "push %4\n" 
        "int 0x80\n"
        "add esp, 0xC\n"
        : "=a" (result)
        : "a" (sysno),
          "b" (count),
          "c" (buf),
          "d" (fd)
    );
    return result;
}

int read(int fd, char *buf, uint32_t count) {
    int sysno = SYS_read;
    int result;
    asm volatile(
        "mov eax, %1\n" 
        "push %2\n" 
        "push %3\n" 
        "push %4\n" 
        "int 0x80\n"
        "add esp, 0xC\n"
        : "=a" (result)
        : "a" (sysno),
          "b" (count),
          "c" (buf),
          "d" (fd)
    );
    return result;
}

int open(char *pathname, uint32_t flags) {
    int sysno = SYS_open;
    int result;
    asm volatile(
        "mov eax, %1\n" 
        "push %2\n" 
        "push %3\n" 
        "int 0x80\n"
        "add esp, 0x8\n"
        : "=a" (result)
        : "a" (sysno),
          "b" (flags),
          "c" (pathname)
    );
    return result;
}

int close(int fd) {
    int sysno = SYS_close;
    int result;

    asm volatile(
        "mov eax, %1\n" 
        "push %2\n"
        "int 0x80\n"
        "add esp, 0x4\n"
        : "=a" (result)
        : "a" (sysno), "b" (fd)
    );
    
    return result;
}

void exit(int rc) {
    int sysno = SYS_exit;
    int result;

    asm volatile(
        "mov eax, %1\n" 
        "push %2\n"
        "int 0x80\n"
        "add esp, 0x4\n"
        : "=a" (result)
        : "a" (sysno), "b" (rc)
    );

}

void *memcpy(void *dest, const void *src, unsigned int n)
{
    for (unsigned int i = 0; i < n; i++)
    {
        ((char*)dest)[i] = ((char*)src)[i];
    }
    return (void*)NULL;
}