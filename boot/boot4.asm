ORG 0x7c00
[BITS 16]

CODE_OFFSET equ 0x8
DATA_OFFSET equ 0x10
;CODE_OFFSET equ gdt_code - gdt_start
;DATA_OFFSET equ gdt_data - gdt_start

KERNEL_LOAD_SEG equ 0x1000 ;4096
KERNEL_LOAD_OFFSET equ 0x0000
KERNEL_LOAD equ 0x10000 ;65536
KERNEL_START_ADDR equ 0x100000 ;1048576

VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f ; the color byte for each character

start:
    ; Real Mode
    ;(register or segment) * 16 + offset (org is offset)
    cli ;clear interupts
    mov ax, 0x00 ;code segment location
    mov ds, ax ;DS (Data Segment)
    mov es, ax ;ES (Extra Segment)
    mov ss, ax ;SS (Stack Segment)
    mov sp, 0x7c00 ;stack pointer, setup stack, grows downwards, 31744
    sti ;enable interupts

    ; video mode, al 03h is a text mode, al 13h is a graphics mode
    mov AH, 00h
    mov AL, 03h
    ;mov AL, 13h
    int 0x10

    mov si, message
    call print

    call load_kernel

    ; Can directly jmp to 64bit/long mode, recommended not
    
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

message: db 'Bailey 16-bit', 0

; Load Kernel
;read from disk, CHS addressing, CH DH CL, Cylinder Head Sector
load_kernel:
    ;segment*16+offset since bx is 16bit/can't hold 0x10000
    mov bx, KERNEL_LOAD_SEG
    mov es, bx
    mov bx, KERNEL_LOAD_OFFSET
    ;push KERNEL_LOAD_SEG
    ;pop es

    call disk_load

    ret

; Assumes kernel is under 1MB size, unreal mode for >1MB kernel or int 13 LBA in extended mode?
disk_load:
    mov dh, 0x00  ;head 0
    mov dl, 0x80  ;read from first drive
    mov cl, 0x02  ;2nd sector since first is bootload
    mov ch, 0x00  ;cylinder 0
    mov ah, 0x02  ;read, 3 is write
    mov al, 32    ;num of sectors to read, num*512=size of kernel, 8 sectors|16|32
    int 0x13

    jc disk_read_error

    ret

disk_read_error:
    cli
    hlt
    jmp $

; Load Protected Mode
load_PM:
    cli
    lgdt[gdt_descriptor] ;load gdt
    mov eax, cr0 ;cr0 holds protected mode enable
    or al, 1 ;set 1st bit to 1
    mov cr0, eax
    ; Next Enable Paging or in PMmain?

    jmp CODE_OFFSET:PMmain

    jmp $

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
    ; Protected Mode
    mov ax, DATA_OFFSET
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov ss, ax
    mov gs, ax
    mov ebp, 0x9C00 ;far enough to not overflow previous stack/bootloader, 8192 above ORG
    mov esp, ebp

    ;access beyond 1MB
    in al, 0x92
    or al, 2
    out 0x92, al
    
    mov ebx, MSG_PROT_MODE
    call print_string_pm
    ;prints Hi at beginning
    ;mov dword [0xb8000], 0x07690748
    call move_cursor_to_origin


    ; 64 Bit
    ; Disable 32bit Paging if was enable
    CR0_PAGING equ 1 << 31
    mov eax, cr0
    and eax, ~CR0_PAGING
    mov cr0, eax

    ; Set PAE Enable in CR4
    CR4_PAE_ENABLE equ 1 << 5
    mov eax, cr4
    or eax, CR4_PAE_ENABLE
    mov cr4, eax
    
    ; Load CR3 with the address of the PML4
    ; Clear Tables
    PML4T_ADDR equ 0x1000
    SIZEOF_PAGE_TABLE equ 4096
    mov edi, PML4T_ADDR
    mov cr3, edi       ; cr3 lets the CPU know where the page tables are

    xor eax, eax
    mov ecx, SIZEOF_PAGE_TABLE
    rep stosd          ; writes 4 * SIZEOF_PAGE_TABLE bytes, which is enough space
                       ; for the 4 page tables
    mov edi, cr3       ; reset di back to the beginning of the page table
    
    ; Link first entries of each table
    PML4T_ADDR equ 0x1000
    PDPT_ADDR equ 0x2000
    PDT_ADDR equ 0x3000
    PT_ADDR equ 0x4000

    ; the page table only uses certain parts of the actual address
    PT_ADDR_MASK equ 0xffffffffff000
    PT_PRESENT equ 1                 ; marks the entry as in use
    PT_READABLE equ 2                ; marks the entry as r/w

    ; edi was previously set to PML4T_ADDR
    mov DWORD [edi], PDPT_ADDR & PT_ADDR_MASK | PT_PRESENT | PT_READABLE

    mov edi, PDPT_ADDR
    mov DWORD [edi], PDT_ADDR & PT_ADDR_MASK | PT_PRESENT | PT_READABLE

    mov edi, PDT_ADDR
    mov DWORD [edi], PT_ADDR & PT_ADDR_MASK | PT_PRESENT | PT_READABLE

    ; Fill Page Table
    ENTRIES_PER_PT equ 512
    SIZEOF_PT_ENTRY equ 8
    PAGE_SIZE equ 0x1000
    mov edi, PT_ADDR
    mov ebx, PT_PRESENT | PT_READABLE
    mov ecx, ENTRIES_PER_PT      ; 1 full page table addresses 2MiB

    .SetEntry:
        mov DWORD [edi], ebx
        add ebx, PAGE_SIZE
        add edi, SIZEOF_PT_ENTRY
        loop .SetEntry               ; Set the next entry.
    
    ; Enable long mode by setting the LME flag
    EFER_MSR equ 0xC0000080
    EFER_LM_ENABLE equ 1 << 8
    mov ecx, EFER_MSR
    rdmsr
    or eax, EFER_LM_ENABLE
    wrmsr
    
    ; Enable Paging and Protected Mode (already done though), compatibility mode
    CR0_PM_ENABLE equ 1 << 0
    CR0_PG_ENABLE equ 1 << 31
    mov eax, cr0
    or eax, CR0_PG_ENABLE | CR0_PM_ENABLE   ; ensuring that PM is set will allow for jumping
                                            ; from real mode to compatibility mode directly
    mov cr0, eax


    ; Copy kernel from 0x10000 to 0x100000 since loaded there before A20 gate, protected mode can access past 1MB now,
    ; does not work if kernel over 1MB in size
    mov esi, KERNEL_LOAD        ; Source
    mov edi, KERNEL_START_ADDR  ; Destination
    mov ecx, 16384              ; sectors * 512 bytes = 4096 bytes|8192|16384
    cld
    rep movsb

    ; Far jmp may leave compatibility mode
    lgdt [GDT.Pointer]
    jmp GDT.Code:LMmain
    ;jmp CODE_OFFSET:LMmain
    ; Best to setup long mode in bootloader first
    ;jmp CODE_OFFSET:KERNEL_START_ADDR

    jmp $

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

;section .rodata
GDT:
    dq 0
    .Code: equ $ - GDT
        dq (1<<44) | (1<<47) | (1<<41) | (1<<43) | (1<<53)
    .Data: equ $ - GDT
        dq (1<<44) | (1<<47) | (1<<41)
    .Pointer:
        dw .Pointer - GDT - 1
        dq GDT

; Setup GDT
; Access bits
;PRESENT        equ 1 << 7
;NOT_SYS        equ 1 << 4
;EXEC           equ 1 << 3
;DC             equ 1 << 2
;RW             equ 1 << 1
;ACCESSED       equ 1 << 0

; Flags bits
;GRAN_4K       equ 1 << 7
;SZ_32         equ 1 << 6
;LONG_MODE     equ 1 << 5

;GDT:
    ;.Null: equ $ - GDT
    ;    dq 0
    ;.Code: equ $ - GDT
    ;    .Code.limit_lo: dw 0xffff
    ;    .Code.base_lo: dw 0
    ;    .Code.base_mid: db 0
    ;    .Code.access: db PRESENT | NOT_SYS | EXEC | RW
    ;    .Code.flags: db GRAN_4K | LONG_MODE | 0xF   ; Flags & Limit (high, bits 16-19)
    ;    .Code.base_hi: db 0
    ;.Data: equ $ - GDT
    ;    .Data.limit_lo: dw 0xffff
    ;    .Data.base_lo: dw 0
    ;    .Data.base_mid: db 0
    ;    .Data.access: db PRESENT | NOT_SYS | RW
    ;    .Data.Flags: db GRAN_4K | SZ_32 | 0xF       ; Flags & Limit (high, bits 16-19)
    ;    .Data.base_hi: db 0
    ;.Pointer:
    ;    dw $ - GDT - 1
    ;    dq GDT

[BITS 64]
LMmain:
    ; Long Mode or Compatibility Mode(32bit allowed)?
    cli ; Ensure interupts disabled
    mov ax, DATA_OFFSET
    ;mov ax, GDT.Data
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ;prints "okay", 64bit
    ;mov rax, 0x2f592f412f4b2f4f
    ;mov qword [0xb8000], rax

    ;prints Hi at beginning, 32bit
    ;mov dword [0xb8000], 0x07690748

    ;jmp CODE_OFFSET:KERNEL_START_ADDR
    ; Cannot far jmp 64 to 64 bit address
    jmp KERNEL_START_ADDR
    
    jmp $

times 510-($ - $$) db 0 ;fill sector
dw 0xAA55 ;55AA but reversed, boot signature