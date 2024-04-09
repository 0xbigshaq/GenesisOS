#include "libs/std.h"

void _start(void) {
    char out[] = "Hello from C :^)\n";
    write(1, out, sizeof(out));

        asm volatile(
        "_hello: \n" 
        "nop\n" 
        "nop\n" 
        "jmp _hello\n"
    );
}