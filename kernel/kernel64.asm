[BITS 64]

global _start
extern kernel_main
extern handle_default_interrupt
extern handle_keyboard_interrupt

VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f ; the color byte for each character

_start:
    ; Clear Screen
    mov     rdi, 0xB8000          ; destination = VGA text buffer
    mov     rcx, 80*25            ; number of character cells (2000)
    mov     rax, 0x0720           ; attribute+space = 0x07 (white on black) << 8 | 0x20 (space)
                                   ; low byte = space, high byte = attribute
    ; rep stosw writes AX to [RDI] and increments RDI by 2 each iteration
    rep     stosw                ; clear the screen

    ;mov eax, 0xb8000
    ;mov byte [eax], 'C' ; Character
    ;mov byte [eax + 1], 0x4F ; Attribute byte
    ;mov dword [0xb8000], 0x07690748
    
    mov rbx, MSG_KERNEL_MODE
    call print_string_lm
    
    ; Call/Enter Kernel
    call kernel_main

    cli
    hlt ;hlt cpu indefinitely since no interrupts

    jmp $

print_string_lm:
    ;pusha ; 32 bit, pushes all general‑purpose registers onto the stack, EAX to EDI
    ;mov edx, VIDEO_MEMORY
    push rbx
    push rdx
    mov rdx, VIDEO_MEMORY
print_string_lm_loop:
    ;mov al, [ebx] ; [ebx] is the address of our character
    mov al, [rbx]
    mov ah, WHITE_ON_BLACK
    cmp al, 0 ; check if end of string
    je print_string_lm_done
    ;mov [edx], ax ; store character + attribute in video memory
    ;add ebx, 1 ; next char
    ;add edx, 2 ; next video memory position
    mov[rdx], ax
    inc rbx
    add rdx, 2
    jmp print_string_lm_loop
print_string_lm_done:
    ;popa ; 32 bit, restores registers in reverse order
    pop rdx
    pop rbx
    ret

MSG_KERNEL_MODE db "Printing in Kernel", 0

; Default handler
global default_handler
default_handler:
    ;mov dword [0xb8000], 0x07690748

    ;pushad
    ; Save 64 bit registers
    push rax
    push rcx
    push rdx
    push rbx
    push rbp
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    call handle_default_interrupt

    ;popad
    ; Restore 64 bit registers
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rbp
    pop rbx
    pop rdx
    pop rcx
    pop rax

    iret

; Keyboard interrupt handler, irq1
global keyboard_handler
keyboard_handler:
    ;cli
    ;mov dword [0xb8000], 0x07690748

    ;pushad
    push rax
    push rcx
    push rdx
    push rbx
    push rbp
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    call handle_keyboard_interrupt

    ;popad
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rbp
    pop rbx
    pop rdx
    pop rcx
    pop rax
    ;sti

    iret

times 510-($ - $$) db 0