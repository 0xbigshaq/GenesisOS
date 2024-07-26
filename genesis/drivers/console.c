#include "kernel/memlayout.h"
#include "kernel/device.h"
#include "drivers/console.h"
#include "drivers/uart.h"


/**
 * @brief   Write to the console `count` characters from `buf`.
 */
int console_write(uint8_t *buf, uint32_t count) {
  int off = 0;
  for(; off < count; off++) {
    uart_putchar(buf[off]);
  }
  return off;
}

/**
 * @brief   Read from the console `count` characters into `buf`.
 */
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

/**
 * @brief Initializes the console device and sets the read and write functions. \n
            Those functions are triggered in syscalls like `SYS_read` and `SYS_write`.
 */
void init_console() {
  all_devs[DEV_CONSOLE].write = &console_write;
  all_devs[DEV_CONSOLE].read = &console_read;
}

/**
 * @brief   Write a character to the console
 */
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

/**
 * @brief Print a number to the serial console
 */
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

/**
 * @brief   Print to the serial console. only understands \%d, \%x, \%p, \%s.
 */
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