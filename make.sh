#!/bin/bash
make clean
# i686-elf for 32bit or x86_64-elf for 64bit
export TARGET=x86_64-elf
export PATH="$HOME/opt/cross/bin:$PATH"
make all
make qemu