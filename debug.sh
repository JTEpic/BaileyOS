#!/bin/bash
#wrong prints if qemu-system-x86_64 in 32bit, i386|x86_64
gdb -ex "add-symbol-file build/kernel.elf 0x100000" -ex "break kernel_main" -ex "target remote | qemu-system-i386 -hda build/os.bin -gdb stdio -S"
#gdb
#c
#add-symbol-file build/completeKernel.o 0x100000
#y
#break kernel_main
#target remote | qemu-system-x86_64 -hda build/os.bin -gdb stdio -S