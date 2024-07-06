#include "drivers/gfx/bmp.h"
#include "drivers/fat32.h"
#include "drivers/console.h"
#include "kernel/kmalloc.h"
#include "kernel/string.h"

unsigned char* read_bmp(const char* filename, BMPFileHeader* fileHeader, BMPInfoHeader* infoHeader) {
    FRESULT rc;
    UINT br;
    FILINFO fno;
    FIL font_fp;
    uint8_t *cursor = 0;
    uint8_t *bmp_data = 0;

    rc = f_stat(filename, &fno);
    if(rc != FR_OK) goto fatal_err;    
    kprintf("[*] logo.bmp was found, size: %d\n", fno.fsize);
    bmp_data = cursor = malloc(fno.fsize);
    kprintf("[*] bmp_data @ 0x%x, cursor @ 0x%x\n", bmp_data, cursor);

    rc = f_open(&font_fp, filename, FA_READ);
    if(rc != FR_OK) goto fatal_err;

    rc = f_read(&font_fp, bmp_data, fno.fsize, &br);
    if(rc != FR_OK) goto fatal_err;

    kprintf("[*] %d bytes read\n", br);
    f_close(&font_fp);


    memcpy(fileHeader, cursor, sizeof(BMPFileHeader));
    cursor += sizeof(BMPFileHeader);
    memcpy(infoHeader, cursor , sizeof(BMPInfoHeader));

    if (fileHeader->bfType != 0x4D42) {
        kprintf("Error: Not a valid BMP file\n");
        return NULL;
    }

    cursor = &bmp_data[fileHeader->bfOffBits];


    return cursor;
    fatal_err:
    kprintf("[!] FAT FS Error: %d\n", rc);
    kprintf("Cannot open logo.bmp file");
    return 0;
}

void render_bmp_to_framebuffer(unsigned char* pixelData, BMPInfoHeader* infoHeader, uint32_t* framebuffer, int screenWidth, int screenHeight) {
    int width = infoHeader->biWidth;
    int height = infoHeader->biHeight;
    int bpp = infoHeader->biBitCount / 8;
    
    // Ensure the image fits in the framebuffer
    if (width > screenWidth) width = screenWidth;
    if (height > screenHeight) height = screenHeight;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int bmpIndex = (height - 1 - y) * width * bpp + x * bpp;
            int fbIndex = y * screenWidth + x;

            uint8_t blue = pixelData[bmpIndex];
            uint8_t green = pixelData[bmpIndex + 1];
            uint8_t red = pixelData[bmpIndex + 2];

            // Assuming 32-bit framebuffer (ARGB)
            framebuffer[fbIndex] = (0xFF << 24) | (red << 16) | (green << 8) | blue;
        }
    }
}

uint32_t display_bmp(const char* filename, uint32_t* framebuffer, int screenWidth, int screenHeight) {
    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    unsigned char* pixelData = read_bmp(filename, &fileHeader, &infoHeader);
    if (pixelData) {
        render_bmp_to_framebuffer(pixelData, &infoHeader, framebuffer, screenWidth, screenHeight);
        free(pixelData);
    }

    return infoHeader.biHeight;
}
