#include "types.h"
#include "mmu.h"
#include "memlayout.h"
#include "console.h"
#include "kmalloc.h"
#include "vm.h"
#include "uart.h"
#include "interrupts.h"

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
    setup_idt();

    puts(0, 1, BRIGHT, BLACK, "Hello from BereshitOS  :^)");
    // int* zzz = 0x0;
    // *zzz=0x300;

    // for(int dummy = 10 ; dummy>=0; dummy--) {
    //   uart_write("[~] working on it....");
    //   uart_putchar('0'+dummy);
    //   uart_putchar(' ');
    //   char rand_char = 'z'/dummy;
    //   uart_putchar('a'+rand_char);
    //   uart_putchar('\n');
      
    // }

    while(1) {
      // spin
    }  
    return;
}