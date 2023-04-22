#pragma once
#include "types.h"
/* 
 * Intel SDM Vol. 3, Section 6.10 - "Interrupt Descriptor Table (IDT)"
 * Figure 6-2. IDT Gate Descriptors
*/
typedef struct idt_entry_ {
    uint isr_lo   : 16; // lower bits of isr addr
    uint seg_sel  : 16; // Segment selector (code seg)
    uint reserved : 8;  // let intel be intel
    uint gtype    : 4;  // Gate type(Interrupt, Trap, Task)
    uint stype    : 1;  // Segment type(Gate=0, Code/Data=1)
    uint dpl      : 2;  // Ring
    uint present  : 1;  // Present flag
    uint isr_hi   : 16; // higher bits of isr addr
} __attribute__((packed)) idt_entry_t;


/* 
 * Intel SDM Vol. 3, Section 6.11 - "IDT Descriptors"
 * Figure 6-1. Relationship of the IDTR and IDT
*/
typedef struct idt_reg {
    ushort limit : 16;
    uint base : 32;
} __attribute__((packed)) idt_reg_t;


/* 
 * Intel SDM Vol. 3, Section 6.12.1 - "Exception- or Interrupt-Handler Procedures"
 * Figure 6-3. Interrupt Procedure Call
 * 
 * Interrupt frame: passed to the Interrupt Service Routines
 * We write the elements in reversed order 
*/
typedef struct interrupt_frame_ {
    uint eip : 32;
    uint cs : 32;
    uint eflags : 32;
    uint esp : 32;
    uint ss : 32;
} __attribute__((packed)) interrupt_frame_t;


/*
 * defs
*/
extern idt_entry_t idt[256];  // IDT Entries
extern idt_reg_t idtr;        // This is loaded to the IDT Register

// System segment type bits
#define INT_GATE     0xE     // 32-bit Interrupt Gate
#define TRAP_GATE    0xF     // 32-bit Trap Gate

// Default exception handler (no error code)
// We use the `interrupt` gcc attribute
// https://gcc.gnu.org/onlinedocs/gcc-9.4.0/gcc/x86-Function-Attributes.html#interrupt
__attribute__((interrupt)) void default_excp_handler_wo(interrupt_frame_t* frame);
__attribute__((interrupt)) void default_excp_handler(interrupt_frame_t* frame, uint error_code);
__attribute__((interrupt)) void default_int_handler(interrupt_frame_t* frame);

// setter(s) for the `idt[]` array
void set_idt_entry(uint idx, void* isr, uint flags);
void setup_idt(void);