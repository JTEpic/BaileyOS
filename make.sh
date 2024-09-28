#!/bin/bash
nasm -f bin boot/boot.asm -o build/boot.bin
dd if=build/boot.bin of=build/boot.iso bs=2048