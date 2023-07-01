#include "mmu.h"

  # trap_dispatcher.S sends all traps here.
.globl trap_entry
trap_entry:
  # Build trap frame.
  pushl %ds
  pushl %es
  pushl %fs
  pushl %gs
  pushal
  
  # Set up data segments.
  movw $(SEGMENT_KERNEL_DATA<<3), %ax
  movw %ax, %ds
  movw %ax, %es

  # Call handle_trap(ctx), where ctx=%esp
  pushl %esp
  call handle_trap
  addl $4, %esp

  # Return falls through to trap_ret...
.globl trap_ret
trap_ret:
  popal
  popl %gs
  popl %fs
  popl %es
  popl %ds
  addl $0x8, %esp  # skip trapno and errcode
  iret
