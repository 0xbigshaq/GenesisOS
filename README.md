# GenesisOS
<p align="center">
<img src='docs/images/logo.png' />
<br />
<img src='https://github.com/0xbigshaq/GenesisOS/actions/workflows/ci.yaml/badge.svg' />
<a href='https://discord.gg/PpeeR64k'><img src='https://img.shields.io/discord/1231650335802789959.svg?logo=discord&logoColor=white&logoWidth=20labelColor=7289DA&color=17cf48&label=Discord' /></a>
<img src='https://img.shields.io/badge/arch-x86/32bit-blue' />
</p>

Dumb project I made because I wanted to learn about kernels and how they work. You are welcomed to contribute to my chaos and join the cult.

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
* Devices/File descriptors(not rly/partial)

All this work, just to load an ELF binary that echoes back whatever you write to it by calling `SYS_read` and `SYS_write`:

<img src='docs/images/terminal.gif' />

To write this project, I used:
* _Intel SDM Vol 3_(`325384-sdm-vol-3abcd.pdf`)
* OSDev Discord community([link](https://discord.gg/osdev))
* OSDev: https://wiki.osdev.org/
* "_The little book about OS development_": https://littleosbook.github.io/
* Academic projects: SerenityOS, JOS and xv6 (to get inspiration for design ideas)
* _nanobyte-dev_ YT channel to get introduction on FAT filesystems.


# Build & Run

Launch an `ubuntu:20.04` container and follow the steps below
```sh
docker run -v $(pwd):/share/ --rm --privileged --name osdev -it ubuntu:20.04
```

```
cmake -GNinja -S . -B build/
```

This will generate a `build/` directory, with a _ninja_ file in it. 
* To start the build with ninja and generate a filesystem, run `./scripts/build.sh`
* To run GenesisOS, run `./scripts/run.sh`

# Debug

To debug:
1. Run `make qemu-gdb`
2. Connect with gdb by running `target remote :12345`

# Security

Found a security issue? Good, fix it and send a PR.

>There are plenty of vulns around the codebase. I'm considering opening a special activities/event for vuln researchers who are interested in OSDev so we can learn and grow together. Should update about it in the Discord server.

# Compile DB

For easier code navigation and auto-completion, use _clangd_'s vscode plugin + [compiledb](https://github.com/nickdiego/compiledb).

```
compiledb make iso
```
