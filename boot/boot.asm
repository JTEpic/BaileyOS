; Prints Letter
ORG 0x7c00
BITS 16

start:
    mov ah, 0eh ;is command
    mov al, 'B'
    mov bx, 0 ;page num, unneeded?
    int 0x10 ;is interupt

    jmp $

times 510-($ - $$) db 0 ;fill sector
dw 0xAA55 ;55AA but reversed