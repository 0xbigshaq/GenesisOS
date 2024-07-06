#include "kernel/mb.h"
#include "drivers/gfx.h"
#include "drivers/console.h"
#include <stdint.h>

int gfx_enabled = 0;  // `parse_multiboot2_info` set this to 1


void init_gfx(void) {
    ENSURE_GFX()
    kprintf("[+] fb_info.common @ 0x%x \n", &fb_info.common);

    // fill_screen(BG_COLOR);
}

void fill_screen(uint32_t color) {
    ENSURE_GFX()
    uint32_t* framebuffer = (uint32_t*)fb_info.common.framebuffer_addr;

    for (uint32_t y = 0; y < fb_info.common.framebuffer_height; ++y) {
        for (uint32_t x = 0; x < fb_info.common.framebuffer_width; ++x) {
            framebuffer[y * fb_info.common.framebuffer_width + x] = color;
        }
    }
}

void fill_square(uint32_t x_pos, uint32_t y_pos, uint32_t square_size, uint32_t color) {
    uint32_t* framebuffer = (uint32_t*)fb_info.common.framebuffer_addr;
    for (uint32_t y = y_pos; y < (y_pos+square_size); ++y) {
        for (uint32_t x = x_pos; x < (x_pos+square_size); ++x) {
            framebuffer[y * fb_info.common.framebuffer_width + x] = color;
        }
    }
}

// for testing purposes hehe
void render_colors(void) {
    ENSURE_GFX()

    uint32_t idx = 0, lol = 0, r = 0, g = 0, b = 0;

    while(idx<(0xff*3)) {
        if(b < 0xff)
            b++;
        else if(g < 0xff)
            g++;
        else if(r < 0xff)
            r++;

        lol = (r<<16) | (g<<8) | (b<<0); // color in 0xRRGGBB format
        fill_screen(lol);
        idx++;
    }
}