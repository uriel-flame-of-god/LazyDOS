# LazyDOS v0.0.5 - New Year 2026 Release! 🎉

![New Year Release](https://img.shields.io/badge/Release-Jan_1,_2026-green)
![QBASIC Added](https://img.shields.io/badge/QBASIC-✅_Working-blue)
![Version: v0.0.5](https://img.shields.io/badge/Version-v0.0.5-orange)

## 🎉 What's New in v0.0.5 - The New Year Edition!

Welcome to LazyDOS v0.0.5! Starting 2026 with a minimalist QBASIC interpreter that brings back the nostalgia of early programming while maintaining our core philosophy of simplicity.

### ✨ Major Feature: QBASIC Minimal Interpreter

We've added a minimal QBASIC interpreter that lets you write and run simple BASIC programs right in LazyDOS. It's intentionally minimal, educational, and true to the spirit of early BASIC implementations.

## 🧰 QBASIC Features

### **Supported Commands:**
- **`LET`** - Variable assignment (both integers and strings)
- **`PRINT`** - Output text and variable values
- **`INPUT`** - Get user input for variables
- **`IF/THEN/ELSE`** - Basic conditional logic
- **Expressions** - Simple variable evaluation in PRINT statements

### **How to Use:**

#### **Interactive Mode:**
```basic
LazyDOS> qbasic
=== QBASIC EDITOR (Ctrl+R: Run, Ctrl+X: Exit) ===

10 LET x = 5
20 LET y = 10
30 PRINT x + y
40 IF x > 3 THEN PRINT "x is greater than 3"
50 INPUT z
60 PRINT "You entered: ", z
70 LET message = "Hello LazyDOS!"
80 PRINT message
```

#### **Run Your Program:**
Press **Ctrl+R** to execute your BASIC program and see the output!

#### **Exit the Editor:**
Press **Ctrl+X** to return to the LazyDOS shell.

### **Variable Support:**
- **Integer variables**: Automatically created with numeric assignments
- **String variables**: Created when assigned string values (surrounded by quotes)
- **Automatic type detection**: Variables are created with appropriate types based on assignment

### **Conditional Logic Examples:**
```basic
IF x > 10 THEN PRINT "Large"
IF y < 5 THEN PRINT "Small" ELSE PRINT "Not small"
IF score >= 90 THEN PRINT "A grade"
```

### **Keyboard Shortcuts:**
- **Ctrl+R**: Run the current program
- **Ctrl+X**: Exit QBASIC and return to LazyDOS
- **Backspace**: Delete previous character
- **Enter**: New line

## 🎨 Design Philosophy

Our QBASIC implementation follows the LazyDOS minimalist approach:

1. **Educational Focus**: Learn programming concepts without complexity
2. **No Line Numbers Required**: Free-form editing (though line numbers work if you use them)
3. **Immediate Feedback**: See results immediately when you run
4. **Understandable Implementation**: The entire interpreter is under 500 lines of clean C code

### **What Makes It "Minimal":**
- No GOSUB/RETURN (yet)
- No FOR/NEXT loops (yet)
- No arrays (yet)
- No functions (yet)
- No file I/O (yet)

### **What Makes It "Useful":**
- Perfect for learning programming basics
- Great for simple calculations and text output
- Teaches fundamental programming concepts
- Nostalgic fun for those who remember early BASIC

## 📁 Project Structure Update

```
src/
├── bootloader/
│   └── boot.asm
└── kernel/
    ├── apps/
    │   ├── calculator.c      # ✅ LazyCalculator - Fully Working!
    │   ├── calculator.h
    │   ├── qbasic.c         # ✅ NEW: QBASIC Minimal Interpreter
    │   └── qbasic.h         # ✅ NEW: QBASIC Header
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

## 🚀 Quick Start with QBASIC

```bash
# Build and run LazyDOS v0.0.5
make clean && make && make run

# Once booted, launch QBASIC
LazyDOS> qbasic

# Try this simple program:
LET a = 10
LET b = 20
PRINT a + b
INPUT name
PRINT "Hello, ", name
IF a > 5 THEN PRINT "a is big!"
```

## 🔍 Technical Implementation Details

### **Core Architecture:**
- **Editor Buffer**: 5000 characters for program storage
- **Variable Storage**: Up to 50 variables (mix of integers and strings)
- **Line-by-Line Execution**: Simple interpreter loop
- **Condition Parser**: Handles `>`, `<`, `=`, `>=`, `<=`, `<>` comparisons

### **Memory Management:**
- **Program Storage**: Static buffer (no dynamic allocation)
- **Variable Table**: Fixed-size array
- **String Values**: Fixed 128-character limit
- **No Memory Leaks**: Simple, predictable memory usage

### **Input/Output:**
- **Keyboard Input**: Full support with backspace
- **VGA Output**: Color-coded editor and output
- **Error Handling**: Basic but functional
- **No File System**: Programs exist only in memory (for now)

## 🎯 Example Programs

### **Simple Calculator:**
```basic
PRINT "Simple Calculator"
INPUT "Enter first number: ", a
INPUT "Enter second number: ", b
PRINT "Sum: ", a + b
PRINT "Difference: ", a - b
PRINT "Product: ", a * b
IF b <> 0 THEN PRINT "Quotient: ", a / b
```

### **Number Guessing Game:**
```basic
LET secret = 42
PRINT "Guess the number (1-100)"
INPUT guess
IF guess = secret THEN PRINT "Correct!"
IF guess > secret THEN PRINT "Too high!"
IF guess < secret THEN PRINT "Too low!"
```

### **Interactive Greeting:**
```basic
PRINT "What's your name?"
INPUT name
PRINT "Hello, ", name, "!"
PRINT "How old are you?"
INPUT age
IF age >= 18 THEN PRINT "You're an adult!"
IF age < 18 THEN PRINT "You're a minor!"
```

## 📈 What's Next for QBASIC?

### **Planned Features (Future Releases):**
1. **Loops**: `FOR/NEXT` and `WHILE/WEND`
2. **Arrays**: Simple numeric arrays
3. **Functions**: `GOSUB/RETURN` for subroutines
4. **More Math Functions**: `RND()`, `ABS()`, etc.
5. **String Functions**: `LEN()`, `LEFT$()`, `RIGHT$()`
6. **Program Storage**: Save/load simple programs
7. **Line Number Support**: Full line number parsing
8. **Error Messages**: Better diagnostics

### **Long-term Vision:**
- Create a complete but minimalist BASIC interpreter
- Educational tool for learning programming
- Run classic simple BASIC programs
- Maintain the LazyDOS philosophy of simplicity

## 🐛 Known Limitations (v0.0.5)

### **Current Constraints:**
1. **No Line Number Parsing**: Programs must be sequential
2. **Simple Conditionals**: Only basic comparisons work
3. **No Error Recovery**: Errors may leave interpreter in odd state
4. **Limited String Support**: Basic string assignment only
5. **No Program Save**: Programs lost on exit

### **These Are Features (Not Bugs):**
1. **Minimalist by Design**: We intentionally start small
2. **Educational Focus**: Complexity added only when beneficial
3. **Understandable Code**: You can read and understand the entire interpreter
4. **DOS Tradition**: Early BASIC versions were similarly limited

## 🎊 Celebrating 2026!

This release marks our commitment to continuous improvement. While QBASIC v0.0.5 is minimal, it's a solid foundation that:

1. **Actually Works**: You can write and run programs
2. **Teaches Fundamentals**: Variables, I/O, conditionals
3. **Maintains Simplicity**: No unnecessary complexity
4. **Sets Direction**: Clear path for future enhancements

## 🙏 New Year's Resolution

For 2026, LazyDOS resolves to:
- **Stay Simple**: Resist the temptation to over-engineer
- **Stay Educational**: Help people learn how computers really work
- **Stay Fun**: Keep the joy of computing alive
- **Stay Open**: Continue sharing our journey transparently

## 🚀 Try It Today!

```bash
# Get the latest version
git clone https://github.com/uriel-flame-of-god/LazyDOS.git
cd lazydos

# Build v0.0.5
make clean
make

# Run and explore QBASIC
make run

# In LazyDOS:
LazyDOS> qbasic
```

## 📚 Learning Resources

Want to learn more about BASIC programming? Check out:
- [The BASIC Programming Language (Wikipedia)](https://en.wikipedia.org/wiki/BASIC)
- [QBASIC Programming for Kids](https://www.qbasic.net)
- [Classic BASIC Games](https://github.com/coding-horror/basic-computer-games)

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

### Wanted Features:
1. `FOR/NEXT` loop implementation
2. `GOSUB/RETURN` subroutine support
3. Better error messages
4. Example programs

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

LazyDOS v0.0.5 represents our ongoing commitment to creating understandable, minimalist software. The QBASIC interpreter may be simple, but it:

1. **Works reliably**: No crashes during normal use
2. **Teaches effectively**: Perfect for programming beginners
3. **Honors tradition**: Follows in the footsteps of early microcomputer BASIC
4. **Points forward**: A foundation we can build upon

**Happy New Year 2026 from the LazyDOS team! May your code be simple, your bugs few, and your understanding deep.**

---

*"The computer revolution hasn't started yet. Don't be misled by the enormous flow of money into bad defacto standards." - Alan Kay*
*"Simplicity is prerequisite for reliability." - Edsger W. Dijkstra*
*"Happy New Year! Let's make 2026 the year of simple, understandable software." - The LazyDOS Team*

**LazyDOS v0.0.5 - Now with Minimalist QBASIC: Start 2026 with Simple Programming!** 🎉