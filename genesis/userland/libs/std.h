#pragma once
#include "kernel/types.h" // IWYU pragma: keep

void *memcpy(void *dest, const void *src, unsigned int n);

int write(int fd, char *buf, uint32_t count);
int read(int fd, char *buf, uint32_t count);
int open(char *pathname, uint32_t flags);
__attribute__((noreturn)) void exit(int rc);