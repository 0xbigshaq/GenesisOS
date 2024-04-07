%define SYS_write 1

section .text
    global  _start

_start:
    mov ebp, esp
    push msg
    push SYS_write
    mov     eax,1
    int     0x80

_loop:
        nop
        nop
        jmp _loop

section .data
msg db      "Hello from userland!"
len equ     $ - msg