#include "interrupts.h"
#include "uart.h"
#include "x86.h"
#include "mmu.h"

idt_entry_t idt[256];  // IDT Entries
idt_reg_t idtr;        // This is loaded to the IDT Register


__attribute__((interrupt)) void default_excp_handler(interrupt_frame_t* frame, uint error_code)
{
    uart_write("\n[*] default_excp_handler called!\n");
    while(1) { } 
}


__attribute__((interrupt)) void default_excp_handler_wo(interrupt_frame_t* frame)
{
    uart_write("\n[*] default_excp_handler_wo called!\n");
    while(1) { } 
}


__attribute__((interrupt)) void default_int_handler(interrupt_frame_t* frame)
{
    uart_write("\n[*] default_int_handler called!\n");
    while(1) { } 
}

void set_idt_entry(uint idx, void* isr, uint flags)
{
    idt_entry_t *desc = &idt[idx];
    // Interrupt's addr
    desc->isr_lo = ((uint)isr & 0xFFFF);
    desc->isr_hi = ((uint)isr >> 16);
    desc->seg_sel = (SEGMENT_KERNEL_CODE*0x8);

    // let intel be intel
    desc->reserved = 0;

    // attributes
    desc->gtype     = flags & 0xF;  // gate type, mask with 0b00001111
    desc->stype     = 0;            // system segment, always 0 on gate descriptors
    desc->dpl       = flags & 0x60; // DPL, mask with 0b01100000
    desc->present   = 1;
}

void setup_idt(void) {
    idtr.limit = sizeof(idt);
    idtr.base  = (uint)&idt[0];

    // setup exception handlers(ISRs 0-32)
    for(uint idx = 0; idx < 32; idx++) {
        if( (idx >= 8 && idx<=17) || idx==21) {
            // Exception takes an error code
            set_idt_entry(idx, &default_excp_handler, TRAP_GATE|DPL_KERNEL);
        } else {
            // Exception is without an error code
            set_idt_entry(idx, &default_excp_handler_wo, TRAP_GATE|DPL_KERNEL);
        }
    }

    // Setup regular interrupts (ISRs 32-255)
    for(uint idx = 32; idx < 256; idx++) {
        set_idt_entry(idx, &default_int_handler, INT_GATE|DPL_KERNEL);
    }

    // load 
    __asm__ __volatile__ ("sti");
    // __asm__ __volatile__ ("lidt %0" : : "memory"(idtr));
    lidt(idt, sizeof(idt));
}