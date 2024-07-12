cmake_minimum_required(VERSION 3.28.0)
# project(Userland)

# Set the compiler
set(CMAKE_C_COMPILER clang-18)
set(CMAKE_ASM_NASM_OBJECT_FORMAT elf32)

# Set compiler flags
set(CMAKE_C_FLAGS "-fno-pic -static -Wall -ggdb -m32 -nostdlib -masm=intel \
                   -fno-pie -fno-omit-frame-pointer -fno-stack-protector -Wno-invalid-noreturn")


# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../.. ${CMAKE_CURRENT_SOURCE_DIR}/libs /usr/include)

# Source files
file(GLOB BINS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
file(GLOB LIBS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/libs/*.c)


add_library(genesis_common  ${LIBS_SRC})

# Executable
add_executable(init.elf ${BINS_SRC})
target_link_libraries(init.elf genesis_common)

set_target_properties(init.elf PROPERTIES
    LINK_FLAGS "-fuse-ld=lld -nostdlib -Wl,-T${CMAKE_CURRENT_SOURCE_DIR}/prog.ld ${CMAKE_C_FLAGS}")

# Custom command to copy binaries to disk image
add_custom_target(userland
    DEPENDS init.elf
)