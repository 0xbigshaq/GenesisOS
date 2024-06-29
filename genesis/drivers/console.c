#include "kernel/memlayout.h"
#include "kernel/file.h"
#include "drivers/console.h"
#include "drivers/uart.h"

// Video Memory related funcs
uint16_t *const video = (uint16_t*) phys_to_virt(0xB8000);

int console_write(uint8_t *buf, uint32_t count) {
  int off = 0;
  for(; off < count; off++) {
    uart_putchar(buf[off]);
  }
  return off;
}

int console_read(uint8_t *buf, uint32_t count) {
  int i = 0;
  char c = NULL;

  while(i < count) {
    c = uart_getchar();
    uart_putchar(c); // display char as the user type it
    if (c == '\n' || c == '\r') {
        break;
    }

    else if(c == 0x7f && i>0) {
        uart_putchar(0x08); // backspace
        uart_putchar(0x20); // clear char
        uart_putchar(0x08); // backspace
        buf[i] = 0x00;
        i--;
    }
    
    else if( c != 0x7f) {
        buf[i] = c;
        i++;
    }
    else {
        continue;
    }
  }
  buf[i] = 0;
  return i;
}

void init_console() {
  devices[DEV_CONSOLE].write = &console_write;
  devices[DEV_CONSOLE].read = &console_read;
}

void putc(uint8_t x, uint8_t y, enum color fg, enum color bg, char c) {
    video[y * COLS + x] = (bg << 12) | (fg << 8) | c;
}

int console_getchar(void) {
  int ch;
  ch = uart_getchar();
  if(ch == 0x7f) {
    uart_putchar('\b');
    uart_putchar(' ');
    uart_putchar('\b');
  } else if(ch == '\r' || ch == '\n') {
    uart_putchar('\r');
    uart_putchar('\n');
  } else {
    uart_putchar(ch);
  }
  return ch;
}

void puts(uint8_t x, uint8_t y, enum color fg, enum color bg, const char *s) {
    while(*s != 0) {
        putc(x, y, fg, bg, *s);
        s++; x++;
    }
}

void clear(enum color bg) {
    uint8_t x, y;
    for (y = 0; y < ROWS; y++)
        for (x = 0; x < COLS; x++)
            putc(x, y, bg, bg, ' ');
}


// UART/Serial related funcs
void printint(int xx, int base, int sign)
{
  static char digits[] = "0123456789abcdef";
  char buf[16];
  int i;
  uint x;

  if(sign && (sign = xx < 0))
    x = -xx;
  else
    x = xx;

  i = 0;
  do{
    buf[i++] = digits[x % base];
  }while((x /= base) != 0);

  if(sign)
    buf[i++] = '-';

  while(--i >= 0)
    uart_putchar(buf[i]);
}

// Print to the console. only understands %d, %x, %p, %s.
void kprintf(char *fmt, ...)
{
  int i, c;
  uint *argp;
  char *s;

  argp = (uint*)(void*)(&fmt + 1);
  for(i = 0; (c = fmt[i] & 0xff) != 0; i++){
    if(c != '%'){
      uart_putchar(c);
      continue;
    }
    c = fmt[++i] & 0xff;
    if(c == 0)
      break;
    switch(c){
    case 'd':
      printint(*argp++, 10, 1);
      break;
    case 'x':
    case 'p':
      printint(*argp++, 16, 0);
      break;
    case 's':
      if((s = (char*)*argp++) == 0)
        s = "(null)";
      for(; *s; s++)
        uart_putchar(*s);
      break;
    case '%':
      uart_putchar('%');
      break;
    default:
      // Print unknown % sequence to draw attention.
      uart_putchar('%');
      uart_putchar(c);
      break;
    }
  }
}