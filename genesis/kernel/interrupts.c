#include "kernel/interrupts.h"
#include "kernel/x86.h"
#include "kernel/mmu.h"
#include "kernel/pic.h"
#include "kernel/sched.h"
#include "kernel/proc.h"
#include "kernel/syscall.h"
#include "drivers/uart.h" // IWYU pragma: keep
#include "drivers/console.h"
#include "drivers/mouse.h"
#include "drivers/keyboard.h"

idt_entry_t idt[256];  // IDT Entries
idt_reg_t idtr;        // This is loaded to the IDT Register


void set_idt_entry(uint idx, void* isr, char flags)
{
    idt_entry_t *desc = &idt[idx];
    // Interrupt's addr
    desc->isr_lo = ((uint)isr & 0xFFFF);
    desc->isr_hi = ((uint)isr >> 16);
    desc->seg_sel = (SEGMENT_KERNEL_CODE*0x8);

    // let intel be intel
    desc->reserved = 0;

    // attributes (8 bits)
    desc->gtype     = (flags >> 4);
    desc->stype     = (flags >> 3) & 1;
    desc->dpl       = (flags >> 1) & 3;
    desc->present   = 1;
}

void setup_idt(void) {
    idtr.limit = sizeof(idt);
    idtr.base  = (uint)&idt[0];

    for(uint idx = 0; idx<256; idx++) {
        set_idt_entry(idx, (void*)vectors[idx], (char)TRAP_GATE|DPL_KERNEL);
    }
    set_idt_entry(INT_SYSCALL, (void*)vectors[INT_SYSCALL], (char)INT_GATE|TRAP_USER); // for syscalls (will be useful when implementing userland)

    // enable & load IDT
    lidt(idt, sizeof(idt));
    sti();
}

void handle_trap(trap_ctx_t* ctx)
{
    if(ctx->vector_idx >= 0 && ctx->vector_idx <= 21) {
        kprintf("\n[!] KERNEL/CPU EXCEPTION %d at 0x%p\n", ctx->vector_idx, ctx->eip);
        while(1) { }
    }

    if(ctx->vector_idx == INT_SYSCALL) {
        sys_dispatch();
        return ;
    }

    if(ctx->vector_idx == IRQ_TIMER) {
        // kprintf("[*] IRQ_TIMER triggered\n");
        if(cur_proc() && cur_proc()->state == RUNNING) {
            yield();
        }
    }

    if(ctx->vector_idx == IRQ_PS2_MOUSE) {
        handle_mouse_irq();
    }

    if(ctx->vector_idx == IRQ_KEYBOARD) {
        handle_keyboard_irq();
    }

    pic_ack(ctx->vector_idx);
}