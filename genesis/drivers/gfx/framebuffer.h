#ifndef GFX_H
#define GFX_H
#include "kernel/types.h"
#include "kernel/mb.h"

typedef struct multiboot_tag_framebuffer_common fb_info_t;

void init_framebuffer(void);
void fill_screen(uint32_t color);
void fill_square(uint32_t x_pos, uint32_t y_pos, uint32_t square_size, uint32_t color);
fb_info_t* get_framebuffer_info(void);

void render_colors(void);

extern int gfx_enabled;

#define ENSURE_GFX(val) if(!gfx_enabled) { return ##val; }


// mouse graphics 
#define BG_COLOR 0x707070
#define C_COLOR 0xFFFFFF

#define SCREEN_WIDTH  1024
#define SCREEN_HEIGHT 768


/* -=--------------- */

#include "kernel/gfx/nuklear/nuklear.h"
#include "kernel/gfx/nuklear/nuklear_internal.h"

struct rawfb_context;
typedef struct rawfb_context rawfb_ctx_t;
typedef struct nk_context nk_ctx_t;

struct rawfb_pl {
    unsigned char bytesPerPixel;
    unsigned char rshift, gshift, bshift, ashift;
    unsigned char rloss, gloss, bloss, aloss;
};

/* All functions are thread-safe */
NK_API struct rawfb_context *nk_rawfb_init(void *fb, void *tex_mem, const unsigned int w, const unsigned int h, const unsigned int pitch, const struct rawfb_pl pl);
NK_API void                  nk_rawfb_render(const struct rawfb_context *rawfb, const struct nk_color clear, const unsigned char enable_clear);
NK_API void                  nk_rawfb_shutdown(struct rawfb_context *rawfb);
NK_API void                  nk_rawfb_resize_fb(struct rawfb_context *rawfb, void *fb, const unsigned int w, const unsigned int h, const unsigned int pitch, const struct rawfb_pl pl);

// #include <string.h>
// #include <stdlib.h>
#include <assert.h>

struct rawfb_image {
    void *pixels;
    int w, h, pitch;
    struct rawfb_pl pl;
};
struct rawfb_context {
    struct nk_context ctx;
    struct nk_rect scissors;
    struct rawfb_image fb;
    struct rawfb_image font_tex;
    struct nk_font_atlas atlas;
};

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a,b) ((a) < (b) ? (b) : (a))
#endif

unsigned int
nk_rawfb_color2int(const struct nk_color c, const struct rawfb_pl pl);

struct nk_color
nk_rawfb_int2color(const unsigned int i, const struct rawfb_pl pl);

void
nk_rawfb_ctx_setpixel(const struct rawfb_context *rawfb,
    const short x0, const short y0, const struct nk_color col);

void
nk_rawfb_line_horizontal(const struct rawfb_context *rawfb,
    const short x0, const short y, const short x1, const struct nk_color col);

void
nk_rawfb_img_setpixel(const struct rawfb_image *img,
    const int x0, const int y0, const struct nk_color col);

struct nk_color
nk_rawfb_img_getpixel(const struct rawfb_image *img, const int x0, const int y0);

void
nk_rawfb_img_blendpixel(const struct rawfb_image *img,
    const int x0, const int y0, struct nk_color col);

void
nk_rawfb_scissor(struct rawfb_context *rawfb,
                 const float x,
                 const float y,
                 const float w,
                 const float h);

void
nk_rawfb_stroke_line(const struct rawfb_context *rawfb,
    short x0, short y0, short x1, short y1,
    const unsigned int line_thickness, const struct nk_color col);

void
nk_rawfb_fill_polygon(const struct rawfb_context *rawfb,
    const struct nk_vec2i *pnts, int count, const struct nk_color col);

void
nk_rawfb_stroke_arc(const struct rawfb_context *rawfb,
    short x0, short y0, short w, short h, const short s,
    const short line_thickness, const struct nk_color col);

void
nk_rawfb_fill_arc(const struct rawfb_context *rawfb, short x0, short y0,
    short w, short h, const short s, const struct nk_color col);

void
nk_rawfb_stroke_rect(const struct rawfb_context *rawfb,
    const short x, const short y, const short w, const short h,
    const short r, const short line_thickness, const struct nk_color col);

void
nk_rawfb_fill_rect(const struct rawfb_context *rawfb,
    const short x, const short y, const short w, const short h,
    const short r, const struct nk_color col);

NK_API void
nk_rawfb_draw_rect_multi_color(const struct rawfb_context *rawfb,
    const short x, const short y, const short w, const short h, struct nk_color tl,
    struct nk_color tr, struct nk_color br, struct nk_color bl);


void
nk_rawfb_fill_triangle(const struct rawfb_context *rawfb,
    const short x0, const short y0, const short x1, const short y1,
    const short x2, const short y2, const struct nk_color col);


void
nk_rawfb_stroke_triangle(const struct rawfb_context *rawfb,
    const short x0, const short y0, const short x1, const short y1,
    const short x2, const short y2, const unsigned short line_thickness,
    const struct nk_color col);


void
nk_rawfb_stroke_polygon(const struct rawfb_context *rawfb,
    const struct nk_vec2i *pnts, const int count,
    const unsigned short line_thickness, const struct nk_color col);


void
nk_rawfb_stroke_polyline(const struct rawfb_context *rawfb,
    const struct nk_vec2i *pnts, const int count,
    const unsigned short line_thickness, const struct nk_color col);


void
nk_rawfb_fill_circle(const struct rawfb_context *rawfb,
    short x0, short y0, short w, short h, const struct nk_color col);


void
nk_rawfb_stroke_circle(const struct rawfb_context *rawfb,
    short x0, short y0, short w, short h, const short line_thickness,
    const struct nk_color col);


void
nk_rawfb_stroke_curve(const struct rawfb_context *rawfb,
    const struct nk_vec2i p1, const struct nk_vec2i p2,
    const struct nk_vec2i p3, const struct nk_vec2i p4,
    const unsigned int num_segments, const unsigned short line_thickness,
    const struct nk_color col);


void
nk_rawfb_clear(const struct rawfb_context *rawfb, const struct nk_color col);


NK_API struct rawfb_context*
nk_rawfb_init(void *fb, void *tex_mem, const unsigned int w, const unsigned int h,
    const unsigned int pitch, const struct rawfb_pl pl);


void
nk_rawfb_stretch_image(const struct rawfb_image *dst,
    const struct rawfb_image *src, const struct nk_rect *dst_rect,
    const struct nk_rect *src_rect, const struct nk_rect *dst_scissors,
    const struct nk_color *fg);


void
nk_rawfb_font_query_font_glyph(nk_handle handle, const float height,
    struct nk_user_font_glyph *glyph, const nk_rune codepoint,
    const nk_rune next_codepoint);




NK_API void
nk_rawfb_draw_text(const struct rawfb_context *rawfb,
    const struct nk_user_font *font, const struct nk_rect rect,
    const char *text, const int len, const float font_height,
    const struct nk_color fg);

NK_API void
nk_rawfb_drawimage(const struct rawfb_context *rawfb,
    const int x, const int y, const int w, const int h,
    const struct nk_image *img, const struct nk_color *col);


NK_API void
nk_rawfb_shutdown(struct rawfb_context *rawfb);

NK_API void
nk_rawfb_resize_fb(struct rawfb_context *rawfb,
                   void *fb,
                   const unsigned int w,
                   const unsigned int h,
                   const unsigned int pitch,
                   const struct rawfb_pl pl);

NK_API void
nk_rawfb_render(const struct rawfb_context *rawfb,
                const struct nk_color clear,
                const unsigned char enable_clear);

/* -=--------------- */
#endif // GFX_H