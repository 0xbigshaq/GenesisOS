#pragma once
#include "kernel/types.h"
#include "drivers/console.h"  // IWYU pragma: keep
#include "kernel/allocator/dlmalloc.h" // IWYU pragma: keep

#define PANIC(str) kprintf("panic @ " str); while(1) { /*spin*/ } 

#define APPEND(head, node)    \
{                             \
  node->next = head;          \
  head = node;                \
} \

void kmalloc_init(void *virt_start, void *virt_end);
void free_range(void *virt_start, void *virt_end);
void* memset(void *dst, int c, uint n);
void kfree(char *v);
void* kmalloc(void);
void* kmalloc_pages(uint32_t sz);

// this type is for future allocator
typedef struct _kmalloc_chunk {
  struct _kmalloc_chunk *next;
} kmalloc_chunk;

typedef struct _kpool_t {
  int use_lock;
  kmalloc_chunk *freelist;
} kpool_t;