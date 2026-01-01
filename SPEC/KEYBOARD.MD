
## SPEC/KEYBOARD.MD

# LazyDOS Keyboard - Simple Key Reading

## The Philosophy
Like early PC keyboards, we keep it simple: read scancodes, translate to ASCII, that's it. No layers, no configurations, just typing.

## How It Works (Simply)
1. **Check if key available** (port 0x64)
2. **Read scancode** (port 0x60)
3. **Translate to ASCII** (using simple tables)
4. **Return character**

## Our Simple API
```c
char lazy_getchar(void);    // Wait for a key, return ASCII
char lazy_trygetchar(void); // Check for key, return 0 if none
int lazy_key_available(void); // Is a key waiting?
```

## Key Features (The Useful Ones)
- **Letters, numbers, symbols**: All the basics
- **Shift modifier**: For uppercase and symbols
- **Backspace**: For fixing typos
- **Enter**: For submitting commands
- **Ctrl+R / Ctrl+X**: For QBASIC control

## What We Don't Handle (For Simplicity)
- Function keys (F1-F12)
- Numpad (except as numbers)
- International layouts
- Key repeat (OS handles that)
- LED lights (Caps Lock, etc.)

## The Translation Tables
We have two simple tables:
1. **Normal**: What keys produce normally
2. **Shifted**: What keys produce with Shift

They're just arrays of 128 characters. Simple lookup.

## Special Keys We Care About
- `\n` = Enter (submits command)
- `\b` = Backspace (deletes character)
- `0x12` = Ctrl+R (run QBASIC)
- `0x18` = Ctrl+X (exit QBASIC)

## Implementation Simplicity
```c
char lazy_getchar(void) {
    char c;
    while (!(c = lazy_trygetchar())) ; // Wait for key
    return c;
}
```

## Why Polling? (Not Interrupts)
Interrupts are complex. Polling is simple. We're simple.

## Testing
If you can type and see letters, it works.

## Limitations (By Design)
1. US QWERTY layout only
2. No key-up events (just key-down)
3. No modifier tracking beyond Shift
4. Simple error handling (ignore bad scancodes)

**LazyDOS Principle**: Input should be straightforward. Our keyboard driver is exactly that.
