/**
 * @file uart.h
 * @brief UART/Serial Driver see https://wiki.osdev.org/Serial_Ports
 */
#ifndef UART_H
#define UART_H
#include "kernel/x86.h"

#define UART_BASE_ADDR 0x3F8     //!< COM2 IO Port number

// I/O port offsets for the UART registers
#define UART_DATA_REG_OFFSET 0
#define UART_INTERRUPT_ENABLE_REG_OFFSET 1
#define UART_DIVISOR_LSB_OFFSET 0
#define UART_DIVISOR_MSB_OFFSET 1

#define UART_LINE_REG_OFFSET 5
#define UART_DATA_BITS_OFFSET 3
#define UART_DLAB_OFFSET 3

void init_uart();
void uart_putchar(char c);
void uart_write(char* str);
int uart_getchar(void);

#endif // UART_H