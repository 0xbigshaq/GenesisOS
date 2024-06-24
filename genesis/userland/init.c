#include "userland/libs/std.h"

void *memcpy(void *dest, const void *src, unsigned int n)
{
    for (unsigned int i = 0; i < n; i++)
    {
        ((char*)dest)[i] = ((char*)src)[i];
    }
    return (void*)NULL;
}

void _start(void) {
    char banner[] = "Welcome to init!\n";
    char prompt[] = "[root@os]~# ";
    char resp[] = "\nYou typed: ";
    char input[0x20];
    int count;
    write(1, banner, sizeof(banner));

    while(1) {
        write(1, prompt, sizeof(prompt));
        count = read(0, input, sizeof(input));
        write(1, resp, sizeof(resp));
        write(1, input, count);
        write(1, "\n", 1);
    }

     // spin
        asm volatile(
        "_hello: \n" 
        "nop\n" 
        "nop\n" 
        "jmp _hello\n"
    );
}