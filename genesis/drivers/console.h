/**
 * @file console.h
 * @brief Serial/console output.
 */
#ifndef CONSOLE_H
#define CONSOLE_H
#include "kernel/types.h" // IWYU pragma: keep
#include "kernel/string.h"

void init_console();
int console_getchar(void);
int console_read(uint8_t *buf, uint32_t count);
int console_write(uint8_t *buf, uint32_t count);

// Serial
void printint(int xx, int base, int sign);
void kprintf(char *fmt, ...);

// ANSI escape codes for colors
#define COLOR_RESET   "\x1b[0m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_YELLOW  "\x1b[33m"

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define dmsg(fmt, ...) kprintf("[" COLOR_RED "%s:%d " COLOR_YELLOW "%s " COLOR_RESET "] :: " fmt "\n", __FILENAME__, __LINE__, __func__, ##__VA_ARGS__)

#endif // CONSOLE_H