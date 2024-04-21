# GenesisOS
<p align="center">
<img src='images/logo.png' />
<br />
<img src='https://github.com/0xbigshaq/GenesisOS/actions/workflows/ci.yaml/badge.svg' />
<a href='https://discord.gg/PpeeR64k'><img src='https://img.shields.io/badge/Discord-5865F2?style=for-the-badge&logo=discord&logoColor=white' /></a>
</p>

Dumb project I made because I wanted to learn about kernels and how they work.

This operating system have the following features:
* Kernel allocator
* Virtual Memory
* Interrupts / IRQs
* Disk: ATA PIO
* Syscalls
* Scheduler (_Round-Robin_ algorithm)
* Console/UART driver 
* Filesystem: Support FAT32
* ELF Loader
* Userland shell/init binary
* Devices/File descriptors(not rly)

All this work, just to load an ELF binary that echoes back whatever you write to it:

<img src='images/terminal.gif' />

To write this project, I used:
* _Intel SDM Vol 3_(`325384-sdm-vol-3abcd.pdf`)
* OSDev Discord community([link](https://discord.gg/osdev))
* OSDev: https://wiki.osdev.org/
* "_The little book about OS development_": https://littleosbook.github.io/
* Academic projects: SerenityOS, JOS and xv6 (to get inspiration for design ideas)
* _nanobyte-dev_ YT channel to get introduction on FAT filesystems.


# Build

Prepare directories
```
make clean && make dirsetup
```

Compile & pack everything:
```
make iso
```

# Run
```
make qemu-nox
```

>Note: _nox_ stands for _no-graphics_. If you run `make qemu` a qemu window will pop. However, it has bugs because the video driver is not finished(unlike the UART/console driver, which is more mature)

# Debug
```
make qemu-gdb
```

# Compile DB

For easier code navigation and auto-completion, use _clangd_'s vscode plugin + [compiledb](https://github.com/nickdiego/compiledb).

```
make clean && make dirsetup
compiledb make iso
```

