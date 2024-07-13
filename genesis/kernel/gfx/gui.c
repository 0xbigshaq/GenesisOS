#include "drivers/console.h"
#include "kernel/gfx/nuklear/nuklear.h"
#include "kernel/string.h"
#include "drivers/mouse.h"
#include "drivers/keyboard.h"
#include "drivers/gfx/framebuffer.h"
#include "drivers/gfx/bmp.h"


uint32_t *graphics_back_buffer;
struct rawfb_context *rawfb;
struct nk_context *nk_ctx;
unsigned char tex_scratch[512 * 512];

void graphics_swapbuffers(void) {
    fb_info_t *fb_info = get_framebuffer_info();
    uint32_t *back_buffer = graphics_back_buffer;
    uint32_t *end_buffer = graphics_back_buffer + (fb_info->framebuffer_width * fb_info->framebuffer_height);
    memmove(fb_info->framebuffer_addr, back_buffer, (end_buffer - back_buffer) * (fb_info->framebuffer_bpp / 8));
}

void render_gui() {
    // framebuffer info
    fb_info_t *fb_info = get_framebuffer_info();
    
    // wallpapaer
    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;
    unsigned char* pixelData = read_bmp("0:logo.bmp", &fileHeader, &infoHeader);

    // Inputs
    mouse_ctx_t *m_ctx;
    keyboard_ctx_t *k_ctx;

    // GUI
    graphics_back_buffer = malloc(fb_info->framebuffer_width *fb_info->framebuffer_height * (fb_info->framebuffer_bpp / 8));
    struct rawfb_pl pl;
    pl.bytesPerPixel = 4;
    pl.ashift = 24;
    pl.rshift = 16;
    pl.gshift = 8;
    pl.bshift = 0;
    pl.aloss = 0;
    pl.rloss = 0;
    pl.gloss = 0;
    pl.bloss = 0;


    rawfb = nk_rawfb_init(graphics_back_buffer, tex_scratch, fb_info->framebuffer_width, fb_info->framebuffer_height, fb_info->framebuffer_pitch, pl);
	nk_ctx = &rawfb->ctx;

    nk_style_push_vec2(nk_ctx, &nk_ctx->style.window.header.padding, nk_vec2(0,0));

    char input[256];
    char output[1024];
    memset2(input, 0, sizeof(input));
    memset2(output, 0, sizeof(output));

    while(1) {
        if (pixelData) {
            render_bmp_to_framebuffer(pixelData, &infoHeader, graphics_back_buffer, fb_info->framebuffer_width, fb_info->framebuffer_height);
        }
        
        if (nk_begin(nk_ctx, "Terminal", nk_rect(140, 140, 700, 500),
            NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
            NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE)) {

            nk_layout_row_dynamic(nk_ctx, 450, 1);
            nk_edit_string_zero_terminated(nk_ctx, NK_EDIT_BOX, output, sizeof(output), nk_filter_default);
        }

    // Keep input state updated
    m_ctx = get_mouse_ctx();
    k_ctx = get_keyboard_ctx();
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

    nk_rawfb_render(rawfb, nk_black, 0);
    graphics_swapbuffers();
    }
}
