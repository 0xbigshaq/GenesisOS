#include "types.h"
#include "memlayout.h"
#include "mmu.h"
#include "linker.h"
#include "vm.h"
#include "kmalloc.h"
#include "x86.h"

vmmap kern_vmmap[] = {
    { VIRTBASE,   virt_to_phys(VIRTBASE),   KBASE_PHYS,           PTE_W|PTE_P }, /*  IO Devices */
    { KBASE_VIRT, virt_to_phys(KBASE_VIRT), virt_to_phys(data),   PTE_P       }, /*  .text and .rodata */
    { (uint)data, virt_to_phys(data),       TOPMEM_PHYS,          PTE_W|PTE_P }, /*  .data */
    { DEVSPACE,   DEVSPACE,                 WRAPAROUND,           PTE_W|PTE_P }, /*  MMIO/Devices */
};

seg_desc_t segments_tbl[0x10];

/**
 * The `init_kernelvm` function enables a larger virtual address space
 * 
 * Before, when we use the bootstrap `entrypgdir` pagetable:
 * gef➤  monitor info mem
 * 0000000000000000-0000000000400000 0000000000400000 -rw
 * 0000000080000000-0000000080400000 0000000000400000 -rw
 * 
 * After we call `init_kernelvm()`:
 * gef➤  monitor info mem
 * 0000000080000000-0000000080100000 0000000000100000 -rw   <---- VIRTBASE    0x100000
 * 0000000080100000-0000000080102000 0000000000002000 -r-   <---- KBASE_VIRT .text and .rodata
 * 0000000080102000-000000008e000000 000000000defe000 -rw-  <---- .data and heap
 * 00000000fe000000-0000000100000000 0000000002000000 -rw   <---- Devices/MMIO Regions
*/

void init_kernelvm(void) {
    init_segmentation();

    pte* pml2 = kmalloc();
    memset(pml2, NULL, PAGESIZE);
    vmmap* cur_vmmap = NULL;

    for(int idx = 0; idx < NELEM(kern_vmmap); idx++) {
        cur_vmmap = &kern_vmmap[idx];
        gen_ptes(pml2, cur_vmmap->vaddr, (cur_vmmap->pa_end - cur_vmmap->pa_begin), cur_vmmap->pa_begin, cur_vmmap->perms);
    }

    // load new pgtable to cr3 
    lcr3(virt_to_phys(pml2));
}

void gen_ptes(pte *pgdir, uint vaddr, uint size, uint paddr, int perms) {
    uint cur_entry  = PAGE_ROUNDUP(vaddr);
    uint last_entry = PAGE_ROUNDDOWN(vaddr + size - 1);

    while(1) {
        pte* pml1 = pte_resolve(pgdir, (void*)cur_entry, perms, 1);
        *pml1 = (paddr | PTE_P | perms);

        if(cur_entry == last_entry)
            break;

        cur_entry += PAGESIZE;
        paddr     += PAGESIZE;
    }
    return ;
}

pte* pte_resolve(pte *pgdir, void *vaddr, int perms, int should_alloc) {
    pte* pml2_entry = &pgdir[PDX(vaddr)];
    pte* pml1_table = NULL;

    // if already exists:
    // de-reference PML2 to reach PML1 and return the entry of the physical address
    if(*pml2_entry & PTE_P) {
        pml1_table = (pte*)phys_to_virt(PTE_ADDR(*pml2_entry));
    }

    // if doesn't exist: 
    // allocate PML1 pagetable
    else if(should_alloc) {
        pml1_table = kmalloc();
        if(pml1_table == NULL) {
            PANIC("kmalloc-oof!");
        }
        memset(pml1_table, NULL, PAGESIZE);
        *pml2_entry = (virt_to_phys(pml1_table) | PTE_P | perms);
    }

    if(pml1_table == NULL) {
        PANIC("pte_resolve");
    }

    return &pml1_table[PTX(vaddr)];
}

void init_segmentation() {
    set_segdesc(&segments_tbl[SEGMENT_KERNEL_CODE], 0xffffffff, 0x0, STA_X|STA_R|STA_SYSTEM, DPL_KERNEL);
    set_segdesc(&segments_tbl[SEGMENT_KERNEL_DATA], 0xffffffff, 0x0, STA_W|STA_SYSTEM,       DPL_KERNEL);

    set_segdesc(&segments_tbl[SEGMENT_USER_CODE], 0xffffffff, 0x0, STA_X|STA_R, DPL_USER);
    set_segdesc(&segments_tbl[SEGMENT_USER_DATA], 0xffffffff, 0x0, STA_W,       DPL_USER);

    // asm volatile("sgdt (%esp)");
    lgdt((seg_desc_t*)virt_to_phys(segments_tbl), sizeof(segments_tbl));
    // asm volatile("sgdt (%esp)");

    asm volatile("xor %eax, %eax");
    asm volatile("mov $0x10, %ax");

    asm volatile("mov %ax, %fs");
    asm volatile("mov %ax, %ds");
    asm volatile("mov %ax, %es");
    asm volatile("mov %ax, %ss");
    asm volatile("mov %ax, %gs");

    asm volatile("mov $0x8, %ax");
    // asm volatile("mov %ax, %cs");
    asm volatile("ljmp $0x8,$dummylabel\n" // we're doing this because `mov cs, 0x8` is not a valid instruction
                "dummylabel:\n"
                "  nop\n" // some NOPs for good luck <fingers-crossed.png>
                "  nop\n");
    
    return ;
}

void set_segdesc(seg_desc_t* seg_desc, uint limit, uint base, uint access, uint dpl) {
    // base 
    seg_desc->base_lo   = (base  & 0xFFFF);
    seg_desc->base_mid  = (base >> 0x10) & 0xFF;
    seg_desc->base_hi   = (base >> 0x18) & 0xFF;

    // size/limit
    seg_desc->limit_lo  = (limit & 0xFFFF);
    seg_desc->limit_hi  = (limit >> 0x10);

    // access 
    seg_desc->accessed  = 0; // better off to let the CPU set this bit
    seg_desc->rw_bit    = (access >> 1) & 1;
    seg_desc->direction = (access >> 2) & 1;
    seg_desc->execute   = (access >> 3) & 1;
    seg_desc->type      = (access >> 4) & 1;
    seg_desc->dpl       = dpl;
    seg_desc->present   = 1; // We don't need `(access >> 7) & 1;` because if we call
                             // this func our intention is to make it present anyways.

    // flags
    seg_desc->reserved    = 0;
    seg_desc->longmode    = 0;
    seg_desc->seg_size    = 1;
    seg_desc->granularity = 1;
    return ;
}