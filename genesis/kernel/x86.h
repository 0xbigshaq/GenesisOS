/**
 * @file x86.h
 * @brief inline C x86 instructions.
 */
#ifndef X86_H
#define X86_H

#include "kernel/types.h"
#include "kernel/mmu.h"
#include "kernel/interrupts.h"

static inline uchar inb(ushort port)
{
  uchar data;

  asm volatile("in %1,%0" : "=a" (data) : "d" (port));
  return data;
}

static inline void insl(int port, void *addr, int cnt)
{
  asm volatile("cld; rep insl" :
               "=D" (addr), "=c" (cnt) :
               "d" (port), "0" (addr), "1" (cnt) :
               "memory", "cc");
}

static inline void outb(ushort port, uchar data)
{
  asm volatile("out %0,%1" : : "a" (data), "d" (port));
}

static inline uint16_t inw(uint16_t port) {
    uint16_t result;
    __asm__ volatile("inw %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

static inline void outw(ushort port, ushort data)
{
  asm volatile("out %0,%1" : : "a" (data), "d" (port));
}

static inline void outsl(int port, const void *addr, int cnt)
{
  asm volatile("cld; rep outsl" :
               "=S" (addr), "=c" (cnt) :
               "d" (port), "0" (addr), "1" (cnt) :
               "cc");
}

static inline void stosb(void *addr, int data, int cnt)
{
  asm volatile("cld; rep stosb" :
               "=D" (addr), "=c" (cnt) :
               "0" (addr), "1" (cnt), "a" (data) :
               "memory", "cc");
}

static inline void stosl(void *addr, int data, int cnt)
{
  asm volatile("cld; rep stosl" :
               "=D" (addr), "=c" (cnt) :
               "0" (addr), "1" (cnt), "a" (data) :
               "memory", "cc");
}

static inline void lgdt(seg_desc_t *p, int size)
{
  seg_desc_reg_t descriptor = {
    .limit = (size - 1),
    .ptr   = p
  };

  asm volatile("mov %0, %%eax\n\t"
              "lgdt (%%eax)\n\t"
              :
              : "r" (&descriptor)
              : "eax", "memory");
}


static inline void lidt(idt_entry_t *p, int size)
{
  volatile ushort pd[3];

  pd[0] = size-1;
  pd[1] = (uint)p;
  pd[2] = (uint)p >> 16;

  asm volatile("lidt (%0)" : : "r" (pd));
}

static inline void ltr(ushort sel)
{
  asm volatile("ltr %0" : : "r" (sel));
}

static inline uint readeflags(void)
{
  uint eflags;
  asm volatile("pushfl; popl %0" : "=r" (eflags));
  return eflags;
}

static inline void loadgs(ushort v)
{
  asm volatile("movw %0, %%gs" : : "r" (v));
}

static inline void cli(void)
{
  asm volatile("cli");
}

static inline void sti(void)
{
  asm volatile("sti");
}

static inline uint xchg(volatile uint *addr, uint newval)
{
  uint result;

  // The + in "+m" denotes a read-modify-write operand.
  asm volatile("lock; xchgl %0, %1" :
               "+m" (*addr), "=a" (result) :
               "1" (newval) :
               "cc");
  return result;
}

static inline uint rcr2(void)
{
  uint val;
  asm volatile("movl %%cr2,%0" : "=r" (val));
  return val;
}

static inline void lcr3(uint val)
{
  asm volatile("movl %0,%%cr3" : : "r" (val));
}

static inline uint32_t get_ebx() {
    uint32_t val;
    __asm__ volatile ("movl %%ebx, %0" : "=r"(val));
    return val;
}

#endif  // X86_H