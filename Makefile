CC=gcc
ASM=nasm
LD=ld

CFLAGS=-ffreestanding -fno-pic -static -fno-builtin \
		-fno-strict-aliasing -O2 -Wall -ggdb -m32 \
		-Werror -fno-omit-frame-pointer -fno-stack-protector -mno-80387 -Wno-div-by-zero

SRC_DIR=src
BUILD_DIR=build

OBJS = \
	$(BUILD_DIR)/kentry.o\
	$(BUILD_DIR)/kmain.o\
	$(BUILD_DIR)/console.o\
	$(BUILD_DIR)/kmalloc.o\
	$(BUILD_DIR)/vm.o\
	$(BUILD_DIR)/uart.o\
	$(BUILD_DIR)/interrupts.o\


CPUS=1
QEMU_OPTS=-smp $(CPUS) -m 512 -serial pty -serial stdio

kentry: $(SRC_DIR)/kentry.S
	@echo "\n[*] ======= Building kentry ======="
	$(CC) -m32 -ggdb -gdwarf-2 -I$(SRC_DIR) -c $(SRC_DIR)/kentry.S -o $(BUILD_DIR)/kentry.o

kernel: kentry
	@echo "\n[*] ======= Building kernel.elf ======="
	$(CC) $(CFLAGS) -fno-pic -nostdinc -I$(SRC_DIR) -c $(SRC_DIR)/kmain.c -o $(BUILD_DIR)/kmain.o
	$(CC) $(CFLAGS) -fno-pic -nostdinc -I$(SRC_DIR) -c $(SRC_DIR)/console.c -o $(BUILD_DIR)/console.o
	$(CC) $(CFLAGS) -fno-pic -nostdinc -I$(SRC_DIR) -c $(SRC_DIR)/kmalloc.c -o $(BUILD_DIR)/kmalloc.o
	$(CC) $(CFLAGS) -fno-pic -nostdinc -I$(SRC_DIR) -c $(SRC_DIR)/vm.c -o $(BUILD_DIR)/vm.o
	$(CC) $(CFLAGS) -fno-pic -nostdinc -I$(SRC_DIR) -c $(SRC_DIR)/uart.c -o $(BUILD_DIR)/uart.o
	$(CC) $(CFLAGS) -fno-pic -nostdinc -I$(SRC_DIR) -c $(SRC_DIR)/interrupts.c -o $(BUILD_DIR)/interrupts.o
	@echo "\n[*] ======= Linking kernel.elf ======="
	$(LD) $(LDFLAGS) -T$(SRC_DIR)/kernel.ld -o $(BUILD_DIR)/kernel.elf $(OBJS) -b binary

iso: kentry kernel
	cp $(BUILD_DIR)/kernel.elf iso/targets/x86/boot/kernel.bin && \
	grub-mkrescue /usr/lib/grub/i386-pc -o $(BUILD_DIR)/kernel.iso iso/targets/x86 

clean:
	rm -f $(BUILD_DIR)/*

make qemu:
	qemu-system-i386 -cdrom $(BUILD_DIR)/kernel.iso $(QEMU_OPTS)

make qemu-nox:
	qemu-system-i386 -nographic -cdrom $(BUILD_DIR)/kernel.iso $(QEMU_OPTS)

make qemu-gdb:
	@echo "\n[*] ======= starting QEMU+gdb ======="
	@echo "[*]         To close: Ctrl-A X"
	@echo "Starting "
	qemu-system-i386 -s -S -nographic -cdrom $(BUILD_DIR)/kernel.iso

# ASM=nasm

# SRC_DIR=src
# BUILD_DIR=build

# $(BUILD_DIR)/main_floppy.img: $(BUILD_DIR)/main.bin
# 	cp $(BUILD_DIR)/main.bin $(BUILD_DIR)/main_floppy.img
# 	truncate -s 1440k $(BUILD_DIR)/main_floppy.img

# $(BUILD_DIR)/main.bin: $(SRC_DIR)/main.asm
# 	$(ASM) $(SRC_DIR)/main.asm -f bin -o $(BUILD_DIR)/main.bin
