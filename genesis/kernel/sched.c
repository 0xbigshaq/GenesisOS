#include "kernel/sched.h"
#include "kernel/interrupts.h"
#include "kernel/proc.h"
#include "kernel/types.h" // IWYU pragma: keep
#include "kernel/gfx/gui.h"
#include "kernel/vm.h"
#include "kernel/x86.h"

void scheduler(void) {
    task_t *p = NULL;

     for(;;) {
        push_cli();
        render_gui();
        pop_cli();
        for(int i = 0; i < N_PROCS; i++) {
            p = &proc_tbl[i];
            if(p->state != RUNNABLE)
                continue;

            p->state = RUNNING;
            switch_user_vm(p);
            ctx_switch(&(cur_cpu()->scheduler), p->ctx);
            switch_kernel_vm();
        }
    }
}


void sched(void) {
    ctx_switch(&(cur_proc()->ctx), cur_cpu()->scheduler);
}


void yield(void) {
    cur_proc()->state = RUNNABLE;
    sched();
}


void __attribute__((naked)) ctx_switch(cpu_context_t** old, cpu_context_t* new_) {
  asm volatile (
    ".intel_syntax noprefix\n"

    "mov eax, [esp+4]\n"    // put the address of the 1st arg `old` into eax
    "mov edx, [esp+8]\n"    // put the address of the 2nd arg `new` into edx

    // Save old callee-saved registers
    "push ebp\n"
    "push ebx\n"
    "push esi\n"
    "push edi\n"

    // Switch stacks
    "mov [eax], esp\n"
    "mov esp, edx\n"

    // Load new callee-saved registers
    "pop edi\n"
    "pop esi\n"
    "pop ebx\n"
    "pop ebp\n"
    "ret\n"

    ".att_syntax\n"  // Restore AT&T syntax if needed
  );
}