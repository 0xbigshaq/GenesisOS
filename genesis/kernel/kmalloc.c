
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

/**
 * @brief Initializes the kernel memory allocator.
 * @details Initializes the kernel heap and prepares the free list of pages.
 * @param virt_start The virtual start address of the heap.
 * @param virt_end The virtual end address of the heap.
 */
void kmalloc_init(void *virt_start, void *virt_end)
{
    free_range(virt_start, virt_end);
    kernel_heap_start = ((char*)virt_end)+0x1000;
    kernel_heap_size = 0x20000000;
    init_mparams();
}

/**
 * @brief Frees a range of virtual memory.
 * @details Marks a range of virtual memory as free by adding each page in the range to the free list.
 * @param virt_start The virtual start address of the range.
 * @param virt_end The virtual end address of the range.
 */
void free_range(void *virt_start, void *virt_end)
{
    char *p;
    p = (char*)PAGE_ROUNDUP((uint)virt_start);
    for(; p + PAGESIZE <= (char*)virt_end; p += PAGESIZE)
        kfree(p);
}

/**
 * @brief Sets a block of memory to a specified value.
 * @details Fills the first `n` bytes of the memory area pointed to by `dst` with the constant byte `c`.
 * @param dst A pointer to the memory area to be filled.
 * @param c The byte value to set.
 * @param n The number of bytes to be set to the value.
 * @return A pointer to the memory area `dst`.
 */
void* memset(void *dst, int c, uint n)
{
    stosb(dst, c, n);
    return dst;
}

/**
 * @brief Frees a page of memory.
 * @details Adds the page at the specified address to the free list.
 * @param addr The address of the page to be freed.
 */
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

/**
 * @brief Allocates a page of physical memory.
 * @details Allocates a single page of physical memory and returns a pointer to it.
 * @return A pointer to the allocated page, or 0 if the allocation fails.
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

/**
 * @brief Allocates multiple pages of physical memory.
 * @details Allocates the specified number of pages and returns a pointer to the first page.
 * @param sz The size in bytes of the memory to be allocated.
 * @return A pointer to the allocated memory.
 */
void* kmalloc_pages(uint32_t sz) {
    void *p = kmalloc();
    uint32_t len = 0;
    while(len < sz) {
        kmalloc();
        len += PAGESIZE;
    }
    return p;
}