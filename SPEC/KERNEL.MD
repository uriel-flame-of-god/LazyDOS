## SPEC/KERNEL.MD

# LazyDOS Kernel - The Simple Core

## The Philosophy
Like early DOS kernels, we provide just enough to run programs. No multitasking, no memory protection, no complexity. Just a simple platform for simple programs.

## What Our Kernel Does
1. **Sets up the screen** (80x25 text mode)
2. **Sets up memory** (simple GDT)
3. **Starts the shell** (our TTY)
4. **That's it**

## Kernel Architecture (Simple)
```
Boot → Kernel → TTY Shell → Programs
```

No layers, no microkernels, no services. Just function calls.

## Our Simple Components
1. **VGA Driver**: Put characters on screen
2. **Keyboard Driver**: Get characters from keyboard
3. **GDT**: Simple memory segmentation
4. **TTY**: Command interpreter
5. **Port I/O**: Talk to hardware

## What We Don't Have (And Don't Need Yet)
- Filesystem
- Networking
- Multitasking
- User accounts
- Security
- Graphics beyond text
- Sound

## The Kernel Entry Point
```c
void _init(void) {
    terminal_initialize();  // Setup screen
    init_gdt();             // Setup memory
    tty_main();             // Start shell (never returns)
}
```

See? Simple.

## Memory Model (Also Simple)
- **Kernel**: Loaded by bootloader
- **Stack**: 16KB we allocated
- **Everything else**: Free for use

No virtual memory, no paging, no swapping. Just physical memory.

## Error Handling (Minimal)
- If something fails, we print a message
- If it's really bad, we halt
- No recovery, no retry, just fail

## Why This Simplicity Works
1. **Understandable**: You can read all the code in an hour
2. **Reliable**: Fewer parts, fewer failures
3. **DOS-like**: This is how early systems worked
4. **Educational**: You can see how everything fits together

## Testing Philosophy
If it boots and shows a prompt, it works.

## Future (Maybe)
We might add:
- Simple filesystem (LazyFS)
- Basic interrupts
- Maybe a simple scheduler

But only if it keeps the simplicity.

**LazyDOS Principle**: A kernel should be a simple foundation. Ours is.

