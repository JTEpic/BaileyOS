BOOT = boot
KERNEL = kernel
BUILD = build
QEMU_TARGET = os.bin

FILES = $(BUILD)/kernel.asm.o $(BUILD)/kernel.o
FLAGS = -g -ffreestanding -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all:
	mkdir -p $(BUILD)

	nasm -f bin $(BOOT)/boot.asm -o $(BUILD)/boot.bin
	dd if=$(BUILD)/boot.bin of=$(BUILD)/boot.iso bs=512

	nasm -f bin $(BOOT)/boot2.asm -o $(BUILD)/boot2.bin
	dd if=$(BUILD)/boot2.bin of=$(BUILD)/boot2.iso bs=512

	nasm -f bin $(BOOT)/boot3.asm -o $(BUILD)/boot3.bin
	nasm -f elf -g $(KERNEL)/kernel.asm -o $(BUILD)/kernel.asm.o
	i686-elf-gcc -I$(KERNEL) $(FLAGS) -std=gnu99 -c $(KERNEL)/kernel.c -o $(BUILD)/kernel.o
	i686-elf-ld -g -relocatable $(FILES) -o $(BUILD)/completeKernel.o
	i686-elf-gcc $(FLAGS) -T linkerScript.ld -o $(BUILD)/kernel.bin -ffreestanding -O0 -nostdlib $(BUILD)/completeKernel.o
	dd if=$(BUILD)/boot3.bin >> $(BUILD)/os.bin
	dd if=$(BUILD)/kernel.bin >> $(BUILD)/os.bin
	dd if=/dev/zero bs=512 count=8 >> $(BUILD)/os.bin

#nasm -f bin $(BOOT)/boot3.asm -o $(BUILD)/boot3.bin
#dd if=$(BUILD)/boot3.bin of=$(BUILD)/boot3.iso bs=512

qemu:
	qemu-system-x86_64 -hda $(BUILD)/$(QEMU_TARGET)

clean:
	rm $(BUILD)/*