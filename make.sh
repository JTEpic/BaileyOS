#!/bin/bash
nasm -f bin boot/boot.asm -o build/boot.bin
nasm -f bin boot/boot2.asm -o build/boot2.bin
dd if=build/boot.bin of=build/boot.iso bs=512
dd if=build/boot2.bin of=build/boot2.iso bs=512