#pragma once
#include "kernel/types.h"
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


typedef struct trap_ctx_ {
  // registers as pushed by pusha
  uint32_t edi;
  uint32_t esi;
  uint32_t ebp;
  uint32_t oesp;      // useless & ignored
  uint32_t ebx;
  uint32_t edx;
  uint32_t ecx;
  uint32_t eax;

  // rest of trap frame
  uint16_t gs;
  uint16_t padding1;
  uint16_t fs;
  uint16_t padding2;
  uint16_t es;
  uint16_t padding3;
  uint16_t ds;
  uint16_t padding4;
  uint32_t vector_idx;

  /* 
  * Intel SDM Vol. 3, Section 6.12.1 - "Exception- or Interrupt-Handler Procedures"
  * Figure 6-3. Interrupt Procedure Call
  * 
  * Interrupt frame: passed to the Interrupt Service Routines
  * We write the elements in reversed order 
  */
  uint32_t err;
  uint32_t eip;
  uint16_t cs;
  uint16_t padding5;
  uint32_t eflags;

  // below here only when crossing rings, such as from user to kernel
  uint32_t esp;
  uint16_t ss;
  uint16_t padding6;
} trap_ctx_t;

typedef struct cpu_context {
  uint32_t edi;
  uint32_t esi;
  uint32_t ebx;
  uint32_t ebp;
  uint32_t eip;
} cpu_context_t;

/*
 * defs & func prototypes
*/

extern idt_entry_t idt[256];  // IDT Entries
extern idt_reg_t idtr;        // This is loaded to the IDT Register
extern uint vectors[256];     // For the ISR dispatcher

// System segment type bits
#define INT_GATE     (0xE<<4)     // 32-bit Interrupt Gate
#define TRAP_GATE    (0xF<<4)     // 32-bit Trap Gate
#define TRAP_USER    (0x3<<1)     // DPL 

// setter(s) for the `idt[]` array
void set_idt_entry(uint idx, void* isr, char flags);
void setup_idt(void);
void handle_trap(trap_ctx_t* ctx);