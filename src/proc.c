#include "proc.h"
#include "kmalloc.h"
#include "memlayout.h"

#include "sched.h"

cpu_t cpus[N_CPUS];
task_t proc_tbl[N_PROCS];
uint32_t nextpid = 1;
cpu_t* cur_cpu(void) {
    return &cpus[0];
}

task_t* cur_proc(void) {
    return cur_cpu()->proc;
}

void testing_subroutine(void) {
  while(1) {
    for(int i = 0; i<0xffffff; i++) {
      // nothing
    }
    kprintf("Hello from task! :^)\n");
  }
}
void run_init(void) {
    task_t* p = alloc_task();
    p->state = RUNNABLE;
    p->trapframe->gs = 0x10;
    p->trapframe->fs = 0x10;
    p->trapframe->es = 0x10;
    p->trapframe->ds = 0x10;
    p->trapframe->ss = 0x10;
    p->trapframe->cs = 0x8;
    p->trapframe->eflags = 0x282; // [ IOPL=0 IF SF ]
    p->trapframe->eip = (uint32_t)&testing_subroutine;

    scheduler(); // enter the scheudler loop

}

task_t* alloc_task(void)
{
  task_t *p;
  char *sp;

    for(p = proc_tbl; p < &proc_tbl[N_PROCS]; p++)
        if(p->state == UNUSED)
          goto found;

  return 0;

found:
  p->state = EMBRYO;
  p->pid = nextpid++;

  // Allocate kernel stack.
  if((p->kstack = kmalloc()) == 0){
    p->state = UNUSED;
    return 0;
  }
  sp = p->kstack + KSTACKSIZE;

  // Leave room for trap frame.
  sp -= sizeof *p->trapframe;
  p->trapframe = (trap_ctx_t*)sp;

/*
  // Set up new context to start executing at forkret,
  // which returns to trap_ret.
  sp -= 4;
  *(uint*)sp = (uint)trap_ret;
*/

  sp -= sizeof *p->ctx;
  p->ctx = (cpu_context_t*)sp;
  memset(p->ctx, 0, sizeof *p->ctx);
  p->ctx->eip = (uint)trap_ret; // should be `fork_ret` (?????)

  return p;
}

/*

typedef struct cpu_context {
  uint eax;
  uint ecx;
  uint edx;
  uint ebx;
  uint esp;
  uint ebp;
  uint esi;
  uint edi;
  uint eip;
} cpu_context_t;

*/
