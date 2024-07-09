#include "kernel/assert.h"
#include "drivers/console.h"

void __assert_fail(const char* assertion, const char* file, unsigned int line, const char* function) {
    kprintf("[!] Assertion failed: %s, function %s, file %s, line %d.\n", assertion, function, file, line);
    while (1) {}  // spin forever, halt the CPU
}