
# Build Instructions
@brief Compiling the OS

@note
> [!NOTE]  
> This project don't have support for Windows(or at least I never tried to), if you're running Windows, generate a build using WSL/Docker and then run the final OS image with `./meta/run.bat`.


## Dependencies

All the dependencies to build and run this project can be found in `./deps-container.sh`


## Docker

Launch an `ubuntu:22.04` container and follow the steps below
```sh
docker run -v $(pwd):/share/ --rm --privileged --name osdev -it ubuntu:22.04
```

And follow the same steps as described in '_Linux/Windows(WSL)_' below.

## Linux/Windows(WSL)

To build, run `./meta/build.sh`
```
$ ./meta/build.sh
-- The C compiler identification is Clang 18.1.8
-- The ASM compiler identification is Clang with GNU-like command-line
-- Found assembler: /usr/bin/cc
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Configuring done (1.3s)
-- Generating done (0.2s)
-- Build files have been written to: /home/sidequest/repos/GenesisOS/build
ninja: Entering directory `build/'
[43/48] Filesystem: Generating FAT32 disk image
33+0 records in
33+0 records out
34603008 bytes (35 MB, 33 MiB) copied, 0.0830217 s, 417 MB/s
[47/48] GenesisOS: Building kernel.elf
[+] kernel.elf built successfully
[48/48] ISO: Generating ISO w/ grub-mkrescue
[+] GenesisOS built successfully
[*] Merging compile commands databases
[+] Done
```

# Run

* If you're on Linux, run `./meta/run.sh` to start the OS with QEMU.
* On Windows, run `.\meta\run.bat` and make sure you have QEMU installed in the default path(`C:\Program Files\qemu\qemu-system-i386.exe`)
