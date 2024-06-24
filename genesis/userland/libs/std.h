#pragma once
#include "kernel/types.h"

int write(int fd, char *buf, uint32_t count);
int read(int fd, char *buf, uint32_t count);