ORG 0 ;0x7c00
BITS 16

_start:
    jmp short start
    nop ;no operation

times 33 db 0 ;bios parameter block, bios wont fill in values

start:
    jmp 0x7c0:start2 ;ensure code seg location jump

start2:
    ;setup segment registers to ensure compatibility
    ;register * 16 + offset (org is offset)
    cli ;clear interupts
    mov ax, 0x7c0 ;code segment location
    mov ds, ax ;DS (Data Segment)
    mov es, ax ;ES (Extra Segment)
    mov ax, 0x00 ;correct?
    mov ss, ax ;SS (Stack Segment)
    mov sp, 0x7c00 ;stack pointer, setup stack
    sti ;enable interupts

    mov si, message
    call print
    jmp $

print:
    mov bx, 0

    .loop:
        lodsb ;load and increment si in al
        cmp al, 0 ;check if 0
        je .done ;call done zero flag (previous if true)
        call print_char

        jmp .loop

    .done:
        ret

print_char:
    mov ah, 0eh
    int 0x10
    ret

message: db 'Bailey', 0

times 510-($ - $$) db 0 ;fill sector
dw 0xAA55 ;55AA but reversed