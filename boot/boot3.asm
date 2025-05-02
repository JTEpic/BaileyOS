ORG 0x7c00
[BITS 16]

CODE_OFFSET equ 0x8
DATA_OFFSET equ 0x10
;CODE_OFFSET equ gdt_code - gdt_start
;DATA_OFFSET equ gdt_data - gdt_start

KERNEL_LOAD_SEG equ 0x1000
KERNEL_START_ADDR equ 0x100000
KERNEL_LOAD_OFFSET equ 0x0000

VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f ; the color byte for each character

start:
    ;register * 16 + offset (org is offset)
    cli ;clear interupts
    mov ax, 0x00 ;code segment location
    mov ds, ax ;DS (Data Segment)
    mov es, ax ;ES (Extra Segment)
    mov ss, ax ;SS (Stack Segment)
    mov sp, 0x7c00 ;stack pointer, setup stack
    sti ;enable interupts

    mov AH, 00h
    mov AL, 03h
    int 0x10

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
    mov es, bx
    mov bx, KERNEL_LOAD_OFFSET
    ;push KERNEL_LOAD_SEG
    ;pop es

    mov dh, 0x00 ;head 0
    mov dl, 0x80 ;read from first drive
    mov cl, 0x02 ;2nd sector since first is bootload
    mov ch, 0x00 ;cylinder 0
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
    mov cr0, eax

    jmp CODE_OFFSET:PMmain

disk_read_error:
    hlt

gdt_start:
    dd 0x0
    dd 0x0

    ; Code Segment Descriptor
    gdt_code:
        dw 0xFFFF ;Limit
        dw 0x0000 ;Base
        db 0x00 ;Base
        db 10011010b ;Access byte (binary)
        db 11001111b ;Flags
        db 0x00 ;Base

    ; Data Segment Descriptor
    gdt_data:
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

[BITS 32]
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
    
    ;mov ebx, MSG_PROT_MODE
    ;call print_string_pm
    ;prints Hi at beginning
    mov dword [0xb8000], 0x07690748
    call move_cursor_to_origin

    ; Copy kernel from 0x10000 to 0x100000
    mov esi, 0x10000  ; Source
    mov edi, 0x100000 ; Destination
    mov ecx, 4096     ; 8 sectors * 512 bytes = 4096 bytes
    cld
    rep movsb

    jmp CODE_OFFSET:KERNEL_START_ADDR

move_cursor_to_origin:
    ; Calculate cursor offset (0 for row=0, col=0)
    mov ecx, 0          ; Offset = row * 80 + col = 0

    ; Send high byte of cursor offset
    mov dx, 0x3D4       ; VGA index register
    mov al, 0x0E        ; Cursor location high register
    out dx, al
    mov dx, 0x3D5       ; VGA data register
    mov al, ch          ; High byte of offset (ch = 0)
    out dx, al

    ; Send low byte of cursor offset
    mov dx, 0x3D4
    mov al, 0x0F        ; Cursor location low register
    out dx, al
    mov dx, 0x3D5
    mov al, cl          ; Low byte of offset (cl = 0)
    out dx, al

    ret

print_string_pm:
    pusha
    mov edx, VIDEO_MEMORY

print_string_pm_loop:
    mov al, [ebx] ; [ebx] is the address of our character
    mov ah, WHITE_ON_BLACK

    cmp al, 0 ; check if end of string
    je print_string_pm_done

    mov [edx], ax ; store character + attribute in video memory
    add ebx, 1 ; next char
    add edx, 2 ; next video memory position

    jmp print_string_pm_loop

print_string_pm_done:
    popa
    ret

MSG_PROT_MODE db "Launched in 32-bit Protected Mode", 0

times 510-($ - $$) db 0 ;fill sector
dw 0xAA55 ;55AA but reversed, boot signature