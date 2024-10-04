#!/bin/bash
nasm -f bin boot/boot.asm -o build/boot.bin
nasm -f bin boot/boot2.asm -o build/boot2.bin
nasm -f bin boot/boot3.asm -o build/boot3.bin
dd if=build/boot.bin of=build/boot.iso bs=512
dd if=build/boot2.bin of=build/boot2.iso bs=512
dd if=build/boot3.bin of=build/boot3.iso bs=512