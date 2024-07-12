#!/bin/bash
set -e

sudo mkdir -p /mnt/disk/
sudo mount "$1" /mnt/disk/
sudo cp "$2" /mnt/disk/init
sudo umount /mnt/disk/