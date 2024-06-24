.intel_syntax noprefix
#include "mmu.h"
#include "memlayout.h"

# Multiboot2 header.  Data to direct multiboot loader.
.p2align 4 # HOT-PATCH: prev: 2 `.p2align 2`
.text
.globl multiboot2_header
multiboot2_header:
header_start:
    .int 0xe85250d6 # multiboot2 magic bytes 
    .int 0 # protected mode i386 
    .int header_end - header_start # header length 
    .int 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start) ) # checksum

    # end tag 
    .long 0 
    .long 0
    .int 8
header_end:

.globl _start
_start = (entry)

# Entering boot process, with paging off.
.globl _start
_start:
  # spin(for gdb)
  # je _start

  # Turn on page size extension for 4Mbyte pages
  mov    eax, cr4
  or     eax, CR4_PSE
  mov    cr4, eax

  # Set page directory
  lea    eax, _virt_to_phys(entrypgdir)
  mov    cr3, eax

  # Turn on paging.
  mov    eax, cr0
  or     eax, (CR0_PG|CR0_WP)
  mov    cr0, eax

  # Set up the stack pointer.
  lea esp, (stack+(KSTACKSIZE*2))

  # Jump to kmain()
  lea eax, kmain
  jmp eax

# reserving memory for stack
.p2align 4
stack_top:
.comm stack, KSTACKSIZE*2


# 32bit testing(TODO: delete this)
testing:
  .word 0x4141        # word is 2 bytes
  .long 0x42424242    # long is 4 bytes

