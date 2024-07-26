/**
 * @file gui.h
 * @brief GUI Component of GenesisOS.
 */

#ifndef GUI_H
#define GUI_H

#include "drivers/gfx/framebuffer.h"
#include "drivers/mouse.h"
#include "drivers/keyboard.h"
#include "drivers/vconsole.h"
#include "drivers/gfx/bmp.h"

typedef struct fb {
    uint32_t *addr;     //!< Framebuffer address
    uint32_t pitch;     //!< Framebuffer pitch
    uint32_t width;     //!< Framebuffer width
    uint32_t height;    //!< Framebuffer height
    uint32_t bpp_bits;  //!< Bits per pixel
    uint32_t bpp_bytes; //!< Bytes per pixel
    uint32_t total;     //!< Total pixels
} fb_t;

typedef struct bg {
    BMPFileHeader fileHeader;   //!< BMP File Header
    BMPInfoHeader infoHeader;   //!< BMP Info Header
    unsigned char *pixeldata;   //!< Raw Pixel data
} bg_t;

/** @brief The GUI context object.
 * 
 * @details This structure represents the context of the GUI module.
 * It contains all the necessary data and settings for GUI operations.
 */
typedef struct gui_ctx {
    // framebuffer / rendering
    rawfb_ctx_t *rawfb;   //!< Raw framebuffer context
    nk_ctx_t *nk_ctx;     //!< Nuklear library context
    fb_t fb;              //!< Framebuffer/VGA information

    // kvm
    keyboard_ctx_t *k_ctx;  //!< Keyboard state object
    vconsole_ctx_t *v_ctx;  //!< Virtual console state object
    mouse_ctx_t *m_ctx;     //!< Mouse state object

    // gui 
    uint32_t *graphics_back_buffer;       //!< Graphics back buffer, used for double buffering.
    unsigned char tex_scratch[512 * 512]; //!< Texture scratch buffer, used by _Nuklear_.
    bg_t bg;                              //!< Background/wallpaper image object.
} gui_ctx_t;

void render_gui();
void gui_init();
void gui_swapbuffers(void);
gui_ctx_t* get_gui_ctx();

#endif // GUI_H


/*! \page gui GUI
 *  \brief Information about the Graphical User Interface

  \tableofcontents
  
  \section gui-overview Overview
  The `render_gui()` is the main function for rendering the GUI.
  It is called on every iteration in the main `scheduler()` loop.

  \note All of the GUI rendering is done using the Nuklear library.
  This is still experimental and may change in the future so it won't be documented until i'm
  sure it's stable/mature/reliable enough.

  \subsection nuklear-lib Rendering Library
  WIP.

  \subsection our-backend GenesisOS Rendering Backend
  WIP.
*/
