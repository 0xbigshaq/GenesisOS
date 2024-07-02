#ifndef GFX_H
#define GFX_H
#include "kernel/types.h"

void init_gfx(void);
void fill_screen(uint32_t color);

void render_colors(void);

extern int gfx_enabled;

#define ENSURE_GFX(val) if(!gfx_enabled) { return ##val; }

#endif // GFX_H