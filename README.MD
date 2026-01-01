# LazyDOS - The "Elegantly Simple" Operating System

![LazyDOS Logo](https://img.shields.io/badge/LazyDOS-v0.0.5-lazy)
![License: GPL v3.0](https://img.shields.io/badge/License-GPLv3-blue.svg)
![Philosophy: Simplicity](https://img.shields.io/badge/Philosophy-Simplicity-blue.svg)
![Inspiration: MS-DOS](https://img.shields.io/badge/Inspiration-MS--DOS-00ff00.svg)
![Calculator: Working!](https://img.shields.io/badge/Calculator-✅_Working-green.svg)

> "Why make it complicated when simple works?"
> 
> *Inspired by the beautiful simplicity of MS-DOS*

- Stable TTY
- LazyCalculator (Integer Math)
- QBASIC MINIMAL interpreter
- Simple control flow (IF/THEN/ELSE)
- LET variable assignment
- PRINT statement support
- No crashes, no reboots
- Stable TTY
- Working LazyCalculator
- Clean integer math
- No spontaneous reboots


## 📋 Table of Contents
- [The LazyDOS Philosophy](#the-lazydos-philosophy)
- [Name Origin](#-name-origin-not-laziness-but-simplicity)
- [What's New in v0.0.3](#-whats-new-in-v003)
- [🧮 LazyCalculator: The Star of v0.0.3](#-lazydos-applications)
- [System Requirements](#system-requirements)
- [Quick Start](#quick-start)
- [Building from Source](#building-from-source)
- [Running the System](#running-the-system)
- [Architecture Overview](#architecture-overview)
- [Known Issues](#known-issues)
- [Current Status](#-current-status-what-works-now)
- [Contributing (If You Must)](#contributing-if-you-must)
- [FAQ (Frequently Avoided Questions)](#faq-frequently-avoided-questions)
- [License](#license)

## 🦥 The LazyDOS Philosophy

LazyDOS is built on one simple principle: **Elegant simplicity over unnecessary complexity**. Unlike other operating systems that bury you in abstractions, LazyDOS embraces directness. Here's what that means:

- **Direct hardware access**: Talk to the hardware like in the good old days
- **Understandable code**: You can read and understand the entire system
- **No bloat**: If it's not essential, it's not included
- **Developer freedom**: The system gets out of your way so you can do real work

We believe in **"Minimalist Open Source"** - we provide a clean, simple foundation that you can understand and build upon. It's not about being lazy; it's about being efficient!

## 🏷️ Name Origin: Not Laziness, But Simplicity

**Contrary to what you might think, LazyDOS isn't named because we're lazy developers.** The name comes from our admiration of **MS-DOS** and its beautifully simple architecture. Here's the real story:

### The MS-DOS Inspiration
Back in the golden age of computing (when memory was measured in kilobytes and computers beeped at you), MS-DOS showed us that an operating system could be:
- **Simple**: No complex abstractions getting in your way
- **Direct**: You talk to the hardware, the hardware talks back
- **Understandable**: You could hold the entire system in your head
- **Practical**: It got the job done with minimal fuss

### Why "Lazy"DOS?
We call it **LazyDOS** because:
1. **MS-DOS was "lazy" in a good way** - it didn't overcomplicate things
2. **It gets out of your way** - like a minimalist butler who knows when not to bother you
3. **It embraces simplicity** - the "laziness" of elegant design
4. **It's approachable** - you don't need a PhD to understand it

Think of it as **MS-DOS's minimalist cousin who woke up in the 21st century**. We're not lazy; we're *efficiently minimalist*.

## 🎉 What's New in v0.0.3

### Major Feature: LazyCalculator is Born! 🧮
The broken calculator from previous versions has been completely reimagined and reborn as **LazyCalculator** - a simple, reliable, integer-only calculator that actually works!

## 🧮 LazyDOS Applications

### LazyCalculator (NEW in v0.0.3)
Meet LazyCalculator - the simple, reliable calculator that does exactly what you need and nothing more:

#### **Key Features:**
- **Integer-only operations**: No floating-point complexity
- **Left-to-right evaluation**: Simple and predictable
- **Basic arithmetic**: `+` `-` `*` `/` - all the essentials
- **Safe division**: Division by zero protection built-in
- **No crashes**: Proper error handling throughout
- **No reboots needed**: Stable and reliable
- **Clean interface**: Type `calc` and start calculating

#### **How to Use:**
```
LazyDOS> calc
Welcome to LazyCalculator v1.0
Enter expressions like: 5 + 3 * 2
Type 'exit' to return to shell

calc> 10 + 5 * 2
Result: 30

calc> 20 / 4
Result: 5

calc> 10 / 0
Error: Division by zero!

calc> exit
Returning to LazyDOS...
```

### QBASIC Minimal Interpreter

LazyDOS includes a minimal QBASIC interpreter for simple programming:
#### **How to Use:**
```
LazyDOS> qbasic
Welcome to LazyBASIC v1.0
Type your BASIC program commands:

> LET x = 5
> LET y = 10
> PRINT x + y
> Ctrl+R

Result:
15

> Ctrl+X
Returning to LazyDOS...
```

#### **Supported Commands:**
- `LET` - Variable assignment
- `PRINT` - Output text and values
- `IF/THEN/ELSE` - Simple control flow
- `GOTO` - Jump to line numbers

#### **Keyboard Shortcuts:**
- **Ctrl+R**: Run the program
- **Ctrl+X**: Exit and return to shell

#### **Design Philosophy:**
Minimal, simple BASIC for educational purposes - not a full implementation, but enough to run classic simple programs.

#### **Design Philosophy:**
LazyCalculator embodies the LazyDOS philosophy:
1. **Simple**: No complex precedence rules (strict left-to-right)
2. **Reliable**: No crashes, proper error messages
3. **Understandable**: You can read the entire source in 5 minutes
4. **Useful**: Does the job without unnecessary features

### Project Structure Cleanup
```
src/
├── bootloader/
│   └── boot.asm
└── kernel/
    ├── apps/
    │   ├── calculator.c    # ✅ LazyCalculator - Simple & Working!
    │   └── calculator.h
    |   ├── qbasic.c # ✅ QBASIC Minimal - Working
    |   └── qbasic.h
    ├── core/
    │   ├── gdt.c
    │   ├── gdt.h
    │   ├── kernel.c
    │   ├── tty.c
    │   └── tty.h
    ├── io/
    │   ├── keyboard.c
    │   ├── keyboard.h
    │   ├── port.c
    │   ├── port.h
    │   ├── vga.c
    │   └── vga.h
    └── lib/
        ├── float.c
        ├── float.h
        ├── int.c
        ├── int.h
        ├── string.c
        └── string.h
```

### Enhanced Stability
- Improved TTY input handling
- Better error messages throughout the system
- More robust command parsing
- Proper integer overflow handling in calculator

## 🖥️ System Requirements

### Minimal (It Will Boot)
- **CPU**: i486 or better (or worse, we don't judge)
- **RAM**: 640 KB conventional memory (we're nostalgic like that)
- **Bootloader**: Anything that supports Multiboot 1.0 (or pretends to)
- **Patience**: A moderate amount

### Recommended (It Will Work Well)
- **CPU**: Anything made after 1995
- **RAM**: At least 1 MB (so we have somewhere to put our stack)
- **Emulator**: QEMU or Bochs (because why not?)
- **Expectations**: Realistic but positive
- **Nostalgia**: For the days when software was understandable

## 🚀 Quick Start

### For the Impatient

```bash
# Clone the repository
git clone https://github.com/uriel-flame-of-god/LazyDOS.git
cd lazydos

# Build it (it actually works now)
make

# Run it and try the new calculator!
make run

# Once booted, type:
LazyDOS> calc
```

## 🔨 Building from Source

### Step 1: Get the Tools

You'll need:
- **NASM** (for the bootloader)
- **GCC Cross-Compiler** (i686-elf target)
- **Make** (or patience to run commands manually)

### Step 2: The Build Process

```bash
# The easy way (recommended)
make all

# Or step by step
nasm -f elf32 src/bootloader/boot.asm -o boot.o
i686-elf-gcc -c src/kernel/core/kernel.c -o kernel.o -ffreestanding -O2
# ... and so on
```

### Step 3: Create the ISO

```bash
# Create bootable ISO
make iso

# Output: BetterDOS.iso
# Size: Compact and efficient, just like DOS
```

## 🏃 Running the System

### In QEMU (Recommended)

```bash
# Basic run
make run

# With debug output
make debug

# With custom memory
qemu-system-i386 -cdrom BetterDOS.iso -m 128M
```

### In Bochs (For Authenticity)

```bash
# Configure and run
bochs -q -f bochsrc.txt
```

### On Real Hardware (Adventurous)
1. Burn ISO to USB/CD
2. Boot from it
3. Enjoy the simplicity (and try the calculator!)

## 🏗️ Architecture Overview

### Boot Process (Simple and Reliable)
1. **boot.asm**: Multiboot header + basic setup
2. **GDT**: Global Descriptor Table (works properly)
3. **Kernel Entry**: Clean jump to C code

### Kernel Components

#### Core (Functional and Clean)
- **kernel.c**: Main entry point, initializes everything
- **gdt.c**: Segmentation setup (simple but effective)
- **tty.c**: Integrated shell (where the magic happens)

#### I/O Drivers (They Work)
- **vga.c**: Text mode driver (80×25, classic and reliable)
- **keyboard.c**: PS/2 keyboard driver (responsive)
- **port.c**: I/O port helpers (actually useful)

#### Applications (Useful and Working!)
- **calculator.c**: LazyCalculator - simple integer arithmetic (✅ **FULLY WORKING!**)
- Left-to-right evaluation with error handling

#### Libc Subset (Just Enough)
- **string.c**: Essential string functions
- **int.c**: Integer utilities for the calculator

## 🐛 Known Issues

### Minor Quirks (DOS Had Them Too)
1. **Backspace**: Works but might have edge cases
2. **Memory**: Still no advanced memory manager (by design)
3. **No multitasking**: Single-tasking, just like early DOS

### Philosophical Choices (Not Bugs)
1. **No IDT**: Interrupts handled simply
2. **No PMM/VMM**: Direct memory access
3. **No ACPI**: Simplicity over power management

## ✅ Current Status: What Works Now

### Fully Functional:
- ✅ **TTY Shell**: Interactive input with backspace support
- ✅ **Command Suite**: All commands work:
  - `help` - Shows available commands
  - `cfetch` - System information display
  - `info` - Detailed system info
  - `reboot` - Restarts the system
  - `shutdown` - Powers off (in emulator)
  - `clear` - Clears the screen
  - `echo` - Echoes text back
  - `calc` - 🧮 **LazyCalculator (FULLY WORKING!)**
- ✅ **I/O Systems**: VGA output and keyboard input
- ✅ **Basic Utilities**: String and math functions

### LazyCalculator (The Star of v0.0.3):
The calculator has been completely reworked into a reliable, simple tool:
- **Integer-only**: No floating point complexity
- **Left-to-right evaluation**: Simple and predictable
- **Safe operations**: Division by zero protection
- **Error handling**: Clear error messages, no crashes
- **Clean interface**: Easy to use, easy to understand

## 🤝 Contributing (Welcome!)

We welcome contributions that maintain our philosophy of simplicity:

### Contribution Guidelines
1. **Keep it simple**: If it adds complexity without clear benefit, reconsider
2. **Understandable code**: Comments and clear logic are appreciated
3. **DOS-like spirit**: Direct, efficient, and minimal
4. **Test it**: Make sure it doesn't break existing functionality

### What We're Looking For:
- Simple utilities in the DOS tradition
- Documentation improvements
- Bug fixes (especially edge cases)
- Performance improvements that don't add complexity

### What to Avoid:
- Over-engineering
- Unnecessary abstractions
- Breaking the simplicity philosophy

## ❓ FAQ (Frequently Asked Questions)

### Q: Is the calculator really fixed?
**A**: Yes! v0.0.3 features **LazyCalculator** - a completely new, reliable, integer-only calculator that actually works!

### Q: Why integer-only and left-to-right evaluation?
**A**: Simplicity and predictability. Like early DOS calculators, we keep it simple and understandable. No complex precedence rules to remember.

### Q: What if I divide by zero?
**A**: LazyCalculator handles it gracefully with a clear error message. No crashes, no reboots needed.

### Q: Can I run this on real hardware?
**A**: Yes! LazyDOS should work on any x86 system that supports legacy boot. QEMU is recommended for development.

### Q: Is there a filesystem?
**A**: Not yet. Like very early DOS versions, we're focusing on core functionality first. LazyCalculator was our priority for v0.0.3.

### Q: Why no operator precedence in the calculator?
**A**: Left-to-right evaluation is simpler to implement and understand. It's a deliberate design choice for simplicity.

### Q: What's next for LazyDOS?
**A**: We're considering:
- Simple filesystem (A very simplified FAT32, might even call it LazyFS)
- Basic text editor
- More utilities in the DOS tradition
- Perhaps a simple game (if it fits the minimalist philosophy)

## 🙏 Acknowledgments

- **MS-DOS**: For the inspiration of simplicity
- **Tim Paterson**: For QDOS, the simple foundation
- **IBM PC**: For a straightforward hardware platform
- **Multiboot Specification**: For bootloading simplicity
- **QEMU Developers**: For making testing easy
- **The Retro Computing Community**: For keeping the spirit alive

## 🎯 Final Thoughts

LazyDOS v0.0.3 represents a significant milestone: we now have a useful application that embodies our philosophy. LazyCalculator isn't just a fixed calculator - it's a statement about what we believe in:

1. **Simplicity**: Integer-only, left-to-right evaluation
2. **Reliability**: Proper error handling, no crashes
3. **Usefulness**: Does the job without unnecessary complexity
4. **Understandability**: You can read and understand the entire implementation

In a world of bloated software with countless features you'll never use, LazyCalculator is a breath of fresh air. It does one thing, and does it well.

**Try it. Type `calc`. Experience the joy of software that just works, simply.**

Happy calculating! (And remember: 10 + 5 * 2 = 30, because we evaluate left-to-right!)

---
*"Simplicity is the ultimate sophistication." - Leonardo da Vinci*
*"640K ought to be enough for anybody." - Bill Gates (allegedly)*
*"Make it work, make it right, make it simple." - The LazyDOS Philosophy*

**LazyDOS v0.0.3 - Now with LazyCalculator: Simple, Reliable, and Actually Works!** 🧮