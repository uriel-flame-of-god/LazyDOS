
## SPEC/GDT.MD

# LazyDOS GDT - Memory Segmentation Made Simple

## The Philosophy
Early x86 systems used segmentation. We use it too - not because it's fancy, but because it's simple and gets the job done.

## What's a GDT? (Simple Explanation)
It's like giving the CPU a map of memory. Our map says: "Everything is at address 0, everything is 4GB, just use it."

## Our Simple GDT Layout
1. **Null entry** (0x00): Required, does nothing
2. **Kernel code** (0x08): Where kernel instructions live
3. **Kernel data** (0x10): Where kernel data lives
4. **User code** (0x18): For future user programs
5. **User data** (0x20): For future user programs
6. **TSS** (0x28): A simple task structure

## Why This Simplicity Works
- **Flat model**: No complex address calculations
- **4GB everything**: No worrying about limits
- **Two privilege levels**: Kernel (can do anything) and User (limited)

## The TSS (Task State Segment)
It's just a structure that holds:
- Where the kernel stack is
- Some CPU state
- That's mostly it

We don't use full task switching (too complex), we just use it for stack switching.

## Implementation Notes
```c
// Setting up a GDT entry is straightforward:
void set_gdt_entry(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt[num].base_low    = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    // ... etc, it's just splitting up the address
}
```

## What We Don't Do
- No paging (too complex for now)
- No fancy protection schemes
- No segment limit checking
- No LDTs (Local Descriptor Tables)

## Testing
If the kernel runs, the GDT works. That's our test.

**LazyDOS Principle**: Use simple, understandable memory management. Our GDT is exactly that.
