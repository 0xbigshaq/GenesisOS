#!/bin/bash


qemu-system-i386 -s -S -nographic -boot d -cdrom build/kernel.iso -hda build/disk.img
