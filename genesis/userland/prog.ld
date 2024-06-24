SECTIONS
{
    . = 0x08048000;

    .text : ALIGN(0x1000) {
        *(.text)
    }

    .rodata : ALIGN(0x1000) {
        *(.rodata)
    }

    .eh_frame : ALIGN(0x1000) {
        *(.eh_frame)
        *(.eh_frame_hdr)
    }

    .data : ALIGN(0x1000) {
        *(.data)
    }

    .bss : ALIGN(0x1000) {
        *(.bss)
    }

    /DISCARD/ : {
        *(.note)
        *(.comment)
    }
}
