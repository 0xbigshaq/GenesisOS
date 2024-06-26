cmake_minimum_required(VERSION 3.28.0)
# project(genesis_dlmalloc)

# Set the compiler
set(CMAKE_C_COMPILER clang-18)

set(SRCS 
        ${CMAKE_CURRENT_SOURCE_DIR}/stub.c
        ${CMAKE_CURRENT_SOURCE_DIR}/dlmalloc.c)

add_library(dlmalloc ${SRCS})

set_property(SOURCE ${SRCS}
        APPEND_STRING
        PROPERTY COMPILE_FLAGS
        " -Wno-parentheses-equality -Wno-expansion-to-defined -Wno-shorten-64-to-32 \
        -Wno-unused-but-set-variable")
