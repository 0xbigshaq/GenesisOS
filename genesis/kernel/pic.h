#pragma once
#include "kernel/types.h" // IWYU pragma: keep

// 8259A Software Port Mappings
#define PIC1		0x20		/* base addr for master PIC */
#define PIC2		0xA0		/* base addr for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

// Initialization Control Words 
#define ICW1_ICW4	0x01		/* Indicates that ICW4 will be present */
#define ICW1_INIT	0x10		/* Initialization - required! */
#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
 
// Signals
#define PIC_EOI		0x20		/* End-of-interrupt command code */

// defs
void init_pic(void);
void pic_remap(int pic1_offset, int pic2_offset);
void pic_ack(uint8_t irq);

// IRQ(s). We receive these as (IRQ_OFFSET+n)
#define IRQ_COM1 PIC1 + 0x4
#define IRQ_OFFSET	32
#define IRQ_TIMER       (IRQ_OFFSET+0)
#define IRQ_PS2_MOUSE   (IRQ_OFFSET+12)
#define IRQ_KEYBOARD    (IRQ_OFFSET+1)

// Custom for our kernel
#define INT_SYSCALL     0x80
