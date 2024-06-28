CC=gcc
ASM=nasm
LD=ld
CFLAGS=-ffreestanding -fno-pic -static -fno-builtin \
		-fno-strict-aliasing -O2 -Wall -ggdb -m32 \
		-Werror -fno-omit-frame-pointer -fno-stack-protector -mno-80387 -Wno-div-by-zero

SRC_DIR=src
INC_DIR=src  -I/usr/include/ -I/usr/include/x86_64-linux-gnu
BUILD_DIR=build
DISK_IMG=disk.img

KERN_SRC_DIR=$(SRC_DIR)/kernel
KERN_OBJ_DIR=$(BUILD_DIR)/kernel

DRIVERS_SRC_DIR=$(SRC_DIR)/drivers
DRIVERS_OBJ_DIR=$(BUILD_DIR)/drivers

# $(wildcard *.cpp /xxx/xxx/*.cpp): get all .cpp files from the current directory and dir "/xxx/xxx/"
# $(patsubst %.cpp,%.o,$(SRCS)): substitute all ".cpp" file name strings to ".o" file name strings

KERNEL_SRCS := $(wildcard $(KERN_SRC_DIR)/*.c)
KERNEL_SRCS += $(wildcard $(KERN_SRC_DIR)/*.S)
KERNEL_OBJS := $(patsubst $(KERN_SRC_DIR)/%.c,$(KERN_OBJ_DIR)/%.o,$(KERNEL_SRCS))
KERNEL_OBJS := $(patsubst $(KERN_SRC_DIR)/%.S,$(KERN_OBJ_DIR)/%.o,$(KERNEL_OBJS))

DRIVER_SRCS := $(wildcard $(DRIVERS_SRC_DIR)/*.c)
DRIVER_OBJS := $(patsubst $(DRIVERS_SRC_DIR)/%.c,$(DRIVERS_OBJ_DIR)/%.o,$(DRIVER_SRCS))

CPUS=1
QEMU_OPTS=-smp $(CPUS) -m 512 -serial pty -serial stdio

dirsetup:
	mkdir -p $(DRIVERS_OBJ_DIR)
	mkdir -p $(KERN_OBJ_DIR)
	python3 ./gen_vectors.py > $(KERN_SRC_DIR)/trap_dispatcher.S


# drivers
# -------------------------
build/drivers/%.o: src/drivers/%.c
	@echo "[*] Compiling $^"
	$(CC) $(CFLAGS) -fno-pic -nostdinc -I$(INC_DIR) -c $^ -o $@

drivers: $(DRIVER_OBJS)
	@echo "[*] ======= Done compiling drivers ======="

# kernel (C sources & assemblies)
# -------------------------
build/kernel/%.o: src/kernel/%.c
	@echo "[*] Compiling $^"
	$(CC) $(CFLAGS) -fno-pic -nostdinc -I$(INC_DIR) -c $^ -o $@

build/kernel/%.o: src/kernel/%.S
	@echo "[*] Compiling $^"
	$(CC) $(CFLAGS) -fno-pic -nostdinc -I$(INC_DIR) -c $^ -o $@

kern: $(KERNEL_OBJS)
	@echo "[*] ======= Done compiling kernel ======="

kernel: kern drivers
	@echo "\n[*] ======= Linking kernel.elf ======="
	$(LD) $(LDFLAGS) -T$(KERN_SRC_DIR)/kernel.ld -o $(BUILD_DIR)/kernel.elf $(KERNEL_OBJS) $(DRIVER_OBJS) -b binary


# OS image
# -------------------------
iso: kernel fs
	@echo "\n[*] ======= Making iso image w/ GRUB bootloader ======="
	cp $(BUILD_DIR)/kernel.elf iso/targets/x86/boot/kernel.bin && \
	grub-mkrescue /usr/lib/grub/i386-pc -o $(BUILD_DIR)/kernel.iso iso/targets/x86 

clean:
	rm -rf $(BUILD_DIR)/*
	$(MAKE) clean -C ./src/userland/

# Emulation & debugging
# -------------------------
qemu:
	qemu-system-i386 -cdrom $(BUILD_DIR)/kernel.iso $(QEMU_OPTS)

qemu-nox:
	qemu-system-i386 -nographic -boot d -cdrom $(BUILD_DIR)/kernel.iso -hda $(DISK_IMG) $(QEMU_EXTRAS)

qemu-gdb:
	@echo "\n[*] ======= starting QEMU+gdb ======="
	@echo "[*]         To close: Ctrl-A X"
	@echo "Starting "
	qemu-system-i386 -s -S -nographic -boot d -cdrom $(BUILD_DIR)/kernel.iso -hda $(DISK_IMG) $(QEMU_EXTRAS)

# Disk / filesystem
# -------------------------
disk:
	@echo "\n[*] ======= Generating FAT32 disk image ======="
	dd if=/dev/zero of=disk.img bs=1M count=1
	mkfs.vfat -F 32 disk.img

fs: disk
	$(MAKE) fs -C ./src/userland/
