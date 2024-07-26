/**
 * @file pic.h
 * @brief 8259A IRQ chip
 */
#ifndef PIC_H
#define PIC_H
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
void pic_init(void);
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

#endif // PIC_H


/*! \page pic 8259A PIC
 *  \brief Programmable Interrupt Controller

  \tableofcontents

  \note This page is still WIP.

  \section pic-intro Introduction
   The QEMU emulator gives us a 8259A Programmable Interrupt Controller chip.
    The 8259A is a programmable interrupt controller designed for the Intel 8085 and Intel 8086 
    microprocessors.
   \image html "pic-0.png" "8259A PIC" 

  \section pic-list-irqs List of IRQs

This is a standard mapping of IRQs in a typical x86 system:
- **IRQ 0:** System timer
- **IRQ 1:** Keyboard
- **IRQ 2:** Cascaded signals from IRQs 8-15 (used for chaining the master and slave PICs)
- **IRQ 3:** Second serial port (COM2)
- **IRQ 4:** First serial port (COM1)
- **IRQ 5:** Sound card or secondary LPT (parallel) port
- **IRQ 6:** Floppy disk controller
- **IRQ 7:** First parallel port (LPT1)
- **IRQ 8:** Real-time clock (RTC)
- **IRQ 9:** ACPI or redirected IRQ2
- **IRQ 10:** Available for use (often used by network cards)
- **IRQ 11:** Available for use (often used by network cards or other peripherals)
- **IRQ 12:** PS/2 mouse
- **IRQ 13:** FPU (Floating Point Unit) or math coprocessor
- **IRQ 14:** Primary IDE channel
- **IRQ 15:** Secondary IDE channel

  \image html "8259-PIN.png" "8259A PIC Block Diagram" width=400px 

  \subsection pic-subsection1 The main IRQ Handler
  WIP.
  \subsection pic-subsection2 Remapping the IRQs
  WIP.
*/
