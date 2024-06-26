
#include "kernel/kmalloc.h"
#include "kernel/types.h"
#include "kernel/mmu.h"
#include "kernel/memlayout.h"
#include "kernel/x86.h"
#include <stddef.h>
extern char kern_end[]; // first address after kernel loaded from ELF file
                   // defined by the kernel linker script in kernel.ld

// imported from `kernel/allocator/dlmalloc.h`
extern char* kernel_heap_start;
extern size_t kernel_heap_size;
extern int init_mparams(void);

kpool_t kpool;

void kmalloc_init(void *virt_start, void *virt_end)
{
    free_range(virt_start, virt_end);
    kernel_heap_start = ((char*)virt_end)+0x1000;
    kernel_heap_size = 0x40000;
    init_mparams();
}

void free_range(void *virt_start, void *virt_end)
{
    char *p;
    p = (char*)PAGE_ROUNDUP((uint)virt_start);
    for(; p + PAGESIZE <= (char*)virt_end; p += PAGESIZE)
        kfree(p);
}

void* memset(void *dst, int c, uint n)
{
    stosb(dst, c, n);
    return dst;
}

void kfree(char *addr)
{
    kmalloc_chunk *chunk;

    if(!IS_PALIGN(addr) || addr < kern_end || virt_to_phys(addr) >= TOPMEM_PHYS) {
        PANIC("kfree");
        return ;
    }

    memset(addr, 0x1, PAGESIZE); // for debugging purposes

    chunk = (kmalloc_chunk*)addr;
    APPEND(kpool.freelist, chunk);
}

/*
 * Allocate a page of physical memory.
 * Returns a `kmalloc_chunk*` pointer.
 * Returns 0 if the allocation fails.
 */
void* kmalloc(void)
{
    kmalloc_chunk* chunk;
    chunk = kpool.freelist;
    if(chunk) {
        kpool.freelist = chunk->next;
    }
    return (void*)chunk;
}
