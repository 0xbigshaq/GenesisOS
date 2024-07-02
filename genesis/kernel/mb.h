#ifndef MB_H
#define MB_H

#include "kernel/multiboot2.h"
void parse_multiboot2_info(void* multiboot_info);

typedef struct multiboot_tag_framebuffer mb_tag_framebuffer_t;
extern mb_tag_framebuffer_t fb_info;

#endif // MB_H
