/* gdt.h  –  32-bit GDT/TSS interface */
#ifndef GDT_H
#define GDT_H

#include <stdint.h>

/* 8-byte GDT descriptor */
struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed));

/* GDTR pointer */
struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

/* ---------- assembly helpers ---------- */
extern void gdt_flush(uint32_t gdt_ptr);   /* lgdt + reload segs */
extern void tss_flush(uint16_t sel);       /* ltr  */

/* ---------- C API ---------- */
void init_gdt(void);                /* build and load the GDT */
void set_kernel_stack(uint32_t stack); /* update tss.esp0 */

#endif /* GDT_H */