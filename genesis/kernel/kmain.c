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
#include "kernel/multiboot2.h"

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


typedef struct {
    uint32_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t  framebuffer_bpp;
} framebuffer_info_t;

framebuffer_info_t framebuffer_info;

void parse_multiboot2_info(void* multiboot_info) {
    uint32_t* header = (uint32_t*)multiboot_info;
    uint32_t total_size = *header++;
    uint32_t reserved = *header++;
    (void)reserved;

    uint8_t* tag = (uint8_t*)header;

    while (tag < (uint8_t*)multiboot_info + total_size) {
        uint32_t type = *((uint32_t*)tag);
        uint32_t size = *((uint32_t*)(tag + 4));

        kprintf("Tag type: 0x%x, size: 0x%x\n", type, size);

        if (type == MULTIBOOT_TAG_TYPE_FRAMEBUFFER) { // Framebuffer tag
            framebuffer_info.framebuffer_addr = *((uint32_t*)(tag + 8));
            framebuffer_info.framebuffer_pitch = *((uint32_t*)(tag + 12));
            framebuffer_info.framebuffer_width = *((uint32_t*)(tag + 16));
            framebuffer_info.framebuffer_height = *((uint32_t*)(tag + 20));
            framebuffer_info.framebuffer_bpp = *((uint8_t*)(tag + 24));
            kprintf("Framebuffer info: addr=0x%x, pitch=%d, width=%d, height=%d, bpp=%d\n",
                   framebuffer_info.framebuffer_addr,
                   framebuffer_info.framebuffer_pitch,
                   framebuffer_info.framebuffer_width,
                   framebuffer_info.framebuffer_height,
                   framebuffer_info.framebuffer_bpp);
        }

        tag += size;
        if (size % 8 != 0) {
            tag += 8 - (size % 8);
        }
    }
}


void kmain()
{
    uint32_t mb_addr = get_ebx();
    void* multiboot_info =(void*)mb_addr;
    parse_multiboot2_info(phys_to_virt(multiboot_info));
    // clear(BLACK);
    // puts(0, 0, GREEN, BLACK, "Hello from BereshitOS  :^)");
    
    kmalloc_init(kern_end, phys_to_virt(1024*1024*4)); // Map only 4MBs  because the  mapping 
                                                                            // at `entrypgdir` has a limit of 4MB.
    
    init_kernelvm();
    init_uart();
    init_console();
    
    uart_write("[*] *OS booting...\n");
    init_pic();
    setup_idt();

    // puts(0, 1, BRIGHT, BLACK, "Hello from BereshitOS  :^)");

    mount_fs();
    load_init();
    uart_write("[+] Ready\n");
    // putpixel(0,0, 255);
    run_init();

    while(1) {
      // spin
    }  
    return;
}