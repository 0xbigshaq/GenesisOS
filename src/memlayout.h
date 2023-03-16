// https://www.qemu.org/docs/master/devel/memory.html#example-memory-map

// MMIO Regions
#define TOPMEM   0xE000000           // General-purpose device communication. TODO: check why not adding an extra zero
#define DEVSPACE 0xFE000000         //  Device space, i.e: system firmware(network adapter, storage controller, display adapter)

#define KBASE_PHYS   0x100000                // Start of extended memory
#define VIRTBASE     0x80000000              // First kernel virtual address
#define KBASE_VIRT  (VIRTBASE+KBASE_PHYS)    // Address where kernel is linked

#define virt_to_phys(a) (((uint) (a)) - VIRTBASE)
#define phys_to_virt(a) ((void *)(((char *) (a)) + VIRTBASE))

// same as above, but without cast
#define _virt_to_phys(x) ((x) - VIRTBASE)
#define _phys_to_virt(x) ((x) + VIRTBASE)

#define KSTACKSIZE 4096
