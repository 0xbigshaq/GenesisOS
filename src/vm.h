#pragma once
#include "types.h"

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