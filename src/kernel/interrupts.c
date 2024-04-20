#include "kernel/interrupts.h"
#include "drivers/uart.h"
#include "kernel/x86.h"
#include "kernel/mmu.h"
#include "drivers/console.h"
#include "kernel/pic.h"
#include "kernel/sched.h"
#include "kernel/proc.h"
#include "kernel/syscall.h"

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
    // if(ctx->vector_idx != 0x20) {
        // kprintf("handle_trap() triggered! Interrupt number(ctx->vector_idx) = %d (hex: 0x%x)\n", ctx->vector_idx, ctx->vector_idx);
    // }

    if(ctx->vector_idx >= 0 && ctx->vector_idx <= 21) {
        kprintf("\n[!] KERNEL/CPU EXCEPTION %d at 0x%p\n", ctx->vector_idx, ctx->eip);
        while(1) { }
    }

    if(ctx->vector_idx == INT_SYSCALL) {
        sys_dispatch();
        return ;
    }

    // if(ctx->vector_idx == IRQ_COM1) {
    //     char ch;
    //     if((ch = uart_getchar()) >= '\0') {
    //         // kprintf("[*] IRQ_COM1 triggered! recv'd: 0x%x", ch);
    //         // uart_putchar(ch);
    //         // uart_putchar('\n');
    //         if(ch == '\r' || ch == '\n') {
    //             kprintf("\n[root@wrld]> ");
    //         } else if (ch == 0x7f) { // Handle backspace
    //             uart_putchar('\b'); uart_putchar(' '); uart_putchar('\b');
    //         }
    //          else {
    //             uart_putchar(ch);
    //         }
    //     }
    // }

    if(ctx->vector_idx == IRQ_TIMER) {
        // kprintf("[*] IRQ_TIMER triggered\n");
        if(cur_proc() && cur_proc()->state == RUNNING) {
            yield();
        }
    }
    pic_ack(ctx->vector_idx);
}