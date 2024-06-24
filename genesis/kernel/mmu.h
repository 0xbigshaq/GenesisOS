#pragma once
// This file contains definitions for the
// x86 memory management unit (MMU).

/* Eflags register
 * Intel SDM Vol. 3, Section 2.3 - System Flags and Fields In The EFLAGS Register
*/
#define FL_IF           (1UL << 9U)      // Interrupt Enable


/* Control Register flags
 * Intel SDM Vol. 3, Section 2.5 - Control Registers
*/
#define CR0_PE (1UL << 0U)              // Protection Enable
#define CR0_ET (1UL << 4U)              // Extension Type (Reserved for older Intel CPUs, hard-coded as 1) 
#define CR0_NW (1UL << 29U)             // Not Write-through
#define CR0_CD (1UL << 30U)             // Cache Disable/Enable bit
#define CR0_PG (1UL << 31U)             // Paging bit
#define CR0_WP (1UL << 16U)             // Write-Protect

#define CR4_PSE (1UL << 4U)             // Page Size Extension (4MB Pages)

/* Segment selectors / idx's
 * Intel SDM Vol. 3, Section 3.4.2 - Segment Selectors
*/
#define SEGMENT_KERNEL_CODE 1  // kernel code
#define SEGMENT_KERNEL_DATA 2  // kernel data+stack
#define SEGMENT_USER_CODE   3  // user code
#define SEGMENT_USER_DATA   4  // user data+stack
#define SEGMENT_TSS         5  // this process's task state

#ifndef __ASSEMBLER__

/* Segment Descriptors
 * Intel SDM Vol. 3, Section 3.4.5 - Segment Descriptors
*/
typedef struct seg_desc {
  // Segment Limit
  uint limit_lo : 16;
  uint base_lo  : 16;

  // Base address
  uint base_mid : 8;

  // Access Byte (8-bit)
  uint accessed  : 1;
  uint rw_bit    : 1;
  uint direction : 1;
  uint execute   : 1;
  uint type      : 1; // 0=TSS, 1=Code/Data
  uint dpl       : 2;
  uint present   : 1;

  // Segment Limit
  uint limit_hi : 4;

  // Flags
  uint reserved    : 1;
  uint longmode    : 1;
  uint seg_size    : 1; // 1=32bit, 0=16bit.
  uint granularity : 1; // 0=byte granularity, 1=page granularity
  
  // Base address
  uint base_hi     : 8;
} __attribute__((packed, __aligned__(0x8))) seg_desc_t;

typedef struct seg_desc_reg {
    uint limit: 16;
    seg_desc_t* ptr;
}  __attribute__((packed, __aligned__(0x8))) seg_desc_reg_t; 

#define DPL_USER    0x3     // Ring 3
#define DPL_KERNEL  0x0     // Ring 0

/* https://wiki.osdev.org/Global_Descriptor_Table#Segment_Descriptor */
// Segment Access bits
#define STA_X       0x8     // Executable segment
#define STA_W       0x2     // Writeable (non-executable segments)
#define STA_R       0x2     // Readable (executable segments)
#define STA_SYSTEM  0x10    // (0b00010000)
#define STA_TSS     0x9     // (0b00001001)

// Segment flags
#define STF_L 0x2   // (0b0010)
#define STF_S 0x4   // (0b0100)
#define STF_G 0x8   // (0b1000)

// A VA has the following structure:
//
// +--------10------+-------10-------+---------12----------+
// | Page Directory |   Page Table   | Offset within Page  |
// |      Index     |      Index     |                     |
// +----------------+----------------+---------------------+
//  \--- PDX(va) --/ \--- PTX(va) --/

// page directory index
#define PDX(va)         (((uint)(va) >> PDXSHIFT) & 0x3FF)

// page table index
#define PTX(va)         (((uint)(va) >> PTXSHIFT) & 0x3FF)

// construct virtual address from indexes and offset
#define PGADDR(d, t, o) ((uint)((d) << PDXSHIFT | (t) << PTXSHIFT | (o)))

// Page directory and page table constants.
#define NPDENTRIES      1024    // # directory entries per page directory
#define NPTENTRIES      1024    // # PTEs per page table
#define PAGESIZE        4096    // bytes mapped by a page
#define PAGE_MASK       (~(PAGESIZE-1))

#define PTXSHIFT        12      // offset of PTX in a linear address
#define PDXSHIFT        22      // offset of PDX in a linear address

#define PAGE_ROUNDUP(x)    ((((uint)(x)) + PAGESIZE - 1) & PAGE_MASK)
#define PAGE_ROUNDDOWN(x)  (((uint)(x)) & PAGE_MASK)
#define IS_PALIGN(x) !((uint)x % PAGESIZE)

// Page table/directory entry flags.
#define PTE_P           0x001   // Present
#define PTE_W           0x002   // Writeable
#define PTE_U           0x004   // User
#define PTE_PS          0x080   // Page Size


// Address in page table or page directory entry
#define PTE_ADDR(pte)   ((uint)(pte) & ~0xFFF)
#define PTE_FLAGS(pte)  ((uint)(pte) &  0xFFF)

// everything below is not used in kernel


// Task state segment format
typedef struct task_state {
  uint link;         // Old ts selector
  uint esp0;         // Stack pointers and segment selectors
  ushort ss0;        //   after an increase in privilege level
  ushort padding1;
  uint *esp1;
  ushort ss1;
  ushort padding2;
  uint *esp2;
  ushort ss2;
  ushort padding3;
  void *cr3;         // Page directory base
  uint *eip;         // Saved state from last task switch
  uint eflags;
  uint eax;          // More saved state (registers)
  uint ecx;
  uint edx;
  uint ebx;
  uint *esp;
  uint *ebp;
  uint esi;
  uint edi;
  ushort es;         // Even more saved state (segment selectors)
  ushort padding4;
  ushort cs;
  ushort padding5;
  ushort ss;
  ushort padding6;
  ushort ds;
  ushort padding7;
  ushort fs;
  ushort padding8;
  ushort gs;
  ushort padding9;
  ushort ldt;
  ushort padding10;
  ushort t;          // Trap on task switch
  ushort iomb;       // I/O map base address
} task_state_t;

#endif
