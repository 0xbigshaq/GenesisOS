#!/bin/bash
set -e
set -x

# rm -rf build/
# cmake -GNinja -S . -B ./build/

# ninja -C build/ -v iso
ninja -C build/ iso

dd if=/dev/zero of=build/disk.img bs=1M count=1
mkfs.vfat -F 32 build/disk.img

echo "\n[*] Copying userland binaries to disk.img"
sudo mkdir -p /mnt/disk/
sudo umount /mnt/disk
sudo mount build/disk.img /mnt/disk/
sudo cp build/genesis/userland/init.elf /mnt/disk/init
# sudo sh -c 'echo "It works! this is lol.txt" > /mnt/disk/lol.txt'
sudo cp docs/banner.txt /mnt/disk/lol.txt
sudo umount /mnt/disk/

