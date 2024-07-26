/**
 * @file bmp.h
 * @brief BMP Rendering component
 */
#ifndef BMP_H
#define BMP_H

#include <stdint.h>

#pragma pack(push, 1)
typedef struct {
    uint16_t bfType;        //!< Magic identifier
    uint32_t bfSize;        //!< File size in bytes
    uint16_t bfReserved1;   //!< Not used
    uint16_t bfReserved2;   //!< Not used
    uint32_t bfOffBits;     //!< Offset to image data in bytes from beginning of file
} BMPFileHeader;

typedef struct {
    uint32_t biSize;            //!< Size of this header in bytes
    int32_t biWidth;            //!< Image width
    int32_t biHeight;           //!< Image height
    uint16_t biPlanes;          //!< Number of color planes
    uint16_t biBitCount;        //!< Number of bits per pixel
    uint32_t biCompression;     //!< Compression methods used
    uint32_t biSizeImage;       //!< Image size in bytes
    int32_t biXPelsPerMeter;    //!< Preferred resolution in pixels per meter
    int32_t biYPelsPerMeter;    //!< Preferred resolution in pixels per meter
    uint32_t biClrUsed;         //!< Number color map entries that are actually used
    uint32_t biClrImportant;    //!< Number of significant colors
} BMPInfoHeader;
#pragma pack(pop)


unsigned char* read_bmp(const char* filename, BMPFileHeader* fileHeader, BMPInfoHeader* infoHeader);
void render_bmp_to_framebuffer(unsigned char* pixelData, BMPInfoHeader* infoHeader, uint32_t* framebuffer, int screenWidth, int screenHeight);
uint32_t display_bmp(const char* filename, uint32_t* framebuffer, int screenWidth, int screenHeight);

#endif // BMP_H