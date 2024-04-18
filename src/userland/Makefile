CC=gcc
ASM=nasm
LD=ld

CFLAGS=-fno-pic -static -Wall -ggdb -m32 -nostdlib -masm=intel \
		-fno-omit-frame-pointer -fno-stack-protector

LDFLAGS=-m elf_i386

BUILD_PATH=/share/src/userland/build
SRC_DIR=/share/src/userland

OBJS=$(BUILD_PATH)/libs/std.o \
	  $(BUILD_PATH)/init.o

INC=-I/share/src -I/share/src/userland/libs  -I/usr/include -I/usr/include/linux/

bins:
	@echo "\n[*] ======= Compiling user-land binaries ======="
	mkdir -p $(BUILD_PATH)
	mkdir -p $(BUILD_PATH)/libs
	$(CC) $(CFLAGS) $(INC) -c $(SRC_DIR)/init.c -o $(BUILD_PATH)/init.o
	$(CC) $(CFLAGS) $(INC) -c $(SRC_DIR)/libs/std.c -o $(BUILD_PATH)/libs/std.o
	$(LD) $(LDFLAGS) -o $(BUILD_PATH)/init.elf $(OBJS) -b binary

fs: bins
	@echo "\n[*] Copying userland binaries to disk.img"
	mkdir -p /mnt/disk/
	mount /share/disk.img /mnt/disk/
	cp $(BUILD_PATH)/init.elf /mnt/disk/init
	umount /mnt/disk/

clean:
	rm -rf $(BUILD_PATH)/*

