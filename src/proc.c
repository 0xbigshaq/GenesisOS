#include "proc.h"
#include "kmalloc.h"
#include "memlayout.h"
#include "vm.h"
#include "sched.h"
#include "fat32.h"
#include <elf.h>

// char init[] = "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x8B\x1D\xF8\xEF\x3B\x80\x90\x90\x90\x90";
// char init[] = "\x90\x90\x90\x90\x90\xEB\xFC\x90\x90\x90";

// syscall in a loop
// char init[] = "\x90\x90\x90\x90\xB8\x01\x00\x00\x00\xCD\x40\x90\x90\xEB\xF4\x90";

// syscall outside of a loop
// char init[] = "\x90\x90\x90\x90\xB8\x01\x00\x00\x00\xCD\x40\x90\x90\xEB\xFC\x90";

// with stack
// char init[] = "\x90\xBC\x00\x45\x00\x00\x89\xE5\x6A\x03\x6A\x04\x6A\x05\x90\x90\x90\xB8\x01\x00\x00\x00\xCD\x40\x90\x90\xEB\xFC\x90";

// with stack + SYS_write syscall
// char init[] = "\x90\xBC\x00\x45\x00\x00\x89\xE5\x68\x29\x20\x20\x20\x68\x73\x21\x20\x3A\x68\x77\x6F\x72\x6B\x54\x6A\x00\x90\x90\x90\xB8\x01\x00\x00\x00\xCD\x80\x90\x90\xEB\xFC\x90";

// SYS_read syscall
// char init[] ="\x90\x90\x90\x89\xE5\x6A\x08\x68\x00\x45\x00\x00\x6A\x00\xB8\x00\x00\x00\x00\xCD\x80\x90\x90\x90\x68\x00\x45\x00\x00\x6A\x01\xB8\x01\x00\x00\x00\xCD\x80\x90\x90\x90\xEB\xFC\x90\x90";

// same as bove but in a loop
char init[] ="\x90\x90\x90\x89\xE5\x90\xE8\x02\x00\x00\x00\xEB\xF8\x89\xE5\x6A\x08\x68\x00\x45\x00\x00\x6A\x00\xB8\x00\x00\x00\x00\xCD\x80\x90\x68\x00\x45\x00\x00\x6A\x01\xB8\x01\x00\x00\x00\xCD\x80\x90\x83\xC4\x14\xC3";

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
    p->trapframe->eflags = FL_IF;
    p->trapframe->eax = 0xdeadbeef;
    p->trapframe->esp = 0x4f00;
    p->trapframe->eip = 0x4000;
    init_userland_vm(p->pgdir, init, sizeof(init));
    switch_user_vm(p);

    p->state = RUNNABLE;
    kprintf("[~] Jumping to userland\n---\n");
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
