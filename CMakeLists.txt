cmake_minimum_required(VERSION 3.28.0)
project(GenesisOS VERSION 0.1.0 LANGUAGES C ASM)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
set( CMAKE_MESSAGE_LOG_LEVEL "STATUS" )
set(CMAKE_ASM_COMPILER /usr/bin/gcc)

# set the compiler
set(CMAKE_C_COMPILER "/usr/bin/clang-18")
set(CMAKE_ASM_NASM_OBJECT_FORMAT elf32)

# set compiler flags
set(CMAKE_C_FLAGS "-ffreestanding -fno-pic -static \
                   -fno-strict-aliasing -O0 -Wall -ggdb -m32 \
                   -Werror -Wno-pragma-pack -fno-omit-frame-pointer -fno-stack-protector \
                   -Wno-int-conversion")

set(CMAKE_ASM_FLAGS "-m32")

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
file(GLOB KERNEL_SRCS  
            ${KERNEL_SRC_DIR}/*.c
            ${KERNEL_SRC_DIR}/gfx/*.c)

add_definitions(-DNK_IMPLEMENTATION
                -DNK_INCLUDE_DEFAULT_ALLOCATOR
                -DNK_INCLUDE_STANDARD_FS
                -DNK_INCLUDE_FIXED_TYPES
                -DNK_RAWFB_IMPLEMENTATION
                -DNK_INCLUDE_FONT_BAKING
                -DNK_INCLUDE_DEFAULT_FONT
                -DNK_INCLUDE_SOFTWARE_FONT
                -DNK_IMPLEMENTATION)

set_property(SOURCE ${KERNEL_SRC_DIR}/gfx/gui.c 
            APPEND_STRING
            PROPERTY COMPILE_FLAGS 
            "-Wno-unused-function -fno-stack-protector -Wno-incompatible-pointer-types-discards-qualifiers \
            -Wno-unused-value")

file(GLOB KERNEL_ASSEMBLIES     ${KERNEL_SRC_DIR}/trap_entry.S 
                                ${KERNEL_SRC_DIR}/trap_dispatcher.S
                                ${KERNEL_SRC_DIR}/kentry.S)

file(GLOB DRIVERS_SRCS 
            ${DRIVERS_SRC_DIR}/*.c
            ${DRIVERS_SRC_DIR}/gfx/*.c)

set_property(SOURCE ${KERNEL_SRC_DIR}/kmalloc.c
            APPEND_STRING
            PROPERTY COMPILE_FLAGS
            " -Wno-parentheses-equality -Wno-expansion-to-defined -Wno-shorten-64-to-32 \
            -Wno-unused-but-set-variable")

add_executable(kernel.elf ${KERNEL_SRCS} ${KERNEL_ASSEMBLIES} ${DRIVERS_SRCS})
add_dependencies(kernel.elf dispatcher)
add_custom_target(kernel
    COMMENT "GenesisOS: Building kernel.elf"
    COMMAND echo "[+] kernel.elf built successfully"
    DEPENDS kernel.elf)

add_subdirectory(genesis/kernel/allocator)
target_link_libraries(kernel.elf dlmalloc)

add_subdirectory(genesis/drivers/ff15)
target_link_libraries(kernel.elf fatfs)


# Set linker options to skip standard libraries and startup files
set_target_properties(kernel.elf PROPERTIES
    LINK_FLAGS "-fuse-ld=lld -nostdlib -nodefaultlibs -nostartfiles -Wl,--build-id=none,-T,${KERNEL_SRC_DIR}/kernel.ld")




# Include userland directory
add_subdirectory(genesis/userland)


add_custom_target(fsdisk
    COMMENT "Filesystem: Generating FAT32 disk image"
    COMMAND dd if=/dev/zero of=${CMAKE_BINARY_DIR}/disk.img bs=1M count=33

    # Creating FAT32 partition"
    COMMAND mformat -F -i ${CMAKE_BINARY_DIR}/disk.img ::

    # Copying files to disk image
    COMMAND mcopy -i ${CMAKE_BINARY_DIR}/disk.img ${CMAKE_BINARY_DIR}/genesis/userland/init.elf ::init
    COMMAND mcopy -i ${CMAKE_BINARY_DIR}/disk.img ${CMAKE_SOURCE_DIR}/scripts/res/banner.txt ::lol.txt
    COMMAND mcopy -i ${CMAKE_BINARY_DIR}/disk.img ${CMAKE_SOURCE_DIR}/scripts/res/out.sfn2 ::font.sfn
    COMMAND mcopy -i ${CMAKE_BINARY_DIR}/disk.img ${CMAKE_SOURCE_DIR}/scripts/res/logo.bmp ::logo.bmp
    COMMAND mcopy -i ${CMAKE_BINARY_DIR}/disk.img ${CMAKE_SOURCE_DIR}/scripts/res/opensa.ttf ::opensa.ttf

    DEPENDS userland)


# Custom target for ISO creation
add_custom_target(iso
    COMMENT "ISO: Generating ISO w/ grub-mkrescue"
    COMMAND cp -r ${CMAKE_SOURCE_DIR}/iso ${CMAKE_BINARY_DIR}/iso
    COMMAND cp ${CMAKE_BINARY_DIR}/kernel.elf ${CMAKE_BINARY_DIR}/iso/targets/x86/boot/kernel.bin
    COMMAND grub-mkrescue /usr/lib/grub/i386-pc -o ${CMAKE_BINARY_DIR}/kernel.iso ${CMAKE_BINARY_DIR}/iso/targets/x86 > /dev/null 2>&1
    DEPENDS kernel userland fsdisk
)


include(ExternalProject)
ExternalProject_Add(libnuklear
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/genesis/kernel/gfx/nuklear
    BINARY_DIR ${CMAKE_BINARY_DIR}/nuklear_build
    CMAKE_ARGS  -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} 
                -DBUILD_SHARED_LIBS=OFF
                -DCMAKE_C_FLAGS=-I${CMAKE_SOURCE_DIR}/genesis
    BUILD_BYPRODUCTS ${CMAKE_BINARY_DIR}/nuklear_build/libnuklear.a
    INSTALL_COMMAND ""
    LOG_BUILD ON
    LOG_CONFIGURE ON
)

target_link_libraries(kernel.elf ${CMAKE_BINARY_DIR}/nuklear_build/libnuklear.a)
