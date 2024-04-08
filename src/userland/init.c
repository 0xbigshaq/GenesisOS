#include "libs/std.h"

void _start(void) {
    write(1, "Hello from C :^)");

        asm volatile(
        "_hello: \n" 
        "nop\n" 
        "nop\n" 
        "jmp _hello\n"
    );
}