gdb -ex "add-symbol-file build/completeKernel.o 0x100000" -ex "break kernel_main" -ex "target remote | qemu-system-x86_64 -hda build/os.bin -gdb stdio -S"
#gdb
#c
#add-symbol-file build/completeKernel.o 0x100000
#y
#break kernel_main
#target remote | qemu-system-x86_64 -hda build/os.bin -gdb stdio -S