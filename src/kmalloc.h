#pragma once
#include "types.h"
#include "console.h"

#define PANIC(str) puts(0, 0, RED, BLACK, "panic @ " str);

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


typedef struct _kmalloc_chunk {
  struct _kmalloc_chunk *next;
} kmalloc_chunk;

struct {
  int use_lock;
  kmalloc_chunk *freelist;
} kpool;