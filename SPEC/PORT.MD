## SPEC/PORT.MD

# LazyDOS Port I/O - Talking to Hardware Directly

## The Philosophy
Like early DOS, we talk to hardware directly through I/O ports. No drivers, no abstraction layers, just simple `inb` and `outb`.

## What Are I/O Ports?
They're like hardware mailboxes:
- `outb`: Put a byte in a mailbox
- `inb`: Get a byte from a mailbox

## Our Simple API
```c
void outb(uint16_t port, uint8_t val);  // Send a byte
uint8_t inb(uint16_t port);             // Receive a byte
```

That's it. Two functions. No configuration, no initialization, just talk to hardware.

## Common Hardware We Talk To
| Hardware | Port | What We Do |
|----------|------|------------|
| **VGA** | 0x3D4/0x3D5 | Move the cursor |
| **Keyboard** | 0x60/0x64 | Read keys |
| **PIC** | 0x20/0xA0 | (Future) Handle interrupts |

## Why Direct I/O?
1. **Simplicity**: No driver layers
2. **Speed**: Direct hardware access
3. **Control**: We know exactly what's happening
4. **DOS Tradition**: This is how DOS did it

## The Implementation (It's Simple)
```c
void outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
    // That's literally one assembly instruction
}
```

## Safety Considerations (We're Simple Here)
- We assume you know what you're doing
- Wrong port? Might crash, that's on you
- No error checking (that's not our job)

## Testing
If the keyboard works and the screen shows text, ports work.

## What We Don't Do
- No port validation
- No interrupt masking
- No DMA setup
- No PCI configuration

**LazyDOS Principle**: Hardware should be accessible, not hidden. Our port I/O makes it accessible.
