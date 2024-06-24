#pragma once
#include "kernel/types.h"

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
void putc(uint8_t x, uint8_t y, enum color fg, enum color bg, char c);
void puts(uint8_t x, uint8_t y, enum color fg, enum color bg, const char *s);
void clear(enum color bg);
void console_putchar(char c);
int console_getchar(void);


// Serial
void printint(int xx, int base, int sign);
void kprintf(char *fmt, ...);