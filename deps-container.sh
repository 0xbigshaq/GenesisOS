#!/bin/bash

apt update
apt install \
    build-essential \
    gdb \
    file \
    less \
    qemu-system-x86 \
    tmux vim \
    nasm \
    git \
    sudo \
    grub-mkrescue \
    -y


# Note: to exit qemu, Ctrl-A X    (source: https://www.ics.uci.edu/~aburtsev/238P/2020spring/hw/hw2-boot-xv6.html)


# tmux commands:
# :set-window-option -g mode-keys vi
# 
# more: https://tmuxcheatsheet.com/
# 
# 
