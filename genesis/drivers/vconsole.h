#ifndef VCONSOLE_H
#define VCONSOLE_H

#include <stdint.h>

void init_vconsole();
int video_console_write(uint8_t *buf, uint32_t count);
int video_console_read(uint8_t *buf, uint32_t count);
uint8_t video_console_getc();
void splash_screen();

#define FONT_PATH "0:font.sfn"
#define LOGO_PATH "0:logo.bmp"
#define VC_COLOR_LIME 0x05ff5d
#define VC_COLOR_WHITE 0xffffff
#define VC_COLOR_BLACK 0x000000

#endif // VCONSOLE_H