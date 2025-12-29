/* linker.ld */
ENTRY(_start)

SECTIONS
{
    . = 1M;  /* Kernel loaded at 1MB */

    .multiboot : {
        *(.multiboot)
    }

    .text : {
        *(.text)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
    }
}