#include "diskio.h"
#include "ff.h"
#include "kernel/types.h"
#include "kernel/mmu.h"
#include "kernel/memlayout.h"
#include "kernel/kmalloc.h"
#include "kernel/vm.h"
#include "kernel/interrupts.h"
#include "kernel/pic.h"
#include "kernel/proc.h"
#include "drivers/console.h"
#include "drivers/uart.h"
#include "drivers/ata.h"
#include "drivers/fat32.h"

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

    mount_fs();
    load_init();
    uart_write("[+] Ready\n");
    run_init();

    while(1) {
      // spin
    }  
    return;
}