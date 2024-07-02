#include "kernel/mb.h"
#include "kernel/string.h"
#include "drivers/console.h"
#include "drivers/gfx.h"

mb_tag_framebuffer_t fb_info;

void parse_multiboot2_info(void* multiboot_info) {
    uint32_t* header = (uint32_t*)multiboot_info;
    uint32_t total_size = *header++;
    uint32_t reserved = *header++;
    (void)reserved;

    uint8_t* tag = (uint8_t*)header;

    kprintf("  > multiboot2 info address: 0x%x\n", multiboot_info);
    kprintf("  > total size: 0x%x\n", total_size);

    while (tag < (uint8_t*)multiboot_info + total_size) {
        uint32_t type = *((uint32_t*)tag);
        uint32_t size = *((uint32_t*)(tag + 4));

        // kprintf("Tag type: 0x%x, size: 0x%x\n", type, size);

        // Print the tag content for debugging purposes
        // for (uint32_t i = 0; i < size; i += 4) {
        //     kprintf("Tag data[%d]: 0x%x\n", i, *((uint32_t*)(tag + i)));
        // }

        if (type == MULTIBOOT_TAG_TYPE_FRAMEBUFFER) { // Framebuffer tag found
            memcpy(&fb_info.common, tag, sizeof(fb_info.common));
            kprintf("[+] Framebuffer info: addr=0x%x, pitch=%d, width=%d, height=%d, bpp=%d\n",
                   fb_info.common.framebuffer_addr,
                   fb_info.common.framebuffer_pitch,
                   fb_info.common.framebuffer_width,
                   fb_info.common.framebuffer_height,
                   fb_info.common.framebuffer_bpp);
            if(fb_info.common.framebuffer_addr) { gfx_enabled = 1;}
        }

        tag += size;
        if (size % MULTIBOOT_TAG_ALIGN != 0) {
            tag += MULTIBOOT_TAG_ALIGN - (size % MULTIBOOT_TAG_ALIGN);
        }
    }
}
