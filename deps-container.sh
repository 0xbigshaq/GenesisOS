#!/bin/bash

apt update
DEBIAN_FRONTEND=noninteractive apt install \
    build-essential \
    gdb \
    file \
    less \
    qemu-system-x86 \
    tmux vim \
    nasm \
    git \
    sudo \
    grub-common \
    grub-pc \
    xorriso \
    dosfstools \
    -y

git clone --branch main https://github.com/hugsy/gef.git /root/gef/
echo "source /root/gef/gef.py" > ~/.gdbinit 
echo "add-auto-load-safe-path /share/.gdbinit" >> ~/.gdbinit

echo "export LC_CTYPE=C.UTF-8" >> ~/.bashrc
echo "export PS1='\[\e[1;34m\]\u@\h \[\e[1;32m\]\w \[\e[1;31m\]🌀 \\$ \[\e[0m\]'" >> ~/.bashrc

# Note: to exit qemu, Ctrl-A X    (source: https://www.ics.uci.edu/~aburtsev/238P/2020spring/hw/hw2-boot-xv6.html)


# tmux commands:
# :set-window-option -g mode-keys vi
# 
# more: https://tmuxcheatsheet.com/
# 
# 
