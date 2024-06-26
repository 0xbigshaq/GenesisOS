
#include "kernel/kmalloc.h"
#include "kernel/types.h"
#include "kernel/mmu.h"
#include "kernel/memlayout.h"
#include "kernel/x86.h"

#include <stddef.h>
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


char* kernel_heap_start;
size_t kernel_heap_size;

void* morecore(ptrdiff_t increment) {
    static char* heap_end = NULL;
    static char* heap_limit = NULL;

    if (heap_end == NULL) {
        heap_end = kernel_heap_start;
        heap_limit = kernel_heap_start + kernel_heap_size;
    }

    if (increment == 0) {
        return heap_end;
    }

    if (increment > 0) {
        if (heap_end + increment > heap_limit) {
            // Not enough memory
            return (void*)-1;
        }

        char* old_heap_end = heap_end;
        heap_end += increment;
        return old_heap_end;
    }

    // If increment < 0, you need to implement memory release (optional)
    return (void*)-1;
}

// Define the MORECORE macro to your morecore function
#define MORECORE morecore
#define MORECORE_CONTIGUOUS 1
#define ABORT abort
#define NO_MALLOC_STATS 1

// Disable mmap
#define HAVE_MMAP 0
#define HAVE_MREMAP 0

// -----
#include <time.h>

time_t time(time_t *tloc) {
    // Return a dummy time or implement your own time function
    time_t current_time = 0; // Example: always return 0
    if (tloc) {
        *tloc = current_time;
    }
    return current_time;
}

// -----
// #define _SC_PAGESIZE 0x1000

long sysconf(int name) {
    if (name == 0x1000) {
        // Return your system's page size
        return 4096; // Example page size
    }
    return -1;
}

int* __errno_location() {
    static int errno;
    return &errno;
}

#include "kernel/external/dlmalloc.c"