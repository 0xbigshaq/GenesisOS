# project(fatfs)

add_library(fatfs STATIC
    ff.c
    ff.h
    diskio.c
    diskio.h
)

target_include_directories(fatfs PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR} 
                    ${CMAKE_CURRENT_SOURCE_DIR}/../../)

