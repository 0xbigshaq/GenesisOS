#include "drivers/console.h"
#include "drivers/vconsole.h"
#include "kernel/gfx/nuklear/nuklear.h"
#include "kernel/string.h"
#include "drivers/mouse.h"
#include "drivers/keyboard.h"
#include "drivers/gfx/framebuffer.h"
#include "drivers/gfx/bmp.h"
#include "drivers/vconsole.h"
#include "kernel/gfx/gui.h"

uint32_t *graphics_back_buffer;
unsigned char tex_scratch[512 * 512];
gui_ctx_t gui_ctx;

gui_ctx_t* get_gui_ctx() {
    return &gui_ctx;
}

void gui_init() {
    gui_ctx_t *ctx = get_gui_ctx();
    fb_info_t *fb_info = get_framebuffer_info();
    struct rawfb_pl pl;

    ctx->k_ctx = get_keyboard_ctx();
    ctx->v_ctx = vconsole_get_ctx();
    ctx->m_ctx = get_mouse_ctx();

    ctx->fb.addr = fb_info->framebuffer_addr;
    ctx->fb.pitch = fb_info->framebuffer_pitch;
    ctx->fb.width = fb_info->framebuffer_width;
    ctx->fb.height = fb_info->framebuffer_height;
    ctx->fb.bpp_bits = fb_info->framebuffer_bpp;
    ctx->fb.bpp_bytes = fb_info->framebuffer_bpp / 8;
    ctx->fb.total = fb_info->framebuffer_width * fb_info->framebuffer_height;

    ctx->graphics_back_buffer = malloc(ctx->fb.total * ctx->fb.bpp_bytes);

    pl.bytesPerPixel = 4;
    pl.ashift = 24;
    pl.rshift = 16;
    pl.gshift = 8;
    pl.bshift = 0;
    pl.aloss = 0;
    pl.rloss = 0;
    pl.gloss = 0;
    pl.bloss = 0;


    ctx->rawfb = nk_rawfb_init(ctx->graphics_back_buffer, tex_scratch, fb_info->framebuffer_width, fb_info->framebuffer_height, fb_info->framebuffer_pitch, pl);
	ctx->nk_ctx = &ctx->rawfb->ctx;

    // styling
    nk_style_push_vec2(ctx->nk_ctx, &ctx->nk_ctx->style.window.header.padding, nk_vec2(0,0));

    // wallpaper
    ctx->bg.pixeldata = read_bmp("0:logo.bmp", &ctx->bg.fileHeader, &ctx->bg.infoHeader);
}

void gui_swapbuffers(void) {
    gui_ctx_t *ctx = get_gui_ctx();
    uint32_t *back_buffer = ctx->graphics_back_buffer;
    memmove(ctx->fb.addr, back_buffer, ctx->fb.total * ctx->fb.bpp_bytes);
}



void render_gui() {
    gui_ctx_t *ctx = get_gui_ctx();

    struct nk_context *nk_ctx = &ctx->rawfb->ctx;;
    keyboard_ctx_t *k_ctx     = ctx->k_ctx;
    vconsole_ctx_t *v_ctx     = ctx->v_ctx;
    mouse_ctx_t    *m_ctx     = ctx->m_ctx;

    if (ctx->bg.pixeldata) {
        render_bmp_to_framebuffer(ctx->bg.pixeldata, 
                                &ctx->bg.infoHeader,
                                ctx->graphics_back_buffer,
                                ctx->fb.width,
                                ctx->fb.height);
    }
    
    if (nk_begin(nk_ctx, "Terminal", nk_rect(140, 140, 700, 500),
        NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
        NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE)) {

        nk_layout_row_dynamic(nk_ctx, 450, 1);
        nk_edit_string_zero_terminated(nk_ctx, NK_EDIT_BOX, (char*)v_ctx->buf, v_ctx->cap, nk_filter_default);
    }

    nk_input_begin(nk_ctx);
    nk_input_motion(nk_ctx, m_ctx->mouse_x, m_ctx->mouse_y);
    nk_input_button(nk_ctx, NK_BUTTON_LEFT, m_ctx->mouse_x, m_ctx->mouse_y, m_ctx->packet.left_button);
    nk_input_button(nk_ctx, NK_BUTTON_MIDDLE, m_ctx->mouse_x, m_ctx->mouse_y, m_ctx->packet.middle_button);
    nk_input_button(nk_ctx, NK_BUTTON_RIGHT, m_ctx->mouse_x, m_ctx->mouse_y, m_ctx->packet.right_button);

    // flush pending keystrokes to console window 
    if(k_ctx->pending) {
        switch (k_ctx->incoming_scancode)
        {
            case KEY_BACKSPACE_PRESSED: nk_input_key(nk_ctx, NK_KEY_BACKSPACE, true); break;
            case KEY_LEFT_PRESSED: nk_input_key(nk_ctx, NK_KEY_LEFT, true); break;
            case KEY_RIGHT_PRESSED: nk_input_key(nk_ctx, NK_KEY_RIGHT, true); break;
            case KEY_TAB_PRESSED: nk_input_key(nk_ctx, NK_KEY_TAB, true); break;
        }

        if(k_ctx->pos) {
            for(int i = 0; i < k_ctx->pos; i++)
                nk_input_char(nk_ctx, k_ctx->pending_buf[i]);
        }
        keyboard_clear_pending_buf();
    }
    nk_input_end(nk_ctx);
    // end of window
    nk_end(nk_ctx);

    nk_rawfb_render(ctx->rawfb, nk_black, 0);
    gui_swapbuffers();
}
