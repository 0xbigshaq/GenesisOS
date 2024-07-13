#include "drivers/console.h"
#include "kernel/assert.h"
#include "kernel/gfx/nuklear/nuklear.h"
#include "kernel/kmalloc.h"
#include "kernel/string.h"
#include "drivers/mouse.h"
#include "drivers/keyboard.h"
#include "drivers/gfx/framebuffer.h"
#include "kernel/mb.h"
#include "drivers/gfx/bmp.h"

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


enum theme {THEME_BLACK, THEME_WHITE, THEME_RED, THEME_BLUE, THEME_DARK};

static void
set_style(struct nk_context *ctx, enum theme theme)
{
    struct nk_color table[NK_COLOR_COUNT];
    if (theme == THEME_WHITE) {
        table[NK_COLOR_TEXT] = nk_rgba(20, 20, 20, 255);
        table[NK_COLOR_WINDOW] = nk_rgba(185, 185, 185, 255);
        table[NK_COLOR_HEADER] = nk_rgba(150, 150, 150, 255);
        table[NK_COLOR_BORDER] = nk_rgba(0, 0, 0, 255);
        table[NK_COLOR_BUTTON] = nk_rgba(210, 210, 210, 255);
        table[NK_COLOR_BUTTON_HOVER] = nk_rgba(180, 180, 180, 255);
        table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(160, 160, 160, 255);
        table[NK_COLOR_TOGGLE] = nk_rgba(150, 150, 150, 255);
        table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(120, 120, 120, 255);
        table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(175, 175, 175, 255);
        table[NK_COLOR_SELECT] = nk_rgba(190, 190, 190, 255);
        table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(175, 175, 175, 255);
        table[NK_COLOR_SLIDER] = nk_rgba(190, 190, 190, 255);
        table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(80, 80, 80, 255);
        table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(70, 70, 70, 255);
        table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(60, 60, 60, 255);
        table[NK_COLOR_PROPERTY] = nk_rgba(175, 175, 175, 255);
        table[NK_COLOR_EDIT] = nk_rgba(150, 150, 150, 255);
        table[NK_COLOR_EDIT_CURSOR] = nk_rgba(0, 0, 0, 255);
        table[NK_COLOR_COMBO] = nk_rgba(175, 175, 175, 255);
        table[NK_COLOR_CHART] = nk_rgba(160, 160, 160, 255);
        table[NK_COLOR_CHART_COLOR] = nk_rgba(45, 45, 45, 255);
        table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba( 255, 0, 0, 255);
        table[NK_COLOR_SCROLLBAR] = nk_rgba(180, 180, 180, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(140, 140, 140, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(150, 150, 150, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(160, 160, 160, 255);
        table[NK_COLOR_TAB_HEADER] = nk_rgba(180, 180, 180, 255);
        nk_style_from_table(ctx, table);
    } else {
        nk_style_default(ctx);
    }
}

void woot() {
    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;
             
    unsigned char* pixelData = read_bmp("0:logo.bmp", &fileHeader, &infoHeader);


    graphics_back_buffer = malloc(FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT * sizeof(uint32_t));
    struct rawfb_pl pl;
    mouse_ctx_t *m_ctx;
    keyboard_ctx_t *k_ctx;
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

    nk_style_push_vec2(nk_ctx, &nk_ctx->style.window.header.padding, nk_vec2(0,0));

    char input[256];
    char output[1024];
    memset(input, 0, sizeof(input));
    memset(output, 0, sizeof(output));

    while(1) {
        if (pixelData) {
            render_bmp_to_framebuffer(pixelData, &infoHeader, graphics_back_buffer, fb_info.common.framebuffer_width, fb_info.common.framebuffer_height);
        }
        
        if (nk_begin(nk_ctx, "Terminal", nk_rect(140, 140, 700, 500),
            NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
            NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE)) {

            nk_layout_row_dynamic(nk_ctx, 450, 1);
            nk_edit_string_zero_terminated(nk_ctx, NK_EDIT_BOX, output, sizeof(output), nk_filter_default);
        }

    m_ctx = get_mouse_ctx();
    k_ctx = get_keyboard_ctx();
    nk_input_begin(nk_ctx);
    nk_input_motion(nk_ctx, m_ctx->mouse_x, m_ctx->mouse_y);
    nk_input_button(nk_ctx, NK_BUTTON_LEFT, m_ctx->mouse_x, m_ctx->mouse_y, m_ctx->packet.left_button);
    nk_input_button(nk_ctx, NK_BUTTON_MIDDLE, m_ctx->mouse_x, m_ctx->mouse_y, m_ctx->packet.middle_button);
    nk_input_button(nk_ctx, NK_BUTTON_RIGHT, m_ctx->mouse_x, m_ctx->mouse_y, m_ctx->packet.right_button);

    if(k_ctx->pending) {
        switch (k_ctx->incoming_scancode)
		{
            // case KEY_ENTER_PRESSED: nk_input_key(nk_ctx, NK_KEY_ENTER, true); break;
            case KEY_BACKSPACE_PRESSED: nk_input_key(nk_ctx, NK_KEY_BACKSPACE, true); break;
            case KEY_LEFT_PRESSED: nk_input_key(nk_ctx, NK_KEY_LEFT, true); break;
            case KEY_RIGHT_PRESSED: nk_input_key(nk_ctx, NK_KEY_RIGHT, true); break;
            // case KEY_UP_PRESSED: nk_input_key(nk_ctx, NK_KEY_UP, true); break;
            // case KEY_DOWN_PRESSED: nk_input_key(nk_ctx, NK_KEY_DOWN, true); break;
            case KEY_TAB_PRESSED: nk_input_key(nk_ctx, NK_KEY_TAB, true); break;
        }

        // flush to console window 
        if(k_ctx->pos) {
            for(int i = 0; i < k_ctx->pos; i++)
                nk_input_char(nk_ctx, k_ctx->pending_buf[i]);
        }
        keyboard_clear_pending_buf();
    }
    nk_input_end(nk_ctx);
    
    nk_end(nk_ctx);

    nk_rawfb_render(rawfb, nk_black, 0);
    graphics_swapbuffers();
    }
}
