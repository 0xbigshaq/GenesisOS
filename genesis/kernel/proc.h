/**
 * @file proc.h
 * @brief Kernel Task/Processe Management.
 */
#ifndef _PROC_GENESIS_H_
#define _PROC_GENESIS_H_

#include "kernel/interrupts.h"
#include "kernel/types.h"
#include "kernel/mmu.h"
#include "kernel/device.h"
#include "kernel/file.h"

#define N_SEG   0x10
#define N_PROCS 0x10
#define N_CPUS  1
#define NOFILE  16 

enum proc_state {
  UNUSED,         //!< The process is not in use
  FRESH,          //!< The process is fresh and ready to run
  RUNNABLE,       //!< The process is ready to run
  RUNNING,        //!< The process is running
  HANG,           //!< The process is waiting for an event
};

typedef struct task {
    trap_ctx_t *trapframe;
    cpu_context_t *ctx;
    pte *pgdir;
    char *kstack;
    enum proc_state state;
    uint32_t channel;
    uint32_t pid;
    file_t ofile[NOFILE];
    volatile uint in_yield;
} task_t;

extern task_t proc_tbl[N_PROCS];
extern seg_desc_t segments_tbl[N_SEG];

/**
 * @brief Per CPU state (note: we only have one CPU for now)
 */
typedef struct cpu {
  cpu_context_t *scheduler;   //!< The state where the scheduler loop was last saved. Used in `sched()`
  task_state_t ts;            //!< Used by x86 to find stack for interrupt
  seg_desc_t gdt[N_SEG];      //!< x86 global descriptor table
  task_t *proc;               //!< The process running on this cpu or null
} cpu_t;

extern cpu_t cpus[N_CPUS];


task_t* alloc_task(void);
void run_init(void);
task_t* cur_proc(void);
cpu_t* cur_cpu(void);
extern void trap_ret(void); // part of trap_entry.S
int alloc_fd(task_t *proc);
void sleep(uint32_t channel);
void wakeup(uint32_t channel);

#endif // _PROC_GENESIS_H_