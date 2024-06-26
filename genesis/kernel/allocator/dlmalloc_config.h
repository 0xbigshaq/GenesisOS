// Disable mmap
#define HAVE_MMAP 0
#define HAVE_MORECORE 1

// Define MORECORE function and other configurations
#define MORECORE more_core
#define MORECORE_CONTIGUOUS 1
#define NO_MALLOC_STATS 1
#define ABORT abort

// Other configurations
#define USE_LOCKS 0 // Use this if you do not want thread safety (single-threaded)
#define MALLOC_ALIGNMENT 16 // Alignment for allocations


// Disable mmap
#define HAVE_MMAP 0
#define HAVE_MREMAP 0