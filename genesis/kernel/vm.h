/**
 * @file vm.h
 * @brief Virtual Memory
 */
#ifndef VM_H
#define VM_H

#include "kernel/types.h"
#include "kernel/mmu.h"
#include "kernel/proc.h"

#define WRAPAROUND 0

typedef struct vmmap_ {
    uint vaddr;
    uint pa_begin;
    uint pa_end;
    uint perms;
} vmmap;

void init_kernelvm(void);
void gen_ptes(pte *pgdir, uint vaddr, uint size, uint paddr, int perms);
pte* pte_resolve(pte *pgdir, void *vaddr, int perms, int should_alloc);
void init_segmentation() __attribute__((noinline));
void set_segdesc(seg_desc_t* seg_desc, uint limit, uint base, uint access, uint dpl, uint flags);

void switch_user_vm(task_t *p);
void switch_kernel_vm();
pte* map_kernel_vm(void);
void init_userland_vm(pte *pgdir, char *init, uint sz);

extern pte* kernel_pgtbl;

#endif  // VM_H