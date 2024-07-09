#include "drivers/console.h"
#include "kernel/assert.h"
#include "kernel/kmalloc.h"
#include "drivers/mouse.h"
#include "drivers/gfx/framebuffer.h"

struct nk_context ctx;


#define FRAMEBUFFER_ADDR 0xfd000000
#define FRAMEBUFFER_WIDTH 1024
#define FRAMEBUFFER_HEIGHT 768
#define FRAMEBUFFER_PITCH 4096
#define FRAMEBUFFER_BPP 32

uint32_t *graphics_back_buffer;

void* framebuffer = (void*)FRAMEBUFFER_ADDR;

struct rawfb_context *rawfb;
struct nk_context *nk_ctx;
unsigned char tex_scratch[512 * 512];

void graphics_swapbuffers(void)
{
    uint32_t *buffer = FRAMEBUFFER_ADDR;
    uint32_t *back_buffer = graphics_back_buffer;
    uint32_t *end_buffer = graphics_back_buffer + FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT;
    for (; back_buffer < end_buffer; back_buffer++, buffer++)
        *buffer = *back_buffer;
}

void woot() {
    graphics_back_buffer = malloc(FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT * sizeof(uint32_t));
    // kprintf("woot\n");
    struct rawfb_pl pl;
    mouse_ctx_t *m_ctx;
    pl.bytesPerPixel = 4;
    pl.ashift = 24;
    pl.rshift = 16;
    pl.gshift = 8;
    pl.bshift = 0;
    pl.aloss = 0;
    pl.rloss = 0;
    pl.gloss = 0;
    pl.bloss = 0;

    rawfb = nk_rawfb_init(graphics_back_buffer, tex_scratch, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, FRAMEBUFFER_PITCH, pl);
	nk_ctx = &rawfb->ctx;

    // nk_style_push_color(nk_ctx, &nk_ctx->style.window.background, nk_rgb(255,255,255));
    // nk_style_push_style_item(nk_ctx, &nk_ctx->style.window.fixed_background, nk_style_item_color(nk_rgb(255,255,255)));
    // nk_style_push_style_item(nk_ctx, &nk_ctx->style.cursors[], nk_style_item_color(nk_rgb(100,100,100)));

    // nk_style_push_style_item(nk_ctx, &nk_ctx->style.button.normal, nk_style_item_color(nk_rgb(255,0,0))); 
    // nk_style_push_style_item(nk_ctx, &nk_ctx->style.button.hover, nk_style_item_color(nk_rgb(200,0,0))); 
    // nk_style_push_style_item(nk_ctx, &nk_ctx->style.button.active, nk_style_item_color(nk_rgb(255,0,0)));

    while(1) {
    if (nk_begin(nk_ctx, "Genesis OS", nk_rect(512-100, 384-100, 200, 200), NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_MOVABLE|NK_WINDOW_BORDER|NK_WINDOW_TITLE|NK_WINDOW_CLOSABLE))
    {
            // kprintf("nk_begin\n");
            nk_layout_row_static(nk_ctx, 20, 175, 1);
            nk_label(nk_ctx, "GUI Testing", NK_TEXT_ALIGN_CENTERED);
            nk_label(nk_ctx, "whattttt", NK_TEXT_ALIGN_CENTERED);
            nk_button_label(nk_ctx, "Continue btn");
    }
    // nk_end(nk_ctx);

    m_ctx = get_mouse_ctx();
    nk_input_begin(nk_ctx);
    nk_input_motion(nk_ctx, m_ctx->mouse_x, m_ctx->mouse_y);
    nk_input_button(nk_ctx, NK_BUTTON_LEFT, m_ctx->mouse_x, m_ctx->mouse_y, m_ctx->packet.left_button);
    nk_input_button(nk_ctx, NK_BUTTON_MIDDLE, m_ctx->mouse_x, m_ctx->mouse_y, m_ctx->packet.middle_button);
    nk_input_button(nk_ctx, NK_BUTTON_RIGHT, m_ctx->mouse_x, m_ctx->mouse_y, m_ctx->packet.right_button);
    nk_input_end(nk_ctx);
    
    nk_end(nk_ctx);

    nk_rawfb_render(rawfb, nk_black, 1);
    graphics_swapbuffers();
    }
}
