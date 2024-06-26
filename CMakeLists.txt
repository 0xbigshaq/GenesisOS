cmake_minimum_required(VERSION 3.28.0)
project(GenesisOS VERSION 0.1.0 LANGUAGES C ASM)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

# set the compiler
set(CMAKE_C_COMPILER clang-18)
set(CMAKE_ASM_NASM_OBJECT_FORMAT elf32)

# set compiler flags
set(CMAKE_C_FLAGS "-ffreestanding -fno-pic -static -fno-builtin \
                   -fno-strict-aliasing -O2 -Wall -ggdb -m32 \
                   -Werror -Wno-pragma-pack -fno-omit-frame-pointer -fno-stack-protector \
                   -mno-80387 -Wno-div-by-zero -Wno-int-conversion")

SET(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS}" )

# include directories
include_directories(${CMAKE_SOURCE_DIR}/genesis /usr/include/ /usr/include/x86_64-linux-gnu)

# source directories
set(KERNEL_SRC_DIR ${CMAKE_SOURCE_DIR}/genesis/kernel)
set(DRIVERS_SRC_DIR ${CMAKE_SOURCE_DIR}/genesis/drivers)

# Custom target to generate trap_dispatcher.S
add_custom_target(dispatcher
    COMMAND python3 ${CMAKE_SOURCE_DIR}/scripts/gen_vectors.py > ${KERNEL_SRC_DIR}/trap_dispatcher.S
    COMMENT "Generating trap_dispatcher.S"
)

# Find all source files
file(GLOB KERNEL_SRCS ${KERNEL_SRC_DIR}/*.c ${KERNEL_SRC_DIR}/*.S ${KERNEL_SRC_DIR}/trap_dispatcher.S)
file(GLOB_RECURSE DRIVERS_SRCS ${DRIVERS_SRC_DIR}/*.c)

set_property(SOURCE ${KERNEL_SRC_DIR}/kmalloc.c
            APPEND_STRING
            PROPERTY COMPILE_FLAGS
            " -Wno-parentheses-equality -Wno-expansion-to-defined -Wno-shorten-64-to-32 \
            -Wno-unused-but-set-variable")

add_executable(kernel.elf ${KERNEL_SRCS} ${DRIVERS_SRCS})
add_dependencies(kernel.elf dispatcher)
add_custom_target(kernel
    DEPENDS kernel.elf)

add_subdirectory(genesis/kernel/allocator)
target_link_libraries(kernel.elf dlmalloc)


# Set linker options to skip standard libraries and startup files
set_target_properties(kernel.elf PROPERTIES
    LINK_FLAGS "-fuse-ld=lld -nostdlib -nodefaultlibs -nostartfiles -Wl,--build-id=none,-T,${KERNEL_SRC_DIR}/kernel.ld")




# Include userland directory
add_subdirectory(genesis/userland)

# Custom target for ISO creation
add_custom_target(iso
    COMMAND cp -r ${CMAKE_SOURCE_DIR}/iso ${CMAKE_BINARY_DIR}/iso
    COMMAND cp ${CMAKE_BINARY_DIR}/kernel.elf ${CMAKE_BINARY_DIR}/iso/targets/x86/boot/kernel.bin
    COMMAND grub-mkrescue /usr/lib/grub/i386-pc -o ${CMAKE_BINARY_DIR}/kernel.iso ${CMAKE_BINARY_DIR}/iso/targets/x86
    DEPENDS kernel.elf userland
)
