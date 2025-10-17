BOOT = boot
KERNEL_FOLDER = kernel
CPU = cpu
DRIVERS = drivers
USER = user
BUILD = build
QEMU_TARGET = os2.bin

# Cross Compiler
PREFIX = $(HOME)/opt/cross
# i686-elf for 32bit or x86_64-elf for 64bit
TARGET=x86_64-elf
SRC_DIR = $(HOME)/src
BINUTILS_VER = 2.45
GCC_VER      = 15.2.0
J_THREADS	 = 6

# $(BUILD)/.o
FILES32 = $(BUILD)/kernel.asm.bin $(BUILD)/kernel.o $(BUILD)/idt.o $(BUILD)/pic.o $(BUILD)/keyboard.o $(BUILD)/images.o $(BUILD)/serials.o $(BUILD)/strings.o $(BUILD)/window.o $(BUILD)/editor.o
FILES64 = $(BUILD)/kernel2.asm.bin $(BUILD)/kernel2.o $(BUILD)/idt2.o $(BUILD)/pic2.o $(BUILD)/keyboard2.o $(BUILD)/images2.o $(BUILD)/serials2.o $(BUILD)/strings2.o $(BUILD)/window2.o $(BUILD)/editor2.o
INCLUDES = -I$(KERNEL_FOLDER) -I$(CPU) -I$(DRIVERS) -I$(USER)
FLAGS = -g -ffreestanding -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all:
	mkdir -p $(BUILD)


	# Bootloader 1
	nasm -f bin $(BOOT)/boot.asm -o $(BUILD)/boot.bin
	dd if=$(BUILD)/boot.bin of=$(BUILD)/boot.iso bs=512


	# Bootloader 2
	nasm -f bin $(BOOT)/boot2.asm -o $(BUILD)/boot2.bin
	dd if=$(BUILD)/boot2.bin of=$(BUILD)/boot2.iso bs=512


	# Bootloader 3, 32bit, requires cross compiled 32bit binutils+gcc
	#nasm -f bin $(BOOT)/boot3.asm -o $(BUILD)/boot3.bin
	#nasm -f elf -g $(KERNEL_FOLDER)/kernel.asm -o $(BUILD)/kernel.asm.bin
	#i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $(KERNEL_FOLDER)/kernel.c -o $(BUILD)/kernel.o

	# i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $()/.c -o $(BUILD)/.o
	#i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $(CPU)/idt.c -o $(BUILD)/idt.o
	#i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $(CPU)/pic.c -o $(BUILD)/pic.o
	#i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $(DRIVERS)/keyboard.c -o $(BUILD)/keyboard.o
	#i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $(USER)/lib/images.c -o $(BUILD)/images.o
	#i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $(USER)/lib/serials.c -o $(BUILD)/serials.o
	#i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $(USER)/lib/strings.c -o $(BUILD)/strings.o
	#i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $(USER)/window.c -o $(BUILD)/window.o
	#i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $(USER)/programs/editor.c -o $(BUILD)/editor.o

	# Combine asm/c kernels
	#i686-elf-ld -g -relocatable $(FILES32) -o $(BUILD)/completeKernel.o
	#i686-elf-gcc $(FLAGS) -T linkerScript.ld -o $(BUILD)/kernel.elf -ffreestanding -O0 -nostdlib $(BUILD)/completeKernel.o
	# Extract flat binary from kernel.elf (temporary workaround)
	#objcopy -O binary $(BUILD)/kernel.elf $(BUILD)/kernel.bin
	## i686-elf-ld -T linkerScript.ld -o $(BUILD)/kernel.bin --oformat=binary $(BUILD)/completeKernel.o

	# Create disk image
	#dd if=$(BUILD)/boot3.bin of=$(BUILD)/os.bin bs=512 conv=notrunc
	#dd if=$(BUILD)/kernel.bin of=$(BUILD)/os.bin bs=512 seek=1 conv=notrunc
	# 8|16 sectors of zeros
	#dd if=/dev/zero bs=512 count=16 >> $(BUILD)/os.bin

	#xxd $(BUILD)/os.bin >> $(BUILD)/os.txt


	# Bootloader 4, 64bit, requires cross compiled 64bit binutils+gcc
	nasm -f bin $(BOOT)/boot4.asm -o $(BUILD)/boot4.bin
	nasm -f elf -g $(KERNEL_FOLDER)/kernel2.asm -o $(BUILD)/kernel2.asm.bin
	x86_64-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $(KERNEL_FOLDER)/kernel.c -o $(BUILD)/kernel2.o

	# x86_64-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $()/.c -o $(BUILD)/.o
	x86_64-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $(CPU)/idt.c -o $(BUILD)/idt2.o
	x86_64-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $(CPU)/pic.c -o $(BUILD)/pic2.o
	x86_64-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $(DRIVERS)/keyboard.c -o $(BUILD)/keyboard2.o
	x86_64-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $(USER)/lib/images.c -o $(BUILD)/images2.o
	x86_64-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $(USER)/lib/serials.c -o $(BUILD)/serials2.o
	x86_64-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $(USER)/lib/strings.c -o $(BUILD)/strings2.o
	x86_64-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $(USER)/window.c -o $(BUILD)/window2.o
	x86_64-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $(USER)/programs/editor.c -o $(BUILD)/editor2.o

	# Combine asm/c kernels
	x86_64-elf-ld -g -relocatable $(FILES64) -o $(BUILD)/completeKernel2.o
	x86_64-elf-gcc $(FLAGS) -T linkerScript.ld -o $(BUILD)/kernel2.elf -ffreestanding -O0 -nostdlib $(BUILD)/completeKernel2.o
	# Extract flat binary from kernel2.elf (temporary workaround)
	objcopy -O binary $(BUILD)/kernel2.elf $(BUILD)/kernel2.bin
	# x86_64-elf-ld -T linkerScript.ld -o $(BUILD)/kernel2.bin --oformat=binary $(BUILD)/completeKernel2.o

	# Create disk image
	dd if=$(BUILD)/boot4.bin of=$(BUILD)/os2.bin bs=512 conv=notrunc
	dd if=$(BUILD)/kernel2.bin of=$(BUILD)/os2.bin bs=512 seek=1 conv=notrunc
	# 8|16 sectors of zeros
	dd if=/dev/zero bs=512 count=16 >> $(BUILD)/os2.bin

	xxd $(BUILD)/os2.bin >> $(BUILD)/os2.txt

qemu:
	# i386 or x86_64, -serial stdio sends serial to terminal
	#qemu-system-i386 -hda $(BUILD)/$(QEMU_TARGET)
	qemu-system-x86_64 -hda $(BUILD)/$(QEMU_TARGET) -serial stdio

qemu-nographic:
	# i386 or x86_64, -display none or -nographic -serial mon:stdio
	qemu-system-x86_64 -hda $(BUILD)/$(QEMU_TARGET) -display none

# Does not delete src folder, each line multiple shells so && \ required
cross-compiler:
	# export PREFIX="$(PREFIX)"
	# export TARGET=$(TARGET)
	# export PATH="$(PREFIX)/bin:$(PATH)"

	# Create build directories
	mkdir -p $(PREFIX)

	# For Binutils:
	export PREFIX="$(PREFIX)" && \
	export TARGET=$(TARGET) && \
	export PATH="$(PREFIX)/bin:$(PATH)" && \
	cd $(SRC_DIR) && \
	mkdir build-binutils && \
	cd $(SRC_DIR)/build-binutils && \
	../binutils-$(BINUTILS_VER)/configure --target=$(TARGET) --prefix="$(PREFIX)" --with-sysroot --disable-nls --disable-werror && \
	make -j $(J_THREADS) && \
	make install

	# For GCC:
	# The $PREFIX/bin dir _must_ be in the PATH. We did that above.
	export PREFIX="$(PREFIX)" && \
	export TARGET=$(TARGET) && \
	export PATH="$(PREFIX)/bin:$(PATH)" && \
	cd $(SRC_DIR) && \
	which -- $(TARGET)-as || (echo $(TARGET)-as is not in the PATH && exit 1) && \
	mkdir build-gcc && \
	cd $(SRC_DIR)/build-gcc && \
	../gcc-$(GCC_VER)/configure --target=$(TARGET) --prefix="$(PREFIX)" --disable-nls --enable-languages=c,c++ --without-headers && \
	make -j $(J_THREADS) all-gcc && \
	make -j $(J_THREADS) all-target-libgcc && \
	make install-gcc && \
	make install-target-libgcc
	# --disable-hosted-libstdcxx
	# make all-target-libstdc++-v3
	# make install-target-libstdc++-v3

	# Delete $(SRC_DIR) folder

clean:
	# -f
	rm $(BUILD)/*