#pragma once
#include "kernel/types.h" // IWYU pragma: keep

// Video Memory
// https://wiki.osdev.org/Printing_To_Screen#Color_Table
enum color {
    BLACK = 0,
    BRIGHT = 7,
    RED = 12,
    GREEN = 10,
};

enum size {
    COLS = 80,
    ROWS = 25
};

extern uint16_t *const video;

void init_console();
void console_putchar(char c);
int console_getchar(void);
int console_read(uint8_t *buf, uint32_t count);
int console_write(uint8_t *buf, uint32_t count);

// Serial
void printint(int xx, int base, int sign);
void kprintf(char *fmt, ...);