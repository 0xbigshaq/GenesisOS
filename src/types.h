#pragma once

/* The following include is a dirty hack, we include the `linker.h`
 * file here because `types.h` is almost always included in all files.
*/
#include "linker.h"

typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;
typedef uint pde_t;
typedef char byte;

#define NELEM(x) (sizeof(x)/sizeof((x)[0]))