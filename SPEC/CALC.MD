## SPEC/CALC.MD

# LazyDOS Calculator - Simple Math

## The Philosophy
Like early calculator programs, we do simple integer math. No scientific functions, no parentheses, no precedence. Just type numbers and operators, get results.

## What It Does
1. **Reads expressions**: Like `2+3*4`
2. **Evaluates left-to-right**: Simple and predictable
3. **Shows results**: Integer answers
4. **Handles errors**: Division by zero, bad syntax

## The Rules (Simple)
- **Integers only**: No decimals
- **Left-to-right**: `2+3*4` = `(2+3)*4` = `20`
- **Four operators**: `+ - * /`
- **Negative numbers**: Start with `-`
- **Spaces optional**: `2+3` or `2 + 3`

## Why Left-to-Right?
Because it's simple. You don't need to remember PEMDAS. What you type is what you get.

## Example Session
```
=== LazyDOS Calculator (int-only) ===
Left-to-right evaluation. Type 'exit' to quit.

calc> 2+3
5
calc> 10-4*2
12
calc> 20/3
6
calc> 5/0
ERROR: divide by zero
calc> exit
Calculator exited
```

## Error Messages (Helpful)
- `ERROR: divide by zero` - Can't divide by zero
- `ERROR: invalid expression` - Something's wrong with what you typed

## Implementation (Simple)
1. **Parse numbers**: Read digits, handle minus
2. **Apply operators**: In order they appear
3. **Check division**: Watch for zero
4. **Convert to string**: Show result

## The Evaluation Function
```c
static bool eval_expr(const char *s, int32 *result) {
    read first number;
    while (more input) {
        read operator;
        read next number;
        apply operator;
    }
    return result;
}
```

See? Simple.

## What We Don't Do (For Simplicity)
- Floating point
- Parentheses
- Operator precedence
- Variables
- Memory functions
- Hexadecimal
- Bitwise operations
- Scientific notation

## Why Integer-Only?
1. **Simplicity**: No floating point complexity
2. **Speed**: Integer math is fast
3. **Predictability**: No rounding errors
4. **DOS Tradition**: Early calculators were integer-only

## Testing
We test:
- Basic operations (`+ - * /`)
- Negative numbers
- Division by zero
- Invalid expressions
- Large numbers

## Limitations (By Design)
- 32-bit integers only (-2 billion to +2 billion)
- Division truncates (no remainder)
- No error recovery (just error message)

## Future (Maybe)
We might add:
- Remainder operator (`%`)
- Simple variables
- Maybe decimal support

But only if it stays simple.

**LazyDOS Principle**: A calculator should be predictable. Ours is.
