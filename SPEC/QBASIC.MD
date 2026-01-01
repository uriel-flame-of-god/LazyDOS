## SPEC/QBASIC.MD

```markdown
# LazyDOS QBASIC - Simple Programming

## The Philosophy
Like early microcomputer BASIC, we provide a simple, understandable programming environment. No complex features, no OOP, no libraries. Just the basics.

## What It Is
A minimal BASIC interpreter with:
- **PRINT**: Show text and numbers
- **INPUT**: Get user input
- **IF/THEN/ELSE**: Make decisions
- **Variables**: Store numbers and strings
- **Editor**: Type and run programs

## What It Isn't
- Not QuickBASIC
- Not Visual Basic
- Not a full language
- Not fast
- Not feature-complete

## Why QBASIC? (For 2026!)
1. **Educational**: Perfect for beginners
2. **Simple**: Easy to understand syntax
3. **Nostalgic**: Reminds us of early computing
4. **Minimalist**: Fits our philosophy

## Features (The Simple Ones)

### PRINT Statement
```basic
PRINT "Hello, 2026!"
PRINT "2+2="; 2+2
```
Shows text and values. Simple.

### INPUT Statement
```basic
INPUT "Your name: ", name$
INPUT age
```
Gets input from user. Simple.

### IF/THEN/ELSE
```basic
IF age > 18 THEN PRINT "Adult" ELSE PRINT "Child"
```
Makes decisions. Simple.

### Variables
- **Numbers**: `age = 25`
- **Strings**: `name$ = "Alice"`
- **Simple types**: That's it

## Editor Features
- **Type code**: Just type lines
- **Ctrl+R**: Run program
- **Ctrl+X**: Exit to shell
- **No saving**: Programs don't persist (yet)

## Example Program
```basic
PRINT "Happy New Year 2026!"
INPUT "What's your name? ", name$
PRINT "Hello, "; name$; "!"
INPUT "How old are you? ", age
IF age >= 21 THEN PRINT "You can celebrate!" ELSE PRINT "Maybe next year!"
```

## Implementation (Understandable)
We don't have a complex parser. We just:
1. **Split lines**: By newlines
2. **Parse tokens**: Simple scanning
3. **Execute**: Direct interpretation
4. **No compilation**: No bytecode, no JIT

## Why This Simplicity Works
1. **No complex grammar**: Easy to parse
2. **Immediate feedback**: Type and run
3. **Understandable errors**: Simple messages
4. **Educational value**: You can see how it works

## Limitations (By Design)
1. **No loops yet**: No `FOR/NEXT`, `WHILE/WEND`
2. **No subroutines**: No `GOSUB/RETURN`
3. **No arrays**: Just simple variables
4. **No files**: No `OPEN`, `CLOSE`
5. **No graphics**: Text only
6. **No sound**: Silent operation

## Variables System (Simple)
We store variables in arrays:
```c
variable vars[MAX_VARS];  // Fixed array, simple
```
No hash tables, no dynamic allocation. Just search linearly.

## Error Messages (Helpful)
We try to give clear errors when:
- Variable not found
- Type mismatch
- Syntax error
- Division by zero

## Testing
We test with:
- Simple PRINT statements
- INPUT with different types
- IF conditions
- Variable assignments

## The Editor (Minimal)
- **No line numbers**: Just type
- **No syntax highlighting**: Monochrome
- **No undo**: Be careful
- **Simple interface**: Just type and run

## Why Include QBASIC in 2026?
1. **Teaching tool**: Great for learning programming concepts
2. **Historical value**: Connects to computing history
3. **Simplicity example**: Shows how simple interpreters work
4. **Fun factor**: Programming should be fun

## Future Features (If Simple)
1. **FOR/NEXT loops**: Basic iteration
2. **GOSUB/RETURN**: Simple subroutines
3. **Arrays**: Fixed-size arrays
4. **DATA/READ**: Simple data storage
5. **RND function**: Random numbers
6. **Simple functions**: DEF FN

## Happy New Year 2026 Feature!
This QBASIC interpreter is our New Year 2026 release. It's not much, but it's:
- **Understandable**: You can read the code
- **Working**: It runs programs
- **Educational**: Teaches BASIC concepts
- **A starting point**: We can build from here

**LazyDOS Principle**: Programming should be accessible. Our QBASIC makes it accessible.
