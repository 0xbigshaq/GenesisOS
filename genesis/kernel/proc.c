#include "kernel/proc.h"
#include "kernel/kmalloc.h"
#include "kernel/memlayout.h"
#include "kernel/mmu.h"
#include "kernel/vm.h"
#include "kernel/sched.h"
#include "kernel/string.h"
#include "drivers/console.h"
#include "drivers/fat32.h"

#include <elf.h>
#include <stdint.h>

cpu_t cpus[N_CPUS];
task_t proc_tbl[N_PROCS];
uint32_t nextpid = 1;

/**
 * @brief Returns the current CPU.
 * @details This function returns a pointer to the current CPU's structure.
 * @return A pointer to the current CPU's `cpu_t` structure.
 */
cpu_t* cur_cpu(void) {
    return &cpus[0];
}

/**
 * @brief Returns the current process.
 * @details This function returns a pointer to the current process's structure.
 * @return A pointer to the current process's `task_t` structure.
 */
task_t* cur_proc(void) {
    return cur_cpu()->proc;
}

/**
 * @brief Loads the initial ELF file into virtual address space.
 * @details This function sets up the initial process by loading the ELF file, setting up the stack,
 * and preparing the trap frame for switching to user mode during boot.
 */
void run_init(void) {
  kprintf("[*] <run_init> Loading init ELF file into VA.\n");
  task_t* p = alloc_task();
  p->pgdir = map_kernel_vm();
  Elf32_Ehdr *elf_header =  (Elf32_Ehdr*)init_data;
  uint8_t *begin = init_data;
  uint8_t *cursor = init_data;
  Elf32_Phdr *phdr;
  uint8_t *mem;

  // Load PT_LOAD entries into memory
  for (int i = 0; i < elf_header->e_phnum; i++) {
    cursor = &begin[elf_header->e_phoff + (i * sizeof(Elf32_Phdr))];
    phdr = (Elf32_Phdr *)cursor;
    cursor = &begin[phdr->p_offset];

    if (phdr->p_type == PT_LOAD) {
      kprintf(" > PT_LOAD @ 0x%x\n", phdr->p_vaddr);
      mem = kmalloc_pages(phdr->p_memsz);  // FIXME: kmalloc should return adjacent chunks
      memset(mem, NULL, phdr->p_memsz);
      gen_ptes(p->pgdir, (uint)phdr->p_vaddr, (uint)phdr->p_memsz, virt_to_phys(mem), (PTE_W|PTE_U));
      memcpy(mem, cursor, phdr->p_memsz);
    }
  }    
  // set up stack
    mem = malloc(USERLAND_STACK_SIZE);
    memset(mem, NULL, USERLAND_STACK_SIZE);
    gen_ptes(p->pgdir, USERLAND_STACK_ADDR, USERLAND_STACK_SIZE, virt_to_phys(mem), (PTE_W|PTE_U));

    p->trapframe->gs = (SEGMENT_USER_DATA << 3) | DPL_USER;
    p->trapframe->fs = (SEGMENT_USER_DATA << 3) | DPL_USER;
    p->trapframe->es = (SEGMENT_USER_DATA << 3) | DPL_USER;
    p->trapframe->ds = (SEGMENT_USER_DATA << 3) | DPL_USER;
    p->trapframe->ss = (SEGMENT_USER_DATA << 3) | DPL_USER;
    p->trapframe->cs = (SEGMENT_USER_CODE << 3) | DPL_USER;
    p->trapframe->eflags = FL_IF;
    p->trapframe->eax = 0xdeadbeef;
    p->trapframe->esp = USERLAND_STACK_TOP;
    p->trapframe->ebp = USERLAND_STACK_TOP;
    p->trapframe->eip = elf_header->e_entry;
    switch_user_vm(p);

    p->state = RUNNABLE;
    kprintf("[~] Jumping to userland, entry @ 0x%x\n---\n", elf_header->e_entry);
}

/**
 * @brief Allocates a new task.
 * @details This function allocates a new task structure and kernel stack for the task. 
 * It initializes the task's context and file descriptors.
 * @return A pointer to the newly allocated `task_t` structure, or 0 if allocation fails.
 */
task_t* alloc_task(void)
{
  task_t *p;
  char *sp;

    for(p = proc_tbl; p < &proc_tbl[N_PROCS]; p++)
        if(p->state == UNUSED)
          goto found;

  return 0;

found:
  p->state = FRESH;
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

  // setup devices, i/o
  p->ofile[0].type = FD_DEVICE;
  p->ofile[0].devno = DEV_CONSOLE;

  p->ofile[1].type = FD_DEVICE;
  p->ofile[1].devno = DEV_CONSOLE;

  return p;
}

/**
 * @brief Allocates a new file descriptor for a process.
 * @details This function finds an available file descriptor in the process's open file table 
 * and allocates it.
 * @param proc A pointer to the process's `task_t` structure.
 * @return The index of the allocated file descriptor, or -1 if allocation fails.
 */
int alloc_fd(task_t *proc) {
  int fd = -1;

  for(int idx=0; idx<NOFILE; idx++) {
    if(proc->ofile[idx].refcount == 0 && proc->ofile[idx].type == FD_NONE) {
      proc->ofile[idx].refcount++;
      fd = idx;
      break;
    }
  }

  return fd;
}

/**
 * @brief Puts the current process to sleep on a specified channel.
 * @details This function changes the current process's state to `HANG` and sets its channel.
 * It then calls the scheduler to switch to another process.
 * @param channel The channel on which the process will sleep.
 */
void sleep(uint32_t channel) {
  task_t *p = cur_proc();
  p->channel = channel;
  p->state = HANG;
  // dmsg("sleeping on channel %d", channel);
  sched();
  // dmsg("returned from %d", channel);
}

/**
 * @brief Wakes up all processes sleeping on a specified channel.
 * @details This function changes the state of all processes sleeping on the specified channel
 * to `RUNNABLE`.
 * @param channel The channel on which the processes are sleeping.
 */
void wakeup(uint32_t channel) {
  task_t *p;

  for(p = proc_tbl; p < &proc_tbl[N_PROCS]; p++) {
    if(p->state == HANG && p->channel == channel) {
      p->channel = 0;
      p->state = RUNNABLE;
    }
  }
}