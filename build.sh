#!/bin/bash
echo "=== Building kernel ==="
make clean
make

echo -e "\n=== Creating ISO ==="
cp kernel.elf iso/boot/
rm -f betterdos.iso
grub2-mkrescue -o betterdos.iso iso/ 2>/dev/null
