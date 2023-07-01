#pragma once
#include "interrupts.h"
#include "types.h"
#include "mmu.h"

#define N_SEG   0x10
#define N_PROCS 0x10
#define N_CPUS  1

enum procstate { UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };

typedef struct task {
    trap_ctx_t *trapframe; // might be "trap-frame", might also be "the f*ck??". depends on context
    cpu_context_t *ctx;
    pte *pgdir;
    char *kstack;
    enum procstate state;
    uint32_t pid;
} task_t;

extern task_t proc_tbl[N_PROCS];
extern seg_desc_t segments_tbl[N_SEG];

// Per-CPU state
typedef struct cpu {
  cpu_context_t *scheduler;   // swtch() here to enter scheduler
  struct taskstate ts;         // Used by x86 to find stack for interrupt
  seg_desc_t gdt[N_SEG];       // x86 global descriptor table
  int ncli;                    // Depth of pushcli nesting.
  int intena;                  // Were interrupts enabled before pushcli?
  task_t *proc;           // The process running on this cpu or null
} cpu_t;

extern cpu_t cpus[N_CPUS];


task_t* alloc_task(void);
void run_init(void);
task_t* cur_proc(void);
cpu_t* cur_cpu(void);
extern void trap_ret(void); // part of trap_entry.S