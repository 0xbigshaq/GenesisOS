#include "console.h"
#include "memlayout.h"

uint16_t *const video = (uint16_t*) phys_to_virt(0xB8000);

void putc(uint8_t x, uint8_t y, enum color fg, enum color bg, char c) {
    video[y * COLS + x] = (bg << 12) | (fg << 8) | c;
}

void puts(uint8_t x, uint8_t y, enum color fg, enum color bg, const char *s) {
    while(*s != 0) {
        putc(x, y, fg, bg, *s);
        s++; x++;
    }
}

void clear(enum color bg) {
    uint8_t x, y;
    for (y = 0; y < ROWS; y++)
        for (x = 0; x < COLS; x++)
            putc(x, y, bg, bg, ' ');
}
