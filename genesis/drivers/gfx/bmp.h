#ifndef BMP_H
#define BMP_H

#include <stdint.h>

#pragma pack(push, 1)
typedef struct {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BMPFileHeader;

typedef struct {
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BMPInfoHeader;
#pragma pack(pop)

unsigned char* read_bmp(const char* filename, BMPFileHeader* fileHeader, BMPInfoHeader* infoHeader);
void render_bmp_to_framebuffer(unsigned char* pixelData, BMPInfoHeader* infoHeader, uint32_t* framebuffer, int screenWidth, int screenHeight);
uint32_t display_bmp(const char* filename, uint32_t* framebuffer, int screenWidth, int screenHeight);

#endif // BMP_H