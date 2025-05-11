BOOT = boot
KERNEL = kernel
CPU = cpu
DRIVERS = drivers
USER = user
BUILD = build
QEMU_TARGET = os.bin

# $(BUILD)/.o
FILES = $(BUILD)/kernel.asm.bin $(BUILD)/kernel.o $(BUILD)/idt.o $(BUILD)/pic.o $(BUILD)/keyboard.o $(BUILD)/images.o $(BUILD)/serials.o $(BUILD)/strings.o
INCLUDES = -I$(KERNEL) -I$(CPU) -I$(DRIVERS) -I$(USER)
FLAGS = -g -ffreestanding -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all:
	mkdir -p $(BUILD)


	nasm -f bin $(BOOT)/boot.asm -o $(BUILD)/boot.bin
	dd if=$(BUILD)/boot.bin of=$(BUILD)/boot.iso bs=512


	nasm -f bin $(BOOT)/boot2.asm -o $(BUILD)/boot2.bin
	dd if=$(BUILD)/boot2.bin of=$(BUILD)/boot2.iso bs=512


	nasm -f bin $(BOOT)/boot3.asm -o $(BUILD)/boot3.bin
	nasm -f elf -g $(KERNEL)/kernel.asm -o $(BUILD)/kernel.asm.bin
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $(KERNEL)/kernel.c -o $(BUILD)/kernel.o

	# i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $()/.c -o $(BUILD)/.o
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $(CPU)/idt.c -o $(BUILD)/idt.o
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $(CPU)/pic.c -o $(BUILD)/pic.o
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $(DRIVERS)/keyboard.c -o $(BUILD)/keyboard.o
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $(USER)/images.c -o $(BUILD)/images.o
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $(USER)/serials.c -o $(BUILD)/serials.o
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $(USER)/strings.c -o $(BUILD)/strings.o

	# Combine asm/c kernels
	i686-elf-ld -g -relocatable $(FILES) -o $(BUILD)/completeKernel.o
	i686-elf-gcc $(FLAGS) -T linkerScript.ld -o $(BUILD)/kernel.elf -ffreestanding -O0 -nostdlib $(BUILD)/completeKernel.o
	# Extract flat binary from kernel.elf (temporary workaround)
	objcopy -O binary $(BUILD)/kernel.elf $(BUILD)/kernel.bin
	# i686-elf-ld -T linkerScript.ld -o $(BUILD)/kernel.bin --oformat=binary $(BUILD)/completeKernel.o

	# Create disk image
	dd if=$(BUILD)/boot3.bin of=$(BUILD)/os.bin bs=512 conv=notrunc
	dd if=$(BUILD)/kernel.bin of=$(BUILD)/os.bin bs=512 seek=1 conv=notrunc
	# 8|16 sectors of zeros
	dd if=/dev/zero bs=512 count=16 >> $(BUILD)/os.bin

	xxd $(BUILD)/os.bin >> $(BUILD)/os.txt

qemu:
	# i386 or x86_64, -serial stdio sends serial to terminal
	#qemu-system-i386 -hda $(BUILD)/$(QEMU_TARGET)
	qemu-system-x86_64 -hda $(BUILD)/$(QEMU_TARGET) -serial stdio

qemu-nographic:
	# i386 or x86_64, -display none or -nographic -serial mon:stdio
	qemu-system-x86_64 -hda $(BUILD)/$(QEMU_TARGET) -display none

clean:
	rm $(BUILD)/*