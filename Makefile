BOOT = boot
BUILD = build
QEMU_TARGET = boot3

all: $(NAMES)
	mkdir -p $(BUILD)

	nasm -f bin $(BOOT)/boot.asm -o $(BUILD)/boot.bin
	dd if=$(BUILD)/boot.bin of=$(BUILD)/boot.iso bs=512

	nasm -f bin $(BOOT)/boot2.asm -o $(BUILD)/boot2.bin
	dd if=$(BUILD)/boot2.bin of=$(BUILD)/boot2.iso bs=512

	nasm -f bin $(BOOT)/boot3.asm -o $(BUILD)/boot3.bin
	dd if=$(BUILD)/boot3.bin of=$(BUILD)/boot3.iso bs=512

qemu:
	qemu-system-x86_64 -hda $(BUILD)/$(QEMU_TARGET).bin

clean:
	rm $(BUILD)/*