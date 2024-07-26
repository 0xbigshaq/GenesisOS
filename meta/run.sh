#!/bin/bash
set -e
set -x


# run Genesis OS with graphics
cmd="qemu-system-i386 \
    -boot d \
    -cdrom build/kernel.iso \
    -hda build/disk.img \
    -vga std -serial mon:stdio \
    -m 512"

# 'headless'/serial mode (experimental)
if [ "$1" == "-s" ]; then
    cmd="qemu-system-i386 \
        -nographic \
        -boot d \
        -cdrom build/kernel.iso \
        -hda build/disk.img \
        -m 512"
fi

eval $cmd