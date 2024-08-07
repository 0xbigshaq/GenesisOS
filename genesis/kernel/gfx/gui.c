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

/**
 * @brief Getter for the global GUI context object
 * 
 * @returns gui_ctx_t* 
 */
gui_ctx_t* get_gui_ctx() {
    return &gui_ctx;
}

/**
 * @brief Initialize the GUI context.
 * @details This function initializes the GUI context by fetching information from the
            VGA driver/framebuffer, Keyboard and Mouse.
 * @return void
 */
void gui_init() {
    gui_ctx_t *ctx = get_gui_ctx();
    fb_info_t *fb_info = get_framebuffer_info();
    struct rawfb_pl pl;

    ctx->k_ctx = keyboard_get_ctx();
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

/**
 * @brief Double-buffering swap function.
 * @details To reduce flickering/lagging, this function swaps the back buffer with the front buffer. \n
 *          While the back buffer is being rendered, the front buffer is displayed.
 * @return void
 * 
 */
void gui_swapbuffers(void) {
    gui_ctx_t *ctx = get_gui_ctx();
    uint32_t *back_buffer = ctx->graphics_back_buffer;
    memmove(ctx->fb.addr, back_buffer, ctx->fb.total * ctx->fb.bpp_bytes);
}


/**
 * @brief   Renders the GUI(one frame).
 * @details This function rendering a single frame to the screen, it is called
            on every iteration in the main `scheduler()` loop.
 * @return  void
 */
void render_gui() {
    gui_ctx_t *gui = get_gui_ctx();

    struct nk_context *nk_ctx = &gui->rawfb->ctx;
    vconsole_ctx_t *v_ctx     = gui->v_ctx;
    mouse_ctx_t    *m_ctx     = gui->m_ctx;

    int line_count = 0;
    
    if (gui->bg.pixeldata) {
        render_bmp_to_framebuffer(gui->bg.pixeldata, 
                                &gui->bg.infoHeader,
                                gui->graphics_back_buffer,
                                gui->fb.width,
                                gui->fb.height);
    }
    
    if (nk_begin(nk_ctx, "Terminal", nk_rect(140, 140, 700, 500),
        NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
        NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE)) {
        nk_layout_row_dynamic(nk_ctx, 450, 1);
        nk_edit_string(nk_ctx, NK_EDIT_BOX, (char*)v_ctx->buf, (int*)&v_ctx->pos, v_ctx->cap, nk_filter_default);

        // Calculate the number of lines in the buffer
        for (int i = 0; i < v_ctx->pos; i++) {
            if (v_ctx->buf[i] == '\n') {
                line_count++;
            }
        }

        // Auto-scroll the buffer if it overflows the window
        // FIXME: This is not fully automatic, to start the auto-scroll, the user
        // needs to press the scroll bar one time.
        if (line_count * nk_ctx->style.font->height > 450) {
            nk_ctx->current->edit.scrollbar.y = line_count * nk_ctx->style.font->height;
        }
    }

    nk_input_begin(nk_ctx);
    nk_input_motion(nk_ctx, m_ctx->mouse_x, m_ctx->mouse_y);
    nk_input_button(nk_ctx, NK_BUTTON_LEFT, m_ctx->mouse_x, m_ctx->mouse_y, m_ctx->packet.left_button);
    nk_input_button(nk_ctx, NK_BUTTON_MIDDLE, m_ctx->mouse_x, m_ctx->mouse_y, m_ctx->packet.middle_button);
    nk_input_button(nk_ctx, NK_BUTTON_RIGHT, m_ctx->mouse_x, m_ctx->mouse_y, m_ctx->packet.right_button);

    nk_input_end(nk_ctx);
    // end of window
    nk_end(nk_ctx);

    nk_rawfb_render(gui->rawfb, nk_black, 0);
    gui_swapbuffers();
}
