## SPEC/BOOTLOADER.MD

# LazyDOS Bootloader - Simple and Direct

## Philosophy
Like early DOS bootloaders, we keep it minimal: just enough to get the kernel loaded and running. No complex features, no unnecessary abstractions.

## What It Does (The Simple Version)
1. **Sets up a Multiboot header** - So GRUB/QEMU knows how to load us
2. **Allocates a stack** - 16KB, that's plenty
3. **Jumps to the kernel** - That's it, job done

## What It Doesn't Do
- No filesystem parsing
- No memory detection (Multiboot handles that)
- No fancy graphics modes
- No error recovery (if it fails, it fails)

## Memory Layout (Simple)
```
0x00000000 - 0x000FFFFF: Kernel (where GRUB puts it)
Bottom of memory: Our 16KB stack
Top of memory: Free for kernel to use
```

## Why It Works
- **Multiboot standard**: Everyone supports it
- **Fixed stack size**: We know what we need
- **No magic**: Everything is explicit

## Testing
Works with:
- QEMU (our main development platform)
- GRUB (if you really want to)
- Anything Multiboot compliant

## The Code (Just Look)
```nasm
; It's all right there in boot.asm
; No hidden complexity, just:
; 1. Multiboot header
; 2. Stack space
; 3. Jump to kernel
```

**LazyDOS Principle**: Bootloaders should be boring. Ours is.
