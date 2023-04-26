#include "interrupts.h"
#include "uart.h"
#include "x86.h"
#include "mmu.h"
#include "console.h"
#include "pic.h"

idt_entry_t idt[256];  // IDT Entries
idt_reg_t idtr;        // This is loaded to the IDT Register


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

    for(uint idx = 0; idx<256; idx++) {
        set_idt_entry(idx, (void*)vectors[idx], TRAP_GATE|DPL_KERNEL);
    }
    set_idt_entry(0x80, (void*)vectors[0x80], TRAP_GATE|DPL_USER); // for syscalls (will be useful when implementing userland)

    // enable & load IDT
    init_pic();
    lidt(idt, sizeof(idt));
    sti();
}

void handle_trap(trap_ctx_t* ctx)
{
    kprintf("handle_trap() triggered! Interrupt number(ctx->vector_idx) = %d (hex: 0x%x)\n", ctx->vector_idx, ctx->vector_idx);
    pic_ack(ctx->vector_idx);
}