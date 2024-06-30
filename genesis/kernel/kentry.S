.intel_syntax noprefix
#include "mmu.h"
#include "memlayout.h"
#define MULTIBOOT_HEADER_MAGIC                 0xe85250d6
#define MULTIBOOT_HEADER_ARCHITECTURE          0
#define MULTIBOOT_HEADER_ALIGN                 8
#define MULTIBOOT_TAG_ALIGN                    8

#define MULTIBOOT_HEADER_TAG_FRAMEBUFFER       5
#define MULTIBOOT_TAG_TYPE_FRAMEBUFFER         8
#define MULTIBOOT_HEADER_TAG_END               0

.section .multiboot2_header
.align MULTIBOOT_HEADER_ALIGN
.text
.globl multiboot2_header
multiboot2_header:
header_start:
    .int MULTIBOOT_HEADER_MAGIC               # Magic number
    .int MULTIBOOT_HEADER_ARCHITECTURE        # Architecture
    .int header_end - header_start            # Header length
    .int -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_ARCHITECTURE + (header_end - header_start)) # Checksum

    .align MULTIBOOT_TAG_ALIGN
    tag_framebuffer:
      .long MULTIBOOT_HEADER_TAG_FRAMEBUFFER  # Framebuffer request tag type
      .long 20                                # Size of the tag (including this header)
      .int 1600                                # Width
      .int 900                                # Height
      .int 32                                 # Bits per pixel
    tag_framebuffer_end:

    .align MULTIBOOT_TAG_ALIGN
    tag_terminator:
      .long MULTIBOOT_HEADER_TAG_END          # End tag
      .long 8                                 # Size of the tag (including this header)
header_end:

.globl _start
_start = (entry)

# addr 0x100000 (or 0x1001cc)
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

