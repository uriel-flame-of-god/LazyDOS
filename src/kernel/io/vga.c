/* vga.c  –  VGA text-mode driver (80×25) */
#include "../io/vga.h"
#include "../lib/string.h"
#include "../io/port.h"

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEM     0xB8000

static uint16_t *const BUFFER = (uint16_t *)VGA_MEM;

static size_t  term_row;
static size_t  term_col;
static uint8_t term_color;

/* ---------- cursor helpers ---------- */
static void update_cursor(void)
{
    uint16_t pos = term_row * VGA_WIDTH + term_col;
    outb(0x3D4, 0x0F); outb(0x3D5, pos & 0xFF);
    outb(0x3D4, 0x0E); outb(0x3D5, (pos >> 8) & 0xFF);
}

/* ---------- scroll one line up ---------- */
static void scroll(void)
{
    memmove(BUFFER,
            BUFFER + VGA_WIDTH,
            (VGA_HEIGHT - 1) * VGA_WIDTH * sizeof(uint16_t));
    /* clear bottom line */
    uint16_t blank = vga_entry(' ', term_color);
    for (size_t x = 0; x < VGA_WIDTH; ++x)
        BUFFER[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = blank;
}

/* ---------- public API ---------- */
void terminal_initialize(void)
{
    term_row   = 0;
    term_col   = 0;
    term_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

    uint16_t blank = vga_entry(' ', term_color);
    for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i)
        BUFFER[i] = blank;
    update_cursor();
}

void terminal_setcolor(uint8_t color)
{
    term_color = color;
}

void terminal_putchar(char c)
{
    if (c == '\n') {
        term_col = 0;
        ++term_row;
    } else {
        BUFFER[term_row * VGA_WIDTH + term_col] = vga_entry(c, term_color);
        if (++term_col == VGA_WIDTH) {
            term_col = 0;
            ++term_row;
        }
    }
    if (term_row == VGA_HEIGHT) {
        scroll();
        --term_row;
    }
    update_cursor();
}

void terminal_write(const char *data, size_t size)
{
    for (size_t i = 0; i < size; ++i)
        terminal_putchar(data[i]);
}

void terminal_writestring(const char *str)
{
    terminal_write(str, strlen(str));
}
