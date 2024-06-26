#include "kernel/types.h"
// string.c
int             memcmp(const void*, const void*, uint);
void*           memmove(void*, const void*, uint);
void*           memset2(void*, int, uint);
int             strlen(const char*);
int             strncmp(const char*, const char*, uint);
char*           strncpy(char*, const char*, int);
void*           memcpy(void *dst, const void *src, uint n);