CC=gcc
ASM=nasm
LD=ld

CFLAGS=-fno-pic -static -Wall -ggdb -m32 -nostdlib -masm=intel \
		-fno-omit-frame-pointer -fno-stack-protector

LDFLAGS=-m elf_i386

OBJS=./libs/std.o ./init.o
INC=-I/share/src -I/share/src/userland/libs  -I/usr/include -I/usr/include/linux/

cfile: 
	$(CC) $(CFLAGS) $(INC) -c ./init.c -o ./init.o
	$(CC) $(CFLAGS) $(INC) -c ./libs/std.c -o ./libs/std.o
	$(LD) $(LDFLAGS) -o ./init.elf $(OBJS) -b binary
	mount /share/dummy_disk.img /mnt/disk/
	cp /share/src/userland/init.elf /mnt/disk/init
	umount /mnt/disk/
