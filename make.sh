#!/bin/bash
make clean
export TARGET=i686-elf
export PATH="$HOME/opt/cross/bin:$PATH"
make all
make qemu