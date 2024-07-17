#ifndef VCONSOLE_H
#define VCONSOLE_H

#include <stdint.h>

#define VCONSOLE_INITIAL_CAP 1024

struct vconsole_ctx {
    uint8_t *buf;
    uint32_t pos;
    uint32_t cap;
};

typedef struct vconsole_ctx vconsole_ctx_t;

void vconsole_init();
int vconsole_write(uint8_t *buf, uint32_t count);
int vconsole_read(uint8_t *buf, uint32_t count);
uint8_t vconsole_wait_ch();
vconsole_ctx_t* vconsole_get_ctx();
void splash_screen();

#define FONT_PATH "0:font.sfn"
#define LOGO_PATH "0:logo.bmp"
#define VC_COLOR_LIME 0x05ff5d
#define VC_COLOR_WHITE 0xffffff
#define VC_COLOR_BLACK 0x000000

#endif // VCONSOLE_H