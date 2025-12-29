# LazyDOS - The "It Works Sometimes" Operating System

![LazyDOS Logo](https://img.shields.io/badge/LazyDOS-v0.0.1-lazy?style=for-the-badge)
![License: GPL v3.0](https://img.shields.io/badge/License-GPLv3-blue.svg)
![Philosophy: Simplicity](https://img.shields.io/badge/Philosophy-Simplicity-blue.svg)
![Inspiration: MS-DOS](https://img.shields.io/badge/Inspiration-MS--DOS-00ff00.svg)

> "Why reinvent the wheel when you can just push the car?"
> 
> *Inspired by the beautiful simplicity of MS-DOS*

## 📋 Table of Contents
- [The LazyDOS Philosophy](#the-lazydos-philosophy)
- [Name Origin](#-name-origin-not-laziness-but-simplicity)
- [System Requirements](#system-requirements)
- [Quick Start](#quick-start)
- [Building from Source](#building-from-source)
- [Running the System](#running-the-system)
- [Architecture Overview](#architecture-overview)
- [Known Issues (We're Too Lazy to Fix)](#known-issues-were-too-lazy-to-fix)
- [Recent Fixes & Current Status](#recent-fixes--current-status)
- [Contributing (If You Must)](#contributing-if-you-must)
- [FAQ (Frequently Avoided Questions)](#faq-frequently-avoided-questions)
- [License](#license)

## 🦥 The LazyDOS Philosophy

LazyDOS is built on one simple principle: **Developers are lazy, so let them be**. Unlike other operating systems that force you to implement everything "properly," LazyDOS embraces the chaos. Here's what that means:

- **No IDT?** Perfect! Implement your own interrupt handling!
- **GDT kinda works?** Good enough!
- **Input handling broken?** Someone else will fix it eventually!
- **Memory management?** Just allocate somewhere and hope for the best!

We believe in **"Lazy Open Source"** - we provide the bare minimum, and the community can either fix it or live with it. It's not a bug; it's a feature!

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
2. **It gets out of your way** - like a lazy butler who knows when not to bother you
3. **It embraces simplicity** - the "laziness" of elegant design
4. **It's approachable** - you don't need a PhD to understand it

Think of it as **MS-DOS's minimalist cousin who took a nap and woke up in the 21st century**. We're not lazy; we're *efficiently minimalist*.

## 🖥️ System Requirements

### Minimal (It Might Boot)
- **CPU**: i486 or better (or worse, we don't judge)
- **RAM**: 640 KB conventional memory (we're nostalgic like that)
- **Bootloader**: Anything that supports Multiboot 1.0 (or pretends to)
- **Patience**: Lots of it

### Recommended (It Might Actually Work)
- **CPU**: Anything made after 1995
- **RAM**: At least 1 MB (so we have somewhere to put our stack)
- **Emulator**: QEMU or Bochs (because testing on real hardware is too much work)
- **Expectations**: Extremely low
- **Nostalgia**: For the days when software fit on a single floppy

## 🚀 Quick Start

### For the Impatient (Our Target Audience)

```bash
# Clone the repository (if you can be bothered)
git clone https://github.com/uriel-flame-of-god/LazyDOS.git
cd lazydos

# Try to build it (no guarantees)
make

# Run it and pray
make run
```

## 🔨 Building from Source

### Step 1: Get the Tools (If You Must)

You'll need:
- **NASM** (because assembly is too mainstream for GCC)
- **GCC Cross-Compiler** (i686-elf target, because we're special)
- **Make** (or just run commands manually, we don't care)

### Step 2: The Build Process

```bash
# Option A: Use the Makefile (recommended for lazy people)
make all

# Option B: Do it manually (for masochists)
nasm -f elf32 src/bootloader/boot.asm -o boot.o
i686-elf-gcc -c src/kernel/core/kernel.c -o kernel.o -ffreestanding -O2
# ... (compile 20 more files)
# ... (link them together with magic incantations)
```

### Step 3: Create the ISO

```bash
# Our sophisticated ISO creation process
make iso

# This creates: lazydos.iso
# File size: Approximately "good enough" bytes
```

## 🏃 Running the System

### In QEMU (The Easy Way)

```bash
# Basic run
make run

# With debug output (for when it doesn't work)
make debug

# With more RAM (because why not)
qemu-system-i386 -cdrom lazydos.iso -m 128M
```

### In Bochs (For the Purists)

```bash
# Create a bochsrc.txt with something like:
# memory: guest=128, host=128
# romimage: file=$BXSHARE/BIOS-bochs-latest
# vgaromimage: file=$BXSHARE/VGABIOS-lgpl-latest
# floppya: 1_44=lazydos.iso, status=inserted
# boot: cdrom
# Then run:
bochs -q
```

### On Real Hardware (Not Recommended)

1. Burn ISO to CD/DVD/USB (using your favorite tool)
2. Boot from it
3. Watch it fail spectacularly
4. Go back to using QEMU

## 🏗️ Architecture Overview

### Boot Process (The Part That Actually Works)
1. **boot.asm**: Multiboot header + stack setup (inspired by the simple boot sectors of old)
2. **GDT**: Global Descriptor Table (kinda works, just like in the good old days)
3. **Kernel Entry**: Jumps to C code (usually)

### Kernel Components

#### Core (Mostly Functional)
- **kernel.c**: Entry point, calls TTY (the real MVP, like COMMAND.COM but simpler)
- **gdt.c**: Sets up segmentation (or tries to, with MS-DOS simplicity)
- **tty.c**: Integrated shell (where you'll spend 99% of your time, just like in DOS)

#### I/O Drivers (Works Sometimes)
- **vga.c**: Text mode driver (80×25, because that's what MS-DOS used)
- **keyboard.c**: PS/2 keyboard driver (set 1, because that's what worked in DOS)
- **port.c**: I/O port helpers (actually works! Direct hardware access, just like DOS)

#### Applications (If You Can Call Them That)
- **calculator.c**: Stack-based calculator (like QBASIC but with fewer features) - *Currently broken, see below*
- **calculator.h**: Header file (contains declarations, probably)

#### Libc Subset (The Bare Minimum)
- **string.c**: strlen, memcpy, memset (but not all work correctly, reminiscent of early C libraries)
- **string.h**: Header with duplicate declarations (we were efficient, not lazy)

### Memory Map (Approximate)
```
0x00000000 - 0x0009FFFF: Conventional memory (just like DOS wanted)
0x000A0000 - 0x000BFFFF: Video memory (VGA text buffer at 0xB8000, DOS style)
0x00100000 - 0x00EFFFFF: Extended memory (if we ever use it, unlike DOS)
0x01000000+:           : Somewhere over there (future expansion, maybe)
```

## 🐛 Known Issues (We're Too Lazy to Fix)

### Critical Issues (Might Actually Matter)
1. **Calculator**: The calculator app is currently broken - it compiles but crashes or gives incorrect results (very authentic to early DOS math libraries)
2. **Input Handling**: The TTY doesn't always echo characters correctly (just like early DOS versions)
3. **Backspace**: Sometimes works, sometimes doesn't (50/50 chance, authentic retro experience)
4. **Memory**: No proper memory manager (just allocate and hope, very DOS-like)

### Minor Issues (Probably Fine)
1. **No IDT**: Interrupts? What interrupts? (DOS didn't need fancy interrupt handling)
2. **No PMM**: Physical Memory Manager? Just use the first free address! (That's how DOS did it)
3. **No VMM**: Virtual Memory? We barely have physical memory! (True to the DOS spirit)
4. **No ACPI**: Advanced Configuration and Power Interface? Too advanced! (DOS never heard of it)

### Philosophical Issues (Features, Not Bugs)
1. **Simplicity First**: We don't add features unless absolutely necessary (like MS-DOS)
2. **Minimal Documentation**: This README is already too long (DOS manuals were concise too)
3. **"Works on My Machine" Mentality**: If it boots once, it's production-ready (a DOS tradition)
4. **Direct Hardware Access**: If you can't poke the hardware directly, what's the point? (The DOS way)

## ✅ Recent Fixes & Current Status

### What's Working Now:
- ✅ **TTY Shell**: The integrated shell now works properly! You can type commands and see them echoed correctly
- ✅ **Command Input**: Basic command input and backspace handling is functional
- ✅ **Help System**: All built-in commands except calculator are accessible
- ✅ **System Commands**: `help`, `cfetch`, `info`, `reboot`, `shutdown`, `clear`, `echo` all work
- ✅ **Basic I/O**: VGA output and keyboard input are stable

### What's Still Broken:
- ❌ **Calculator App**: The calculator application (`calc` command) is currently non-functional. It may:
  - Crash the system
  - Give incorrect results
  - Refuse to start entirely
  - Or any combination of the above
  
  *This is authentic to the early DOS experience where bundled applications were often buggy and incomplete.*

- ⚠️ **Edge Cases**: Various edge cases in input handling may still exist

### Fixing the Calculator (Community Challenge):
The calculator is broken in the true LazyDOS spirit - it's there, it compiles, but it doesn't work properly. This is your chance to:

1. **Fix it yourself** (if you're feeling ambitious)
2. **Ignore it** (if you're feeling lazy)
3. **Complain about it** (if you're feeling traditional)

The source is in `src/kernel/apps/calculator.c` - go wild! Or don't. We're not your boss.

## 🤝 Contributing (If You Must)

We welcome contributions, as long as they don't require too much work:

### Contribution Guidelines
1. **Fork it** (if you can be bothered)
2. **Make minimal changes** (the less code, the better - DOS was small for a reason)
3. **Don't break the "simple" philosophy** (no over-engineering! Keep it DOS-like)
4. **Submit a PR** (or just email us the patch, whatever)

### What We Accept
- Bug fixes (if they're trivial and don't add complexity)
- Calculator fixes (seriously, someone fix this thing)
- New features (if they require < 50 lines of code and feel DOS-like)
- Documentation improvements (if they're funny and concise)
- Spelling fixes (maybe)

### What We Don't Accept
- Complete rewrites (too much work, and DOS never rewrote itself)
- "Best practices" (we have our own practices, just like DOS did)
- Unit tests (testing is for people who care; DOS trusted its developers)
- Performance optimizations (if it's slow, get a faster computer - the DOS solution)

## ❓ FAQ (Frequently Avoided Questions)

### Q: Why doesn't the calculator work?
**A**: It's authentic! Early versions of DOS had buggy utilities too. Consider it a feature - the "broken calculator" is our tribute to the days when software came with personality (and bugs). Feel free to fix it yourself!

### Q: Why doesn't the keyboard input work properly?
**A**: It's authentic! Early versions of DOS had input quirks too. We're honoring tradition. (Actually, we mostly fixed this one, but there might still be edge cases.)

### Q: Is there memory protection?
**A**: Memory protection is for operating systems that don't trust their applications. MS-DOS trusted everyone, and so do we.

### Q: Can I run GUI applications?
**A**: We're focusing on text mode, just like DOS 1.0 through 6.22. If you want graphics, write your own driver (just like in the DOS days).

### Q: Is this production-ready?
**A**: It boots sometimes. MS-DOS 1.0 was less stable, and look how that turned out!

### Q: Why GPL v3.0?
**A**: Because we want you to share your fixes, but we're too busy being simple to enforce it.

### Q: How do I report bugs?
**A**: Don't. MS-DOS didn't have bug trackers either. You just learned to work around them.

### Q: Can I use this for my mission-critical system?
**A**: Only if your mission is to have a system with the elegant simplicity of early personal computing.

### Q: Is there networking support?
**A**: Networking requires packets. Packets require effort. MS-DOS didn't have built-in networking either until much later.

### Q: Why should I use LazyDOS instead of FreeDOS?
**A**: FreeDOS tries to be compatible with MS-DOS. LazyDOS tries to capture the *spirit* of MS-DOS - the simplicity, the directness, the "figure it out yourself" attitude.

## 🙏 Acknowledgments

- **MS-DOS**: For showing us that simple can be beautiful
- **Tim Paterson**: For writing QDOS (which became MS-DOS) in just 4 months
- **IBM PC**: For giving us a simple hardware platform to target
- **Multiboot Specification**: For making bootloading someone else's problem
- **QEMU Developers**: For making it easy to test without real hardware
- **The 80×25 Text Mode**: For being simple enough that even we could implement it
- **Caffeine**: The real MVP
- **Procrastination**: Our chief architectural advisor

## 🎯 Final Thoughts

LazyDOS isn't trying to be the next Linux or Windows. It's trying to capture that beautiful moment in computing history when an operating system was simple enough that one person could understand it all, direct enough that you could talk to the hardware yourself, and practical enough that it powered the personal computing revolution.

**We're not lazy. We're minimalist.** We're not cutting corners. We're removing unnecessary complexity. We're not ignoring best practices. We're prioritizing understandability over abstraction.

In a world of million-line kernels and layers of abstraction, sometimes you just want to `MOV AX, 0x13` and see something happen. That's the spirit of LazyDOS.

**P.S. The calculator is broken. You might want to fix that. Or not. We're not judging.**

Happy hacking! (Or not, if you'd rather just appreciate the simplicity)

---
*"Simplicity is the ultimate sophistication." - Leonardo da Vinci*
*"640K ought to be enough for anybody." - Bill Gates (allegedly)*
*"The less we do, the more we achieve." - The LazyDOS Development Team* (and by the Team I mean Me, Myself and I)