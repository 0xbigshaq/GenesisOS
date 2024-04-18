#include "types.h"
#include "mmu.h"
#include "memlayout.h"
#include "console.h"
#include "kmalloc.h"
#include "vm.h"
#include "uart.h"
#include "interrupts.h"
#include "pic.h"
#include "proc.h"
#include "ata.h"
#include "fat32.h"

/* Bootstrap Page-Table used for initialization. 
 * Uses the `PTE_PS` bit in order to use 4MB Pages
 * > Map VA's [0 ... 4MB) to PA's [0 ... 4MB)
 * > Map VA's [VIRTBASE ... VIRTBASE+4MB) to PA's [0 ... 4MB)
*/
__attribute__((__aligned__(PAGESIZE)))
pte entrypgdir[NPDENTRIES] = {
  [0] = (0) | PTE_P | PTE_W | PTE_PS,
  [PDX(VIRTBASE)] = (0) | PTE_P | PTE_W | PTE_PS,
};

void kmain()
{
    clear(BLACK);
    puts(0, 0, GREEN, BLACK, "Hello from BereshitOS  :^)");
    
    kmalloc_init(kern_end, phys_to_virt(1024*1024*4)); // Map only 4MBs  because the  mapping 
                                                      // at `entrypgdir` has a limit of 4MB.
    
    init_kernelvm();
    init_uart();
    init_console();
    uart_write("[*] *OS booting...\n");
    init_pic();
    setup_idt();

    puts(0, 1, BRIGHT, BLACK, "Hello from BereshitOS  :^)");

    uart_write("[+] Ready\n");
    // run_init();
    // --- experimental ---
    uint8_t sector[512];
    int lba = 0; // Read from LBA 0
    if (ata_read_sector(lba, sector) == 0) {
        // Print the first 16 bytes of the sector
        // for (int i = 0; i < 256; i++) {
            // kprintf("%x ", sector[i]);
            // uart_putchar(sector[i]);
        // }
        // kprintf("\n\n");
    } else {
        kprintf("Error reading sector\n");
    }
    memmove(&bios_param_block, sector, sizeof(bios_param_block));

    dump_fat32_header(&bios_param_block);
    dump_file();
    run_init();

    while(1) {
      // spin
    }  
    return;
}