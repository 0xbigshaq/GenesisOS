#ifndef GFX_H
#define GFX_H
#include "kernel/types.h"

void init_gfx(void);
void fill_screen(uint32_t color);
void fill_square(uint32_t x_pos, uint32_t y_pos, uint32_t square_size, uint32_t color);

void render_colors(void);

extern int gfx_enabled;

#define ENSURE_GFX(val) if(!gfx_enabled) { return ##val; }


// mouse graphics 
#define BG_COLOR 0x707070
#define C_COLOR 0xFFFFFF

#define SCREEN_WIDTH  1024
#define SCREEN_HEIGHT 768

#endif // GFX_H