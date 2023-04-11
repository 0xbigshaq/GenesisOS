
#include "kmalloc.h"
#include "types.h"
#include "mmu.h"
#include "memlayout.h"
#include "x86.h"

extern char kern_end[]; // first address after kernel loaded from ELF file
                   // defined by the kernel linker script in kernel.ld

kpool_t kpool;

void kmalloc_init(void *virt_start, void *virt_end)
{
    free_range(virt_start, virt_end);
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
