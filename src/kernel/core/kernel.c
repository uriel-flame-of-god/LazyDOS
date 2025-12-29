/* kernel.c  –  32-bit kernel entry, launches integrated TTY shell */
#include "../io/vga.h"
#include "../core/gdt.h"
#include "../core/tty.h"          /* new: integrated shell */

static void klog(int level, const char *msg)
{
    const char *prefix[] = { "", "OK   ", "WARN ", "FAIL " };
    enum vga_color colors[] = {
        0, VGA_COLOR_GREEN, VGA_COLOR_LIGHT_RED, VGA_COLOR_RED
    };

    terminal_setcolor(colors[level]);
    terminal_writestring(prefix[level]);
    terminal_setcolor(VGA_COLOR_LIGHT_GREY);
    terminal_writestring(msg);
}

/* ---------- C entry point called from ASM ---------- */
void _init(void)
{
    terminal_initialize();
    klog(1, "Terminal ready");
    terminal_putchar('\n');
    init_gdt();
    klog(1, "GDT loaded");
    terminal_putchar('\n');
    terminal_writestring("Welcome to LazyDOS v0.0.1!\n");
    /* start the built-in interactive shell */
    tty_main();          /* never returns */

    /* should never reach here, but hang safely if we do */
    klog(3, "Kernel panic: TTY returned\n");
    for (;;) asm volatile ("hlt");
}