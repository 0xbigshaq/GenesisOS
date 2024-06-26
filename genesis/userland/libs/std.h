#pragma once
#include "kernel/types.h" // IWYU pragma: keep

int write(int fd, char *buf, uint32_t count);
int read(int fd, char *buf, uint32_t count);