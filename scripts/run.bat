set qemu_exe="C:\Program Files\qemu\qemu-system-i386.exe"
set cmd=%qemu_exe% ^
        -boot d ^
        -cdrom build\kernel.iso ^
        -hda build\disk.img ^
        -vga std ^
        -serial mon:stdio ^
        -m 512

if "%1"=="-s" (
    set cmd=%qemu_exe% ^
        -nographic ^
        -boot d ^
        -cdrom build\kernel.iso ^
        -hda build\disk.img ^
        -m 512
)

call %cmd%