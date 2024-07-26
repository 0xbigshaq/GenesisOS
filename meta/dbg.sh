#!/bin/bash

qemu-system-i386 \
    -s -S \
    -no-reboot \
    -boot d \
    -cdrom build/kernel.iso \
    -d cpu_reset,guest_errors,int,pcall,trace:all -D qemu_debug.log \
    -hda build/disk.img \
    -vga std -serial mon:stdio \
    -m 512