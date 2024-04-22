// https://wiki.osdev.org/Serial_Ports
#pragma once
#include "kernel/x86.h"

// this is hard-coded for now, because I don't have any plans for GUI 
#define NOGRAPHIC 1 

/*
 * When using the -nographic option, QEMU emulates the serial port as a 16550A UART 
 * connected to the virtual machine's COM1 port, which has a base address of 0x3F8(==COM1). 
 * However, when not using the -nographic option, QEMU instead emulates two serial ports: 
 *  > The first one with a base address of 0x3F8, which is connected to the virtual machine's monitor/console.
 *  > Another one with a base address of 0x2F8(==COM2) that is connected to a virtual serial device.
*/
#ifdef NOGRAPHIC
#define UART_BASE_ADDR 0x3F8
#else
#define UART_BASE_ADDR 0x2F8
#endif

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
