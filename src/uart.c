#include "uart.h"

void init_uart()
{
    // Set the baud rate
    unsigned short divisor = 115200 / 9600;
    outb(UART_BASE_ADDR + UART_INTERRUPT_ENABLE_REG_OFFSET, 0x00);
    outb(UART_BASE_ADDR + UART_DLAB_OFFSET, (1 << 7) );             // Enable DLAB
    outb(UART_BASE_ADDR + UART_DIVISOR_LSB_OFFSET, divisor & 0xFF); // lo-byte
    outb(UART_BASE_ADDR + UART_DIVISOR_MSB_OFFSET, divisor >> 8);   // hi-byte
    outb(UART_BASE_ADDR + UART_DATA_BITS_OFFSET, 0x03);             // 8-bit data 

    outb(UART_BASE_ADDR + UART_INTERRUPT_ENABLE_REG_OFFSET, 0x1);

    uart_write("[*] init uart\n");
}


void uart_putchar(char c)
{
    // Wait for the transmit buffer(THRE, located in the 5th bit) to be empty
    while ((inb(UART_BASE_ADDR + UART_LINE_REG_OFFSET) & (1 << 5)) == 0);
    outb(UART_BASE_ADDR + UART_DATA_REG_OFFSET, c);
}

int uart_getchar(void)
{
  if(!(inb(UART_BASE_ADDR+5) & 0x01))
    return -1;
  return inb(UART_BASE_ADDR+0);
}


void uart_write(char* str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        uart_putchar(str[i]);
    }
}