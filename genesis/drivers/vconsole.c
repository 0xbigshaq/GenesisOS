#include "kernel/file.h"
#include "kernel/x86.h"
#include "kernel/kmalloc.h"
#include "kernel/string.h"
#include "kernel/mb.h"
#define SSFN_CONSOLEBITMAP_TRUECOLOR 1        /* use the special renderer for 32 bit truecolor packed pixels */
#include "drivers/gfx/ssfn.h"
#include "drivers/keyboard.h"
#include "drivers/console.h"
#include "drivers/vconsole.h"
#include "drivers/gfx/bmp.h"

ssfn_font_t *ssfn_src;  // Font info, for SSFN
ssfn_buf_t ssfn_dst;    // Framebuffer context
uint8_t *vera_sfn_data; // Font data, lays in kernel heap
vconsole_ctx_t vconsole_ctx;

vconsole_ctx_t* vconsole_get_ctx() {
  return &vconsole_ctx;
}

void init_vconsole() {
  vconsole_ctx_t *ctx = vconsole_get_ctx();
  devices[DEV_CONSOLE].write = &video_console_write;
  devices[DEV_CONSOLE].read = &video_console_read;
  ctx->buf = malloc(1024);
  ctx->pos = 0;
  ctx->cap = 1024;
  splash_screen();
}

int video_console_write(uint8_t *buf, uint32_t count) {
  vconsole_ctx_t *ctx = vconsole_get_ctx();
  for(uint32_t i = 0; i < count; i++) {
    ctx->buf[ctx->pos++] = buf[i];
  }
  // FIXME: add a check if we are at the end of the buffer & realloc
  return count;
}

int video_console_read(uint8_t *buf, uint32_t count) {
  uint8_t c;
  int i = 0;
  for(; i < count; i++) {
    sti(); // Enable interrupts, make it non-blocking I/O
    c = video_console_getc();
    cli();
    if (c == '\n' || c == '\r') {
        break;
    }
    buf[i] = c;
  }
  buf[i] = 0;
  return i;

}

uint8_t video_console_getc() {
  keyboard_ctx_t *ctx = get_keyboard_ctx();
  while(ctx->incoming_char == 0);
  return ctx->incoming_char;
}

void splash_screen() {
    FRESULT rc;
    UINT br;
    FILINFO fno;
    FIL font_fp;
    uint32_t bmp_banner_h; 

    bmp_banner_h = display_bmp(LOGO_PATH,
             fb_info.common.framebuffer_addr, 
             fb_info.common.framebuffer_width,
             fb_info.common.framebuffer_height);    

    rc = f_stat(FONT_PATH, &fno);
    if(rc != FR_OK) goto fatal_err;    
    kprintf("[*] font.sfn was found, size: %d\n", fno.fsize);
    vera_sfn_data = malloc(fno.fsize);
    kprintf("[*] vera_sfn_data @ 0x%x\n", vera_sfn_data);

    rc = f_open(&font_fp, FONT_PATH, FA_READ);
    if(rc != FR_OK) goto fatal_err;

    rc = f_read(&font_fp, vera_sfn_data, fno.fsize, &br);
    if(rc != FR_OK) goto fatal_err;

    kprintf("[*] %d bytes read\n", br);
    f_close(&font_fp);
    
    /* the bitmap font to use */
    ssfn_src = (ssfn_font_t *)vera_sfn_data;

    /* framebuffer address and bytes per line */
    ssfn_dst.ptr = fb_info.common.framebuffer_addr;
    ssfn_dst.p = fb_info.common.framebuffer_pitch;
    ssfn_dst.w = fb_info.common.framebuffer_width;
    ssfn_dst.h = fb_info.common.framebuffer_height;

    /* color settings */
    ssfn_dst.fg = VC_COLOR_LIME;
    ssfn_dst.bg = VC_COLOR_BLACK;

    /* coordinates to draw to */
    ssfn_dst.x = 0;
    ssfn_dst.y = 0; 

    /* Sanity Check */   
    ssfn_putc('G');
    ssfn_putc('e');
    ssfn_putc('n');
    ssfn_putc('e');
    ssfn_putc('s');
    ssfn_putc('i');
    ssfn_putc('s');
    ssfn_putc(' ');
    ssfn_putc('O');
    ssfn_putc('S');
    ssfn_putc('~');

    /* colors, white on black */
    ssfn_dst.fg = VC_COLOR_WHITE;
    ssfn_dst.bg = VC_COLOR_BLACK;
    ssfn_dst.y += bmp_banner_h;
    ssfn_dst.x = 0;

    return;

    fatal_err:
        kprintf("[!] FAT FS Error: %d\n", rc);
        kprintf("Cannot open font.sfn file");

}