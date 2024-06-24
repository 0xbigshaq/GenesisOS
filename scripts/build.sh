#!/bin/bash
set -e

# rm -rf build/
# cmake -GNinja -S . -B ./build/

# ninja -C build/ -v iso
ninja -C build/ iso

dd if=/dev/zero of=build/disk.img bs=1M count=2
mkfs.vfat -F 32 build/disk.img

echo "\n[*] Copying userland binaries to disk.img"
sudo mkdir -p /mnt/disk/
sudo mount build/disk.img /mnt/disk/
sudo cp build/genesis/userland/init.elf /mnt/disk/init
sudo umount /mnt/disk/

