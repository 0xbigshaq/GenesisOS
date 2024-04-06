section .text                   
    global  _start              
                                
_start:                   
    mov ebp, esp 
    push 0x100        
    push 0x200        
    push 0x300        
    push msg
    push 1
    mov     eax,1               
    int     0x80                

_wow:                            
        nop
        nop
        jmp _wow

section .data                   
msg db      "Hello from userland!",0xa 
len equ     $ - msg             