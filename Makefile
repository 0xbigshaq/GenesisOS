CC=gcc
ASM=nasm
LD=ld

CFLAGS=-ffreestanding -fno-pic -static -fno-builtin \
		-fno-strict-aliasing -O2 -Wall -ggdb -m32 \
		-Werror -fno-omit-frame-pointer -fno-stack-protector -mno-80387 -Wno-div-by-zero

SRC_DIR=src
INC_DIR=src  -I/usr/include/ -I/usr/include/x86_64-linux-gnu
BUILD_DIR=build

OBJS = \
	$(BUILD_DIR)/kentry.o\
	$(BUILD_DIR)/kmain.o\
	$(BUILD_DIR)/console.o\
	$(BUILD_DIR)/kmalloc.o\
	$(BUILD_DIR)/vm.o\
	$(BUILD_DIR)/uart.o\
	$(BUILD_DIR)/interrupts.o\
	$(BUILD_DIR)/trap_dispatcher.o\
	$(BUILD_DIR)/trap_entry.o\
	$(BUILD_DIR)/pic.o\
	$(BUILD_DIR)/proc.o\
	$(BUILD_DIR)/sched.o\
	$(BUILD_DIR)/string.o\
	$(BUILD_DIR)/syscall.o\
	$(BUILD_DIR)/ata.o\
	$(BUILD_DIR)/fat32.o\


CPUS=1
QEMU_OPTS=-smp $(CPUS) -m 512 -serial pty -serial stdio

# temporary hack, should be with Makefile variables
userland: src/userland/init.asm
	nasm -f elf32 -o build/userland/init.o src/userland/init.asm
	ld -m elf_i386 -o build/userland/init build/userland/init.o
	mount dummy_disk.img /mnt/disk/
	cp ./build/userland/init /mnt/disk/init
	umount /mnt/disk/

kentry: $(SRC_DIR)/kentry.S
	@echo "\n[*] ======= Building kentry ======="
	$(CC) -m32 -ggdb -gdwarf-2 -I$(INC_DIR) -c $(SRC_DIR)/kentry.S -o $(BUILD_DIR)/kentry.o

kvectors: $(SRC_DIR)/trap_dispatcher.S
	@echo "\n[*] ======= Compiling trap_dispatcher dispatcher ======="
	./gen_vectors.py > src/trap_dispatcher.S
	$(CC) -m32 -ggdb -gdwarf-2 -I$(INC_DIR) -c $(SRC_DIR)/trap_dispatcher.S -o $(BUILD_DIR)/trap_dispatcher.o
	$(CC) -m32 -ggdb -gdwarf-2 -I$(INC_DIR) -c $(SRC_DIR)/trap_entry.S -o $(BUILD_DIR)/trap_entry.o


kernel: kentry kvectors
	@echo "\n[*] ======= Building kernel.elf ======="
	$(CC) $(CFLAGS) -fno-pic -nostdinc -I$(INC_DIR) -c $(SRC_DIR)/kmain.c -o $(BUILD_DIR)/kmain.o
	$(CC) $(CFLAGS) -fno-pic -nostdinc -I$(INC_DIR) -c $(SRC_DIR)/console.c -o $(BUILD_DIR)/console.o
	$(CC) $(CFLAGS) -fno-pic -nostdinc -I$(INC_DIR) -c $(SRC_DIR)/kmalloc.c -o $(BUILD_DIR)/kmalloc.o
	$(CC) $(CFLAGS) -fno-pic -nostdinc -I$(INC_DIR) -c $(SRC_DIR)/vm.c -o $(BUILD_DIR)/vm.o
	$(CC) $(CFLAGS) -fno-pic -nostdinc -I$(INC_DIR) -c $(SRC_DIR)/uart.c -o $(BUILD_DIR)/uart.o
	$(CC) $(CFLAGS) -fno-pic -nostdinc -I$(INC_DIR) -c $(SRC_DIR)/interrupts.c -o $(BUILD_DIR)/interrupts.o
	$(CC) $(CFLAGS) -fno-pic -nostdinc -I$(INC_DIR) -c $(SRC_DIR)/pic.c -o $(BUILD_DIR)/pic.o
	$(CC) $(CFLAGS) -fno-pic -nostdinc -I$(INC_DIR) -c $(SRC_DIR)/proc.c -o $(BUILD_DIR)/proc.o
	$(CC) $(CFLAGS) -fno-pic -nostdinc -I$(INC_DIR) -c $(SRC_DIR)/sched.c -o $(BUILD_DIR)/sched.o
	$(CC) $(CFLAGS) -fno-pic -nostdinc -I$(INC_DIR) -c $(SRC_DIR)/string.c -o $(BUILD_DIR)/string.o
	$(CC) $(CFLAGS) -fno-pic -nostdinc -I$(INC_DIR) -c $(SRC_DIR)/syscall.c -o $(BUILD_DIR)/syscall.o
	$(CC) $(CFLAGS) -fno-pic -nostdinc -I$(INC_DIR) -c $(SRC_DIR)/ata.c -o $(BUILD_DIR)/ata.o
	$(CC) $(CFLAGS) -fno-pic -nostdinc -I$(INC_DIR) -c $(SRC_DIR)/fat32.c -o $(BUILD_DIR)/fat32.o
	@echo "\n[*] ======= Linking kernel.elf ======="
	$(LD) $(LDFLAGS) -T$(SRC_DIR)/kernel.ld -o $(BUILD_DIR)/kernel.elf $(OBJS) -b binary

iso: kernel
	cp $(BUILD_DIR)/kernel.elf iso/targets/x86/boot/kernel.bin && \
	grub-mkrescue /usr/lib/grub/i386-pc -o $(BUILD_DIR)/kernel.iso iso/targets/x86 

clean:
	rm -f $(BUILD_DIR)/*

make qemu:
	qemu-system-i386 -cdrom $(BUILD_DIR)/kernel.iso $(QEMU_OPTS)

make qemu-nox:
	qemu-system-i386 -nographic -boot d -cdrom $(BUILD_DIR)/kernel.iso -hda dummy_disk.img $(QEMU_EXTRAS)
# qemu-system-i386 -nographic -cdrom $(BUILD_DIR)/kernel.iso $(QEMU_OPTS)

make qemu-gdb:
	@echo "\n[*] ======= starting QEMU+gdb ======="
	@echo "[*]         To close: Ctrl-A X"
	@echo "Starting "
	qemu-system-i386 -s -S -nographic -boot d -cdrom $(BUILD_DIR)/kernel.iso -hda dummy_disk.img $(QEMU_EXTRAS)

# ASM=nasm

# SRC_DIR=src
# BUILD_DIR=build

# $(BUILD_DIR)/main_floppy.img: $(BUILD_DIR)/main.bin
# 	cp $(BUILD_DIR)/main.bin $(BUILD_DIR)/main_floppy.img
# 	truncate -s 1440k $(BUILD_DIR)/main_floppy.img

# $(BUILD_DIR)/main.bin: $(SRC_DIR)/main.asm
# 	$(ASM) $(SRC_DIR)/main.asm -f bin -o $(BUILD_DIR)/main.bin


# To generate FAT32 disk:
# dd if=/dev/zero of=dummy_disk.img bs=1M count=1
# mkfs.vfat -F 32 dummy_disk.img
