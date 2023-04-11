#include "types.h"
#include "mmu.h"
#include "memlayout.h"
#include "console.h"
#include "kmalloc.h"
#include "vm.h"
#include "uart.h"

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
    uart_write("[*] GenesisOS booting...\n");
    while(1) {
      // spin
    }  
    return;
}