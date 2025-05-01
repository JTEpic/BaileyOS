BITS 32

global _start
extern kernel_main

VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f ; the color byte for each character

_start:
    ;mov eax, 0xb8000
    ;mov byte [eax], 'C' ; Character
    ;mov byte [eax + 1], 0x4F ; Attribute byte
    ;mov dword [0xb8000], 0x07690748
    
    mov ebx, MSG_PROT_MODE
    call print_string_pm
    
    call kernel_main
    
    jmp $

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

MSG_PROT_MODE db "Printing in Kernel", 0

times 510-($ - $$) db 0