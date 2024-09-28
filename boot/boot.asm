ORG 0x7c00
BITS 16

start:
    mov ah, 0eh
    mov al, 'B'
    mov bx, 0 ;screen color?
    int 0x10

    jmp $

times 510-($ - $$) db 0 ;fill sector
dw 0xAA55 ;55AA?