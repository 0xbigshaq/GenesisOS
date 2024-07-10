cmake_minimum_required(VERSION 3.28.0)

set(CMAKE_SYSTEM_PROCESSOR "i386")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")

project(lib_nuklear VERSION 4.12.0 LANGUAGES C)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-implicit-function-declaration -Wno-incompatible-library-redeclaration")

add_definitions(-DNK_INCLUDE_DEFAULT_ALLOCATOR
                -DNK_INCLUDE_STANDARD_FS
                -DNK_INCLUDE_FIXED_TYPES
                -DNK_RAWFB_IMPLEMENTATION
                -DNK_INCLUDE_FONT_BAKING
                -DNK_INCLUDE_DEFAULT_FONT
                -DNK_INCLUDE_SOFTWARE_FONT
                -DNK_IMPLEMENTATION
)

add_library(nuklear STATIC ${SOURCES})
