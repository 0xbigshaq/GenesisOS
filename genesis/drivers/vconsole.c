#include "kernel/device.h"
#include "kernel/interrupts.h"
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

/**
 * @brief Getter function for the global video console context object.
 * @return A pointer to the video console context object.
 */
vconsole_ctx_t* vconsole_get_ctx() {
  return &vconsole_ctx;
}


/**
 * @brief   Initializes the video console
 * @details This function initializes the video console by setting up the console device and the context object. \n
  *         It also allocates @link VCONSOLE_INITIAL_CAP @endlink bytes of memory for the `vconsole_ctx_t::buf` pointer.
 */
void vconsole_init() {
  vconsole_ctx_t *v = vconsole_get_ctx();
  all_devs[DEV_CONSOLE].write = &vconsole_write;
  all_devs[DEV_CONSOLE].read = &vconsole_read;
  v->buf = malloc(VCONSOLE_INITIAL_CAP);
  v->pos = 0;
  v->cap = VCONSOLE_INITIAL_CAP;
  splash_screen();
}


/**
 * @brief Writes data to the virtual console buffer.
 *
 * This function writes a specified number of bytes from the input buffer
 * to the virtual console's internal buffer. If the internal buffer is full,
 * it reallocates memory to accommodate additional data.
 *
 * @param buf A pointer to the input buffer containing the data to be written.
 * @param count The number of bytes to write from the input buffer.
 * @return The number of bytes written.
 */
int vconsole_write(uint8_t *buf, uint32_t count) {
  vconsole_ctx_t *v = vconsole_get_ctx();
  // dmsg("buf[0] = 0x%x, count=%d, v->pos=%d, v->cap=%d", buf[0], count, v->pos, v->cap);

  if(v->pos >= v->cap) {
    dmsg("Reallocating buffer");
    push_cli();
    v->buf = realloc(v->buf, v->cap * 2);
    v->cap *= 2;
    pop_cli();
  }

  for(uint32_t i = 0; i < count; i++) {
    v->buf[v->pos++] = buf[i];
  }
  return count;
}

/**
 * @brief Reads data from the virtual console buffer.
 *
 * This function reads a specified number of bytes into the provided buffer
 * from the virtual console. It waits for input and processes incoming
 * keyboard data, copying it to the output buffer and updating the console
 * buffer. The read operation stops if the Enter key is pressed.
 *
 * @param buf A pointer to the buffer where the read data will be stored.
 * @param count The number of bytes to read into the buffer.
 * @return The number of bytes read.
 */
int vconsole_read(uint8_t *buf, uint32_t count) {
  uint8_t c[16];
  keyboard_ctx_t *k = keyboard_get_ctx();
  int i = 0;
  int recvd = 0;

  for(; i < count;) {
      vconsole_wait_ch();
      recvd = keyboard_flush_pending_buf(1, c);
      memcpy(&buf[i], &c[0], recvd);
      i+=recvd;
      vconsole_write(c, recvd);

      if(k->incoming_scancode == KEY_ENTER_PRESSED) {
        k->incoming_scancode = 0;
        break;
      }
  }

  keyboard_clear_pending_buf();
  buf[i] = 0;
  return i;
}

/**
 * @brief Waits for a character to be available in the keyboard buffer.
 *
 * This function waits for a character to be available in the keyboard buffer
 * before returning. It is used to synchronize the console with the keyboard
 * input.
 *
 * @return 1
 */
uint8_t vconsole_wait_ch() {
  keyboard_ctx_t *k = keyboard_get_ctx();
  while(k->pending_char == 0) {
    sleep(1); // channel 1 is for keyboard
  }

  return 1;
}

/**
 * @brief Displays the splash screen.
 *
 * This function displays the splash screen on the console. It reads the
 * font and logo files from the FAT filesystem and displays them on the
 * screen.
 */
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