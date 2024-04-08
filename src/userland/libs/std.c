#include "std.h"
#include "syscall.h"

long write(int fd, char *buf) {
    int sysno = SYS_write ;//SYS_write;
    long result;
    asm volatile(
        "mov eax, %1\n" 
        "push %2\n" 
        "push %3\n" 
        "int 0x80\n"
        "add esp, 0x8\n"
        : "=a" (result)
        : "a" (sysno),
          "b" (buf),
          "c" (fd)
    );
    return result;  // Return value is already in %eax
}
