#pragma once
#include "types.h"
#include "mmu.h"
#include "proc.h"

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
void init_segmentation();
void set_segdesc(seg_desc_t* seg_desc, uint limit, uint base, uint access, uint dpl, uint flags);

void switch_user_vm(task_t *p);
pte* map_kernel_vm(void);
void init_userland_vm(pte *pgdir, char *init, uint sz);