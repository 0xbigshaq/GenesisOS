#include "libs/std.h"

void _start(void) {
    char out[] = "Hello from C :^)\nEnter your name: ";
    char welcome[] = "\nWelcome, ";
    char input[0x20];
    int count;
    write(1, out, sizeof(out));
    count = read(0, input, 0x8);
     write(1, welcome, sizeof(welcome));
     write(1, input, count);

     // spin
        asm volatile(
        "_hello: \n" 
        "nop\n" 
        "nop\n" 
        "jmp _hello\n"
    );
}