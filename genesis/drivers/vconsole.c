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
#include "kernel/proc.h"

ssfn_font_t *ssfn_src;  // Font info, for SSFN
ssfn_buf_t ssfn_dst;    // Framebuffer context
uint8_t *vera_sfn_data; // Font data, lays in kernel heap
vconsole_ctx_t vconsole_ctx;

vconsole_ctx_t* vconsole_get_ctx() {
  return &vconsole_ctx;
}

void vconsole_init() {
  vconsole_ctx_t *v = vconsole_get_ctx();
  devices[DEV_CONSOLE].write = &vconsole_write;
  devices[DEV_CONSOLE].read = &vconsole_read;
  v->buf = malloc(VCONSOLE_INITIAL_CAP);
  v->pos = 0;
  v->cap = VCONSOLE_INITIAL_CAP;
  splash_screen();
}

int vconsole_write(uint8_t *buf, uint32_t count) {
  vconsole_ctx_t *v = vconsole_get_ctx();
  // dmsg("buf[0] = 0x%x, count=%d, v->pos=%d, v->cap=%d", buf[0], count, v->pos, v->cap);

  if(v->pos >= v->cap) {
    dmsg("Reallocating buffer");
    v->buf = realloc(v->buf, v->cap * 2);
    v->cap *= 2;
  }

  for(uint32_t i = 0; i < count; i++) {
    v->buf[v->pos++] = buf[i];
  }
  return count;
}

int vconsole_read(uint8_t *buf, uint32_t count) {
  dmsg("entered.");
  uint8_t c[16];
  keyboard_ctx_t *k = keyboard_get_ctx();
  int i = 0;
  int recvd = 0;

  for(; i < count;) {
      vconsole_wait_ch();
      recvd = keyboard_flush_pending_buf(2, c);
      memcpy(&buf[i], &c[0], recvd);
      i+=recvd;
      vconsole_write(c, recvd);

      if(k->incoming_scancode == KEY_ENTER_PRESSED) {
        dmsg("woot wtf");
        k->incoming_scancode = 0;
        break;
      }
  }

  keyboard_clear_pending_buf();
  buf[i] = 0;
  return i;
}

uint8_t vconsole_wait_ch() {
  keyboard_ctx_t *k = keyboard_get_ctx();
  while(k->pending_char == 0) {
    sleep(1); // channel 1 is for keyboard
  }

  return 1;
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
    dmsg("[*] font.sfn was found, size: %d", fno.fsize);
    vera_sfn_data = malloc(fno.fsize);
    dmsg("[*] vera_sfn_data @ 0x%x", vera_sfn_data);

    rc = f_open(&font_fp, FONT_PATH, FA_READ);
    if(rc != FR_OK) goto fatal_err;

    rc = f_read(&font_fp, vera_sfn_data, fno.fsize, &br);
    if(rc != FR_OK) goto fatal_err;

    dmsg("[*] %d bytes read", br);
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
        dmsg("[!] FAT FS Error: %d\n", rc);
        dmsg("Cannot open font.sfn file");

}