ORG 0 ;0x7c00
BITS 16

CODE_OFFSET equ 0x8
DATA_OFFSET equ 0x10

KERNEL_LOAD_SEG equ 0x1000
KERNEL_START_ADDR equ 0x100000

_start:
    jmp short start
    nop ;no operation

times 33 db 0 ;bios parameter block, bios wont fill in values

start:
    jmp 0x7c0:main

main:
    ;register * 16 + offset (org is offset)
    cli ;clear interupts
    mov ax, 0x7c0 ;code segment location
    mov ds, ax ;DS (Data Segment)
    mov es, ax ;ES (Extra Segment)
    
    mov ax, 0x00
    mov ss, ax ;SS (Stack Segment)
    mov sp, 0x7c00 ;stack pointer, setup stack
    sti ;enable interupts

    mov si, message
    call print

    call load_kernel
    jmp load_PM

    jmp $

print:
    mov bx, 0

    .loop:
        lodsb
        cmp al, 0
        je .done
        call print_char

        jmp .loop

    .done:
        ret

print_char:
    mov ah, 0eh
    int 0x10
    ret

message: db 'Bailey', 0

; Load Kernel
;read from disk, CHS addressing, CH DH CL, Cylinder Head Sector
load_kernel:
    mov bx, KERNEL_LOAD_SEG
    mov dh, 0x00
    mov dl, 0x80 ;read from first drive
    mov cl, 0x02 ;2nd sector since first is bootload
    mov ch, 0x00
    mov ah, 0x02 ;read, 3 is write
    mov al, 8 ;num of sectors to read, size of kernel
    int 0x13

    jc disk_read_error

; Load Protected Mode
load_PM:
    cli
    lgdt[gdt_descriptor] ;load gdt
    mov eax, cr0 ;cr0 holds protected mode enable
    or al, 1 ;set 1st bit to 1
    jmp CODE_OFFSET:PMmain

disk_read_error:
    hlt

gdt_start:
    dd 0x0
    dd 0x0

    ; Code Segment Descriptor
    dw 0xFFFF ;Limit
    dw 0x0000 ;Base
    db 0x00 ;Base
    db 10011010b ;Access byte (binary)
    db 11001111b ;Flags
    db 0x00 ;Base

    ; Data Segment Descriptor
    dw 0xFFFF ;Limit
    dw 0x0000 ;Base
    db 0x00 ;Base
    db 10010010b ;Access byte (binary)
    db 11001111b ;Flags
    db 0x00 ;Base

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1 ;size of gdt - 1
    dd gdt_start

BITS 32
PMmain:
    mov ax, DATA_OFFSET
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov ss, ax
    mov gs, ax
    mov ebp, 0x9C00 ;far enough to not overflow previous stack
    mov esp, ebp

    ;access beyond 1MB
    in al, 0x92
    or al, 2
    out 0x92, al

    jmp CODE_OFFSET:KERNEL_START_ADDR

times 510-($ - $$) db 0 ;fill sector
dw 0xAA55 ;55AA but reversed, boot signature