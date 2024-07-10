#pragma once

/* The following include is a dirty hack, we include the `linker.h`
 * file here because `types.h` is almost always included in all files.
*/
#include "kernel/linker.h" // IWYU pragma: keep

typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;
typedef uint pte;
typedef char byte;

typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

#define OK 0
#define NELEM(x) (sizeof(x)/sizeof((x)[0]))