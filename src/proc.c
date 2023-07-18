#include "proc.h"
#include "kmalloc.h"
#include "memlayout.h"
#include "vm.h"
#include "sched.h"

char init[] = "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x8B\x1D\xF8\xEF\x3B\x80\x90\x90\x90\x90";

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
    p->pgdir = map_kernel_vm();

    p->trapframe->gs = (SEGMENT_USER_DATA << 3) | DPL_USER;
    p->trapframe->fs = (SEGMENT_USER_DATA << 3) | DPL_USER;
    p->trapframe->es = (SEGMENT_USER_DATA << 3) | DPL_USER;
    p->trapframe->ds = (SEGMENT_USER_DATA << 3) | DPL_USER;
    p->trapframe->ss = (SEGMENT_USER_DATA << 3) | DPL_USER;
    p->trapframe->cs = (SEGMENT_USER_CODE << 3) | DPL_USER;
    p->trapframe->eflags = FL_IF; // [ IOPL=0 IF SF ]
    p->trapframe->eax = 0xdeadbeef; // (uint32_t)&testing_subroutine;
    p->trapframe->esp = 0xaabbccdd; // (uint32_t)&testing_subroutine;
    p->trapframe->eip = 0x4000; // (uint32_t)&testing_subroutine;
    init_userland_vm(p->pgdir, init, sizeof(init));
    switch_user_vm(p);

    p->state = RUNNABLE;
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
