CC=gcc
ASM=nasm
LD=ld

CFLAGS=-fno-pic -static -Wall -ggdb -m32 -nostdlib -masm=intel \
		-fno-omit-frame-pointer -fno-stack-protector

LDFLAGS=-m elf_i386

REPO_ROOT:=$(shell realpath ../../)
BUILD_PATH=$(REPO_ROOT)/src/userland/build
SRC_DIR=$(REPO_ROOT)/src/userland

OBJS=$(BUILD_PATH)/libs/std.o \
	  $(BUILD_PATH)/init.o

INC=-I$(REPO_ROOT)/src -I$(REPO_ROOT)/src/userland/libs  -I/usr/include -I/usr/include/linux/

bins:
	@echo "\n[*] ======= Compiling user-land binaries ======="
	mkdir -p $(BUILD_PATH)
	mkdir -p $(BUILD_PATH)/libs
	$(CC) $(CFLAGS) $(INC) -c $(SRC_DIR)/init.c -o $(BUILD_PATH)/init.o
	$(CC) $(CFLAGS) $(INC) -c $(SRC_DIR)/libs/std.c -o $(BUILD_PATH)/libs/std.o
	$(LD) $(LDFLAGS) -o $(BUILD_PATH)/init.elf $(OBJS) -b binary

fs: bins
	@echo "\n[*] Copying userland binaries to disk.img"
	sudo mkdir -p /mnt/disk/
	sudo mount $(REPO_ROOT)/disk.img /mnt/disk/
	sudo cp $(BUILD_PATH)/init.elf /mnt/disk/init
	sudo umount /mnt/disk/

clean:
	rm -rf $(BUILD_PATH)/*

