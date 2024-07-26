/**
 * @file vconsole.h
 * @brief Video Console impl
 */
#ifndef VCONSOLE_H
#define VCONSOLE_H

#include <stdint.h>

#define VCONSOLE_INITIAL_CAP 1024       //!< Initial capacity of the `vconsole_ctx_t::buf` pointer in the video console context object

/**
 * @brief Video Console Context Object, contains the input and output
 */
typedef struct vconsole_ctx {
    uint8_t *buf;       //!< Buffer pointer, contains the input and output of all the texts that is displayed on the screen.
    uint32_t pos;       //!< Position/cursor that points to where the next character will be written.
    uint32_t cap;       //!< Capacity of `buf`
} vconsole_ctx_t;

void vconsole_init();
int vconsole_write(uint8_t *buf, uint32_t count);
int vconsole_read(uint8_t *buf, uint32_t count);
uint8_t vconsole_wait_ch();
vconsole_ctx_t* vconsole_get_ctx();
void splash_screen();

#define FONT_PATH "0:font.sfn"  //!< Path to the font file used in the splash screen
#define LOGO_PATH "0:logo.bmp"  //!< Path to the logo file used in the splash screen
#define VC_COLOR_LIME 0x05ff5d
#define VC_COLOR_WHITE 0xffffff
#define VC_COLOR_BLACK 0x000000

#endif // VCONSOLE_H