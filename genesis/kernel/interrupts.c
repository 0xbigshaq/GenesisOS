#include "kernel/interrupts.h"
#include "kernel/vm.h"
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

static void dump_eflags(uint32_t eflags) {
    if(eflags & FL_IF) { kprintf("IF "); }
    kprintf("IOPL=%d ",eflags & FL_IOPL);
    if(eflags & FL_NT) { kprintf("NT "); }
    if(eflags & FL_RF) { kprintf("RF "); }
    if(eflags & FL_VM) { kprintf("VM "); }
    if(eflags & FL_AC) { kprintf("AC "); }
    if(eflags & FL_VIF) { kprintf("VIF "); }
    if(eflags & FL_VIP) { kprintf("VIP "); }
    if(eflags & FL_ID) { kprintf("ID "); }
}

static void print_crash(trap_ctx_t* ctx) {
    // impl
    char *reason;

    switch(ctx->vector_idx) {
        case 0x0: reason ="Division by Zero"; break;
        case 0x1: reason ="Debug"; break;
        case 0x2: reason ="NMI"; break;
        case 0x3: reason ="Breakpoint"; break;
        case 0x4: reason ="Overflow"; break;
        case 0x5: reason ="Bound Range Exceeded"; break;
        case 0x6: reason ="Invalid Opcode"; break;
        case 0x7: reason ="Device Not Available"; break;
        case 0x8: reason ="Double Fault"; break;
        case 0xA: reason ="Invalid TSS"; break;
        case 0xB: reason ="Segment Not Present"; break;
        case 0xC: reason ="Stack-Segment Fault"; break;
        case 0xD: reason ="General Protection Fault"; break;
        case 0xE: reason ="Page Fault"; break;
        case 0x10: reason = "Invalid TSS"; break;
        case 0x11: reason = "Alignment Check"; break;
        case 0x12: reason = "Machine Check"; break;
        case 0x13: reason = "Floating-Point Exception"; break;
        case 0x14: reason = "Virtualization Exception"; break;
        case 0x15: reason = "Control Protection Exception"; break;
        case 0x1C: reason = "Hypervisor Injection Exception"; break;
        case 0x1D: reason = "VMM Communication Exception"; break;
        case 0x1E: reason = "Security Exception"; break;
        default:
            reason = "Unknown";
            break;
    }
    kprintf( COLOR_RED "\nCPU Exception triggered" COLOR_RESET);
    kprintf("\nReason: %s", reason);
    kprintf("\ntrapframe = {");

    // general purpose
    kprintf("\n edi = 0x%x,", ctx->edi);
    kprintf("\n esi = 0x%x,", ctx->esi);
    kprintf("\n ebp = 0x%x,", ctx->ebp);
    kprintf("\n oesp = 0x%x,", ctx->oesp);
    kprintf("\n ebx = 0x%x,", ctx->ebx);
    kprintf("\n edx = 0x%x,", ctx->edx);
    kprintf("\n ecx = 0x%x,", ctx->ecx);
    kprintf("\n eax = 0x%x,", ctx->eax);
    kprintf("\n esp = 0x%x,", ctx->esp);
    kprintf("\n eip = 0x%x,", ctx->eip);
    kprintf("\n eflags = 0x%x ", ctx->eflags);
    kprintf("[ "); dump_eflags(ctx->eflags); kprintf("]"); 

    // segment regs
    kprintf("\n gs = 0x%x,", ctx->gs);
    kprintf("\n fs = 0x%x,", ctx->fs);
    kprintf("\n es = 0x%x,", ctx->es);
    kprintf("\n ds = 0x%x,", ctx->ds);
    kprintf("\n ss = 0x%x,", ctx->ss);
    kprintf("\n cs = 0x%x,", ctx->cs);

    // more info
    kprintf("\n vector_idx = 0x%x,", ctx->vector_idx);
    kprintf("\n err = 0x%x,", ctx->err);
    kprintf("\n}");

    cli();
    while(1) { /* spin*/ }
}
void handle_trap(trap_ctx_t* ctx)
{
    if(ctx->vector_idx >= 0 && ctx->vector_idx <= 21) {
        print_crash(ctx);
    }

    if(ctx->vector_idx == INT_SYSCALL) {
        sys_dispatch();
        return ;
    }

    if(ctx->vector_idx == IRQ_TIMER) {
        // kprintf("[*] IRQ_TIMER triggered\n");
        if(cur_proc() && cur_proc()->state == RUNNING) {
            dmsg("calling yield, kernel_pgtbl = 0x%x", kernel_pgtbl);
            yield();
        }
    }


    if(ctx->vector_idx == IRQ_PS2_MOUSE) {
        handle_mouse_irq();
    }

    if(ctx->vector_idx == IRQ_KEYBOARD) {
        keyboard_handle_irq();
    }

    pic_ack(ctx->vector_idx);
}