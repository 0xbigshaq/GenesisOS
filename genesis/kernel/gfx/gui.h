#ifndef GUI_H
#define GUI_H

#include "drivers/gfx/framebuffer.h"
#include "drivers/mouse.h"
#include "drivers/keyboard.h"
#include "drivers/vconsole.h"
#include "drivers/gfx/bmp.h"

struct fb {
    uint32_t *addr;
    uint32_t pitch;
    uint32_t width;
    uint32_t height;
    uint32_t bpp_bits;
    uint32_t bpp_bytes;
    uint32_t total;
};
typedef struct fb fb_t;

struct bg {
    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;
    unsigned char *pixeldata;
};
typedef struct bg bg_t;

struct gui_ctx {
    // framebuffer / rendering
    rawfb_ctx_t *rawfb;
    nk_ctx_t *nk_ctx;
    fb_t fb;

    // kvm
    keyboard_ctx_t *k_ctx;
    vconsole_ctx_t *v_ctx;
    mouse_ctx_t *m_ctx;

    // gui 
    uint32_t *graphics_back_buffer;
    unsigned char tex_scratch[512 * 512];
    bg_t bg;
};

typedef struct gui_ctx gui_ctx_t;

void render_gui();
void gui_init();
void gui_swapbuffers(void);
gui_ctx_t* get_gui_ctx();

#endif // GUI_H