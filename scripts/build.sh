#!/bin/bash
set -e
set -x

rm -rf build/
cmake -GNinja -S . -B ./build/

# ninja -C build/ -v iso
ninja -C build/ iso

dd if=/dev/zero of=build/disk.img bs=1M count=10
mkfs.vfat -F 32 build/disk.img

echo "\n[*] Copying userland binaries to disk.img"
sudo mkdir -p /mnt/disk/

# Check if the mount point is currently mounted
MOUNT_POINT="/mnt/disk"
if mountpoint -q "$MOUNT_POINT"; then
    echo "$MOUNT_POINT is mounted. Attempting to unmount..."
    sudo umount "$MOUNT_POINT"
    if [ $? -eq 0 ]; then
        echo "Unmounted successfully."
    else
        echo "Failed to unmount $MOUNT_POINT."
        exit 1
    fi
else
    echo "$MOUNT_POINT is not currently mounted."
fi

sudo mount build/disk.img /mnt/disk/
sudo cp build/genesis/userland/init.elf /mnt/disk/init
# sudo sh -c 'echo "It works! this is lol.txt" > /mnt/disk/lol.txt'
sudo cp scripts/res/banner.txt /mnt/disk/lol.txt

# rendering testing
sudo cp scripts/res/out.sfn2 /mnt/disk/font.sfn
sudo cp scripts/res/logo.bmp /mnt/disk/logo.bmp

sudo umount /mnt/disk/

