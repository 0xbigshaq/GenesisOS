#define ASM_FILE
#include "multiboot2.h"
.intel_syntax noprefix
#include "mmu.h"
#include "memlayout.h"

#define __u8  .byte
#define __u16 .word
#define __u32 .long
#define __u64 .quad


.section .multiboot2_header
.text
.align MULTIBOOT_HEADER_ALIGN
multiboot2_header:
    .align MULTIBOOT_TAG_ALIGN
    __u32 MULTIBOOT2_HEADER_MAGIC               # Magic number
    __u32 MULTIBOOT_ARCHITECTURE_I386           # Architecture
    __u32 header_end - multiboot2_header             # Header length
    __u32 -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + (header_end - multiboot2_header)) # Checksum

    .align MULTIBOOT_TAG_ALIGN
    tag_fb:
      __u16 MULTIBOOT_HEADER_TAG_FRAMEBUFFER    # Framebuffer request tag type
      __u16  0      # flags 
      __u32  tag_fb_end-tag_fb                  # Size of the tag, 20 bytes (including this header)
      __u32  1024                               # Width
      __u32  768                                # Height
      __u32  32                                 # Bits per pixel
    tag_fb_end:

    .align MULTIBOOT_TAG_ALIGN
    tag_terminator:
      __u16 MULTIBOOT_HEADER_TAG_END          # End tag
      __u16 0
      __u32 8                                 # Size of the tag (including this header)
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

