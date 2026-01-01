# Makefile  –  builds 32-bit kernel.elf from tree under src/
CC	:= gcc
AS	:= nasm
LD	:= ld

CFLAGS	:= -m32 -ffreestanding -fno-pie -fno-stack-protector -Wall -Wextra
CFLAGS	+= -I src/kernel/io -I src/kernel/core -I src/kernel/lib
ASFLAGS	:= -f elf32
LDFLAGS	:= -T linker.ld -nostdlib -static -m elf_i386

OBJS	:= src/bootloader/boot.o \
	   src/kernel/core/kernel.o \
	   src/kernel/core/gdt.o \
	   src/kernel/core/tty.o \
	   src/kernel/apps/calculator.o \
	   src/kernel/io/keyboard.o \
	   src/kernel/io/vga.o \
	   src/kernel/io/port.o \
	   src/kernel/lib/string.o \
	   src/kernel/lib/int.o \
	   src/kernel/lib/float.o \
	   src/kernel/apps/qbasic.o

all: kernel.elf

kernel.elf: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

iso:
	./build.sh

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.asm
	$(AS) $(ASFLAGS) -o $@ $<

clean:
	rm -f $(OBJS) kernel.elf

.PHONY: all clean iso