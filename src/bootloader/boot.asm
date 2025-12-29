;  multiboot header + minimal startup  (NASM syntax)
;  assemble:  nasm -f elf32 boot.asm

%define ALIGN    1<<0
%define MEMINFO  1<<1
%define FLAGS    ALIGN | MEMINFO
%define MAGIC    0x1BADB002
%define CHECKSUM -(MAGIC + FLAGS)

section .multiboot
align 4
multiboot_header:
    dd 0x1BADB002          ; Magic number
    dd 0x00000003          ; Flags: align + meminfo
    dd -(0x1BADB002 + 0x00000003)  ; Checksum
section .bss
align 16
stack_bottom:
    resb 16384          ; 16 KiB
stack_top:

section .text
global _start
extern _init          ; your kernel entry (C)

_start:
    cli
    mov esp, stack_top
    call _init          ; kernel main

    cli
.hang:
    hlt
    jmp .hang