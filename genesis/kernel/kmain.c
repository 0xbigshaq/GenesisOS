/**
 * @file kmain.c
 * @brief GenesisOS main
 */
#include "kernel/types.h"
#include "kernel/mmu.h"
#include "kernel/memlayout.h"
#include "kernel/kmalloc.h"
#include "kernel/vm.h"
#include "kernel/interrupts.h"
#include "kernel/pic.h"
#include "kernel/proc.h"
#include "kernel/mb.h"
#include "kernel/sched.h"
#include "drivers/vconsole.h"
#include "drivers/uart.h"
#include "drivers/fat32.h"
#include "drivers/gfx/framebuffer.h"
#include "drivers/mouse.h"
#include "drivers/keyboard.h"
#include "kernel/gfx/gui.h"

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

/**
 * @brief Entry point of the kernel.
 */
void kmain()
{
    parse_multiboot2_info(phys_to_virt(get_ebx()));
    kmalloc_init(kern_end, phys_to_virt(1024*1024*4)); // Map only 4MBs  because the  mapping 
                                                                            // at `entrypgdir` has a limit of 4MB.
    init_kernelvm();

    init_uart();
    uart_write("[*] *OS booting...\n");

    init_mouse();
    keyboard_init();
    pic_init();
    setup_idt();

    mount_fs();
    load_init();
    init_framebuffer();
    vconsole_init();
    gui_init();
    uart_write("[+] Ready\n");
    run_init();
    scheduler(); // enter the scheudler loop

    while(1) {
      // spin
    }  
    return;
}