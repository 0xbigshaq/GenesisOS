#!/bin/bash

# Add the LLVM repository to your sources list
sudo bash -c 'echo "deb http://apt.llvm.org/jammy/ llvm-toolchain-jammy-18 main" > /etc/apt/sources.list.d/llvm.list'
sudo bash -c 'echo "deb-src http://apt.llvm.org/jammy/ llvm-toolchain-jammy-18 main" >> /etc/apt/sources.list.d/llvm.list'

# Import the GPG key
wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -


sudo apt update
DEBIAN_FRONTEND=noninteractive sudo apt install \
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
    libc6-dev-i386 \
    python3-pip \
    mtools \
    graphviz \
    jq \
    nasm \
    clang-18 lldb-18 lld-18 clangd-18 \
    ninja-build \
    -y


git clone --branch main https://github.com/hugsy/gef.git $HOME/gef/
echo "source $HOME/gef/gef.py" > ~/.gdbinit 
echo "add-auto-load-safe-path /share/.gdbinit" >> ~/.gdbinit

pip3 install compiledb
# to generate `compile_commands.json` file, run: `compiledb make iso`

echo "" > ~/.osdevrc
echo "export LC_CTYPE=C.UTF-8" >> ~/.osdevrc
echo "export PS1='\[\e[31m\][\[\e[m\]\[\e[38;5;172m\]\u\[\e[m\]@\[\e[38;5;153m\]\h\[\e[m\] \[\e[38;5;214m\]\w\[\e[m\]\[\e[31m\]]\[\e[m\]\\$ '" >> ~/.osdevrc
echo "cat /tmp/penguin.txt" >> ~/.osdevrc

cat > /tmp/penguin.txt<<'EOF'
           _..._
         .'     '.
        /  _   _  \
        | (o)_(o) |
         \(     ) /
         //'._.'\ \
        //   .   \ \
       ||   .     \ \
       |\   :     / |
       \ `) '   (`  /_
     _)``".____,.'"` (_
     )     )'--'(     (
      '---`      `---`
EOF

if ! grep -Fq "osdevrc" ~/.bashrc; then
    echo "source ~/.osdevrc" >> ~/.bashrc
fi