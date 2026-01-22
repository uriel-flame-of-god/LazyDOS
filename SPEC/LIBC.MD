## SPEC/LIBC.MD

# LazyDOS C Library - Minimal and Understandable

## The Philosophy
We implement only what we need, in the simplest way possible. No bloat, no unnecessary features, just basic operations done clearly.

## What We Provide
Just enough to write a kernel and simple applications:
- **String functions**: `strlen`, `strcpy`, `strcmp`, `strncmp`
- **Memory functions**: `memset`, `memcpy`, `memmove`
- **Integer math**: Division and modulus
- **No standard library**: We write our own

## String Functions (Simple Implementations)

### `strlen`
```c
size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len]) len++;
    return len;
}
```
Count characters until null. Simple.

### `strcpy`
```c
char* strcpy(char* dst, const char* src) {
    char *d = dst;
    while ((*d++ = *src++));
    return dst;
}
```
Copy characters until null. Simple.

### `strcmp`
```c
int strcmp(const char* a, const char* b) {
    while (*a && *a == *b) { ++a; ++b; }
    return *(unsigned char*)a - *(unsigned char*)b;
}
```
Compare character by character. Simple.

## Memory Functions (Also Simple)

### `memset`
```c
void* memset(void* ptr, int value, size_t num) {
    unsigned char* p = ptr;
    while (num--) *p++ = (unsigned char)value;
    return ptr;
}
```
Set bytes to value. Simple.

### `memcpy` and `memmove`
One copies forward, one checks for overlap. Both are simple loops.

## Integer Division (The Interesting Part)
We implement division without hardware support (for educational value):

### How It Works (Simply)
```c
uint32 uint32_div(uint32 num, uint32 den) {
    if (den == 0) return 0xFFFFFFFF;  // Handle divide by zero
    uint32 quo = 0;
    for (int bit = 31; bit >= 0; --bit) {
        if (num >= (den << bit)) {
            num -= den << bit;
            quo |= 1 << bit;
        }
    }
    return quo;
}
```
Long division, bit by bit. Understandable.

## What We Don't Have (And Why)
- **No floating point**: Too complex for now
- **No dynamic allocation**: No `malloc`/`free`
- **No file I/O**: No filesystem yet
- **No formatted I/O**: No `printf`/`scanf`
- **No time functions**: No clock yet
- **No math library**: No `sin`, `cos`, etc.

## Design Choices
1. **Simple algorithms**: Easy to understand
2. **No optimization**: Clarity over speed
3. **No error handling**: Caller checks parameters
4. **No portability**: x86 only, 32-bit only

## Why Write Our Own?
1. **Control**: We know exactly what it does
2. **Simplicity**: No complex library code
3. **Size**: Only what we need
4. **Educational**: You can learn from it

## Testing
We test each function with:
- Normal cases
- Edge cases (empty strings, zero length)
- Error cases (divide by zero)

## The `int.h` and `float.h` Modules
We provide basic integer and (simple) floating point operations. The floating point is very basic - good enough for simple math.

## Future (Maybe)
We might add:
- Simple `atoi`/`itoa`
- Basic `printf` subset
- Maybe simple memory allocation

But only if needed and only if simple.

**LazyDOS Principle**: Library functions should be transparent. Ours are.