ENTRY(_start)
OUTPUT_FORMAT(elf64-x86-64) #binary or elf32-i386 or elf64-x86-64
#OUTPUT_ARCH(x86_64?) #i386 or x86_64?
SECTIONS
{
    . = 0x100000;
    # align power of 2, higher may require sector increase
    .text : ALIGN(16)
    {
        *(.text)
    }

    .rodata : ALIGN(16)
    {
        *(.rodata)
    }

    .data : ALIGN(16)
    {
        *(.data)
    }

    .bss : ALIGN(16)
    {
        *(COMMON)
        *(.bss)
    }
}