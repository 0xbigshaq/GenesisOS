#!/bin/bash
set -e
set -x

# qemu:
# 	qemu-system-i386 -cdrom $(BUILD_DIR)/kernel.iso $(QEMU_OPTS)

# qemu-nox:
qemu-system-i386 \
    -nographic \
    -boot d \
    -cdrom build/kernel.iso \
    -hda build/disk.img \
    -m 512 

# qemu-gdb:
# 	@echo "\n[*] ======= starting QEMU+gdb ======="
# 	@echo "[*]         To close: Ctrl-A X"
# 	@echo "Starting "
# 	qemu-system-i386 -s -S -nographic -boot d -cdrom $(BUILD_DIR)/kernel.iso -hda $(DISK_IMG) $(QEMU_EXTRAS)