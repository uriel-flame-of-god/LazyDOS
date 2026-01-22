# Word of God (WOG) Language Specification v0.1
## Simplified QBASIC-Style Interpreter for Bare Metal OS

**Status**: TEMPORARY / ACTIVE (Superseded by WOG 1.0 spec when OS advances)  
**Target Environment**: Single-threaded kernel, Ring 0, VGA text mode, cooperative execution  
**Lifespan**: Until OS supports user mode, paging, and memory protection

---

## ⚠️ CRITICAL NOTICE: TEMPORARY SPECIFICATION

**This is NOT the final WOG specification.** This is a **v0.1 kernel-safe subset** designed specifically for bare-metal execution without memory protection.

### Status Timeline
- **v0.1 (CURRENT)**: Kernel-safe, immediate-mode, no jumps/loops
- **v1.0 (FUTURE)**: Full specification with labels, jumps, loops, recursion (when OS supports user mode)

### What Changed from Full Spec
The complete **WOG 1.0 Language Specification** includes:
- ✅ Labels and jump tables (`GO YE UNTO`)
- ✅ Conditional and unconditional branching
- ✅ Persistent execution state and program counter
- ✅ Advanced control flow (loops, recursion)
- ✅ Multi-statement control blocks
- ✅ Formal type system enforcement

**None of these exist in v0.1.** They are intentionally forbidden because the kernel lacks:
- Memory protection (MMU/paging)
- User/kernel mode separation
- Process isolation
- Safe exception handling

### Migration Path
When your OS advances to support these features, implement the **full WOG 1.0 spec** (see Appendix D).

---

---

## 1. Design Philosophy (NON-NEGOTIABLE)

**WOG SHALL BE AN IMMEDIATE-MODE, LINEAR INTERPRETER.**

This means:
- No virtual machine
- No persistent execution state
- No program counter or jump tables
- No global AST
- No stored statements
- Every line: `parsed → executed → discarded`

### Rationale
A bare-metal kernel without memory protection cannot host a traditional VM. This spec guarantees deterministic behavior, prevents infinite loops, and ensures kernel stability.

---

## 2. Execution Model

### 2.1 Program Execution
```
FOR each line in editor buffer:
    IF line == "AND GOD SAID" THEN
        in_program = true
    ELSE IF line == "AND IT CAME TO PASS" THEN
        break
    ELSE IF in_program THEN
        execute_line(line)
    END IF
END FOR
```

**Key property**: There is no second pass. Each line executes exactly once.

### 2.2 Execution Guarantees
- Execution is **blocking** (no interrupts)
- Editor UI is **frozen** during execution
- Interpreter **never redraws** the editor
- Interpreter **output owns terminal exclusively**
- After execution, control returns to editor

---

## 3. State Model

### 3.1 Allowed Persistent State
The interpreter **MAY** persist:
- Variables (name → value map)
- String pool (literal strings)
- Error flags

The interpreter **MUST NOT** persist:
- Tokens
- Parsed statements
- Labels or jump tables
- Instruction pointers
- Call stacks
- Execution contexts

### 3.2 Variable Rules
- Variables created on **first assignment** (implicit declaration)
- Variable table is **bounded** (≤ 128 variables)
- Variables **persist** across lines within a single program run
- No variable shadowing
- No implicit temporaries

---

## 4. Program Structure

### 4.1 Program Boundaries

Every WOG program **MUST** have this structure:

```
AND GOD SAID
  <statements>
AND IT CAME TO PASS
```

- `AND GOD SAID` marks execution start (case-insensitive)
- `AND IT CAME TO PASS` marks execution end
- Statements between these boundaries are executed sequentially
- Lines outside these boundaries are ignored

---

## 5. Supported Statements (v0.1)

### 5.1 BEHOLD (Print Statement)

**Syntax**:
```
BEHOLD <expr>
BEHOLD "string literal"
BEHOLD variable_name
BEHOLD 42
```

**Behavior**:
- Evaluates expression immediately
- Outputs value to terminal
- Appends newline
- Expressions can be: string literals, integer literals, variable names

**Examples**:
```
BEHOLD "Hello, World"
BEHOLD x
BEHOLD 123
```

### 5.2 THOU SHALT (Assignment Statement)

**Syntax**:
```
THOU SHALT <variable> AND <value>
```

Where `<value>` can be:
- Integer literal
- Variable name
- `VERILY VERILY` (evaluates to 1)
- Arithmetic expression using `AND`

**Behavior**:
- Creates variable if it doesn't exist
- Assigns value immediately
- Variables default to 0 if uninitialized
- All values are 32-bit signed integers

**Examples**:
```
THOU SHALT x AND 42
THOU SHALT y AND VERILY VERILY
THOU SHALT z AND x AND 10
THOU SHALT result AND 0
```

### 5.3 LET THERE BE (Declaration – Optional)

**Syntax**:
```
LET THERE BE <variable> <type> : <lineage>
```

Where:
- `<type>` is 0-5 (optional, syntactic only)
- `<lineage>` is positive integer (optional)

**Behavior**:
- Declares variable with optional type annotation
- No runtime effect beyond allocation
- Type annotations are **syntactic only** (no type checking)
- Duplicate declarations with higher lineage are allowed
- Variable defaults to 0

**Examples**:
```
LET THERE BE x 2 : 1
LET THERE BE flag 1 : 1
LET THERE BE x 2 : 2
```

### 5.4 IF (Conditional Statement – v0.1)

**Syntax**:
```
IF <condition> THEN <statement>
```

**Allowed Conditions**:
```
<variable> = <integer>
<variable> < <integer>
<variable> > <integer>
<variable> <> <integer>
```

**Behavior**:
- Evaluates condition immediately
- If true, executes single statement
- If false, skips to next line
- **No ELSE clause** in v0.1
- **No nested IF statements**
- Condition evaluated at statement time only

**Examples**:
```
IF x = 5 THEN BEHOLD "Five"
IF counter > 10 THEN THOU SHALT flag AND 1
IF value <> 0 THEN BEHOLD "Non-zero"
```

### 5.5 WOE UNTO (Error Statement)

**Syntax**:
```
WOE UNTO "error message"
```

**Behavior**:
- Immediately halts execution
- Prints `WOE UNTO: ` followed by message
- Returns control to editor
- No recovery mechanism exists
- Remaining statements are skipped

**Examples**:
```
WOE UNTO "Fatal error: division by zero"
WOE UNTO "Invalid input"
```

---

## 6. Forbidden Constructs (v0.1 Only)

The following **MUST NOT** exist in v0.1:

❌ **Labels** – No label definition  
❌ **GO YE UNTO** – No jump statements  
❌ **ELSE clause** – Conditionals only  
❌ **Loops** – Iteration not supported  
❌ **Recursion** – No function calls  
❌ **Multi-line control flow** – Single-statement only  
❌ **Persistent token streams** – Line-by-line only  
❌ **Persistent program state** – No instruction pointer, execution stack, or VM state

**Rationale**: These constructs are **temporarily banned** until user mode exists. They would require:
- Stateful execution context (instruction pointer, call stack)
- Persistent label tables
- Jump target resolution
- Exception handling

These break kernel safety in Ring 0 without memory protection.

**Will be re-enabled in v1.0** once the OS supports:
- MMU and paging
- User/kernel mode separation
- Process isolation
- Safe exception handling

---

## 7. Expression Grammar

### 7.1 Arithmetic Expressions

**Syntax**:
```
<expression> := <primary> | <primary> AND <primary> | <primary> AND <primary> AND ...

<primary> := <integer>
           | "string"
           | <identifier>
           | VERILY VERILY
```

**Semantics**:
- `AND` operator performs **addition only**
- Left-to-right evaluation: `a AND b AND c` = `(a + b) + c`
- No operator precedence (operators are left-associative)
- String literals are not evaluated in arithmetic context

**Examples**:
```
AND 5 AND 3           // 0 + 5 + 3 = 8
x AND 10              // x + 10
VERILY VERILY         // 1
a AND b AND c AND d   // a + b + c + d
```

### 7.2 Value Types

All values are stored as **32-bit signed integers**:
- Integers: `-2^31` to `2^31 - 1`
- Strings: Stored separately, referenced by name only (no string arithmetic)
- Booleans: `0` (false) or `1` (true)
- Uninitialized variables: default to `0`

---

## 8. Lexical Rules (Simplified)

### 8.1 Tokenization
- **Lexer operates per-line** (no inter-line token state)
- **No token storage** beyond current line execution
- **Keywords are case-insensitive**: `BEHOLD`, `behold`, `Behold` are equivalent
- Identifiers **case-sensitive**: `x` and `X` are different variables

### 8.2 Identifiers
- Pattern: `[a-zA-Z_][a-zA-Z0-9_]*`
- Max length: 32 characters
- Examples: `x`, `counter`, `_private`, `my_var_123`

### 8.3 String Literals
- Delimited by double quotes: `"..."`
- Max length: 256 characters
- Escape sequences: `\"` for literal quote
- Examples: `"Hello"`, `"Error: x = 0"`

### 8.4 Numeric Literals
- Integers only (no floating point)
- Base 10: `0`, `42`, `-5`, `12345`
- Negative numbers via `AND` operator: `THOU SHALT x AND 0 AND -5`

### 8.5 Comments
- Single-line only, begin with `//`
- Rest of line is comment: `BEHOLD x  // Print x value`

---

## 9. Type System (Syntactic Only)

### 9.1 Type Annotations
Optional type numbers in declarations:
```
0 = void
1 = boolean
2 = integer
3 = string
4 = float
5 = double
```

### 9.2 Runtime Semantics
- **Type annotations are syntactic only**
- **No type checking at runtime**
- All values stored as 32-bit signed integers
- Declarations are hints to the programmer, not enforced

**Example**:
```
LET THERE BE x 2 : 1   // Type hint: integer
THOU SHALT x AND "hello"  // Allowed! No type check
```

---

## 10. Error Handling

### 10.1 Parse-Time Errors
- Missing `AND GOD SAID` → Report error, don't execute
- Missing `AND IT CAME TO PASS` → Report error, don't execute
- Invalid syntax → Report error, don't execute

### 10.2 Runtime Errors
On any runtime error:
1. Print `WOG ERROR: <message>`
2. Halt execution immediately
3. Return control to editor

**Interpreter MUST NOT**:
- Panic kernel
- Reboot system
- Corrupt terminal state
- Leak memory

### 10.3 Common Runtime Errors
- Undefined variable reference (returns 0)
- Division by zero (not applicable; no division in v0.1)
- Overflow (system-dependent; wraparound)

---

## 11. Memory Model

### 11.1 Variable Storage
- Global variable table: up to 128 variables
- Each variable: name + value (32-bit signed integer)
- Uninitialized: default to 0
- Lifetime: entire program run

### 11.2 String Pool
- Strings stored separately
- Each string literal gets unique storage
- Identical literals create separate entries
- Max total: 8 KB program size includes strings

### 11.3 Scope
- All variables are **global** (no block scope)
- No shadowing or redeclaration with same name
- Lineage numbers track re-declaration order

---

## 12. UI Rules (CRITICAL)

### 12.1 Editor Phase
- Editor controls terminal
- Editor redraws screen on each keystroke
- Interpreter is inactive

### 12.2 Execution Phase
- Editor is frozen (UI inactive)
- Interpreter controls terminal
- Interpreter clears screen **once** at start
- Interpreter owns output

### 12.3 Post-Execution Phase
- Interpreter waits for key press
- Control returns to editor
- Editor resumes

**Terminal ownership MUST NOT overlap.**

---

## 13. Resource Limits (HARD)

| Resource | Limit | Consequence |
|----------|-------|-------------|
| Variables | ≤ 128 | Graceful halt if exceeded |
| Line length | ≤ 256 chars | Truncate or error |
| Program size | ≤ 8 KB | Reject if too large |
| Execution steps | ≤ line count | Bounded by input |

**Violation of any limit** → graceful halt, return to editor

---

## 14. Implementation Checklist

Before calling WOG "compliant", verify:

- [ ] No global statement array (one-pass execution only)
- [ ] No program counter or jump table
- [ ] Each line executed once and discarded
- [ ] No loops or conditional jumps
- [ ] No recursion or multi-line control flow
- [ ] UI frozen during execution
- [ ] Terminal cleared once per run at start
- [ ] Cursor state reset on clear
- [ ] Variables persist within run
- [ ] All errors return gracefully to editor
- [ ] Max 128 variables enforced
- [ ] Max 256-char lines enforced
- [ ] Max 8 KB program enforced

---

## 15. Example Programs

### 15.1 Hello World
```
AND GOD SAID
BEHOLD "Hello, World"
AND IT CAME TO PASS
```

**Output**:
```
Hello, World
```

### 15.2 Variable Assignment
```
AND GOD SAID
THOU SHALT x AND 42
BEHOLD x
BEHOLD "Answer is 42"
AND IT CAME TO PASS
```

**Output**:
```
42
Answer is 42
```

### 15.3 Conditional
```
AND GOD SAID
THOU SHALT x AND 5
IF x = 5 THEN BEHOLD "Equal"
IF x > 5 THEN BEHOLD "Greater"
IF x < 5 THEN BEHOLD "Less"
AND IT CAME TO PASS
```

**Output**:
```
Equal
```

### 15.4 Arithmetic
```
AND GOD SAID
THOU SHALT a AND 10
THOU SHALT b AND 20
THOU SHALT sum AND a AND b
BEHOLD sum
AND IT CAME TO PASS
```

**Output**:
```
30
```

### 15.5 Error Handling
```
AND GOD SAID
THOU SHALT value AND 0
IF value = 0 THEN WOE UNTO "Value cannot be zero"
BEHOLD "Never reached"
AND IT CAME TO PASS
```

**Output**:
```
WOE UNTO: Value cannot be zero
```

---

## 16. Grammar (Formal EBNF)

```ebnf
Program      ::= "AND GOD SAID" Statements "AND IT CAME TO PASS"

Statements   ::= Statement*

Statement    ::= Declaration
              |  Assignment
              |  PrintStatement
              |  IfStatement
              |  ErrorStatement
              |  Comment
              |  EmptyLine

Declaration  ::= "LET THERE BE" Identifier [Type] [":" Lineage]

Type         ::= Digit  // 0-5

Lineage      ::= Number

Assignment   ::= "THOU SHALT" Identifier ["AND" Expression]

PrintStatement ::= "BEHOLD" Expression

IfStatement  ::= "IF" Condition "THEN" Statement

Condition    ::= Identifier CompOp Number

CompOp       ::= "=" | "<" | ">" | "<>"

ErrorStatement ::= "WOE UNTO" String

Expression   ::= Primary ("AND" Primary)*

Primary      ::= Number
              |  String
              |  Identifier
              |  "VERILY" "VERILY"

Identifier   ::= [a-zA-Z_][a-zA-Z0-9_]*

Number       ::= [0-9]+

String       ::= '"' [^"]* '"'

Comment      ::= "//" [^\n]*
```

---

## 17. Design Rationale

### Why No Jumps?
Jumps require persistent labels and instruction pointers. In a bare-metal environment, this breaks determinism and enables infinite loops that could hang the kernel.

### Why Immediate Mode?
Immediate-mode execution (parse-then-discard) guarantees:
- Constant memory usage (no AST building)
- No execution state to corrupt
- One-pass semantics (simple, predictable)

### Why Limited Operators?
Addition-only arithmetic simplifies implementation and prevents overflow complexity. Advanced arithmetic requires careful handling in kernel context.

### Why Variables Only?
Global variables are simple to implement correctly in bare metal. Block scope, closures, and dynamic allocation are deferred to user-mode interpreters.

---

## 18. Future Extensions (Post-v0.1)

When user mode exists, these may be added:

- **Labels & Jumps** – Requires label table and program counter
- **Loops** – `FOR`, `WHILE` with break/continue
- **Functions** – Subroutine definition and call
- **Arrays** – Index-based data structures
- **Floating-point** – IEEE 754 arithmetic
- **File I/O** – Read/write to disk
- **Local scope** – Block-level variables

---

## 17. Design Rationale

### Why No Jumps? (v0.1 Temporary Constraint)
Jumps require persistent labels and instruction pointers. In a bare-metal kernel without memory protection, this:
- Enables infinite loops that hang the kernel
- Requires complex exception handling
- Creates unpredictable execution flow
- Breaks kernel stability

**v1.0 will support jumps** with proper OS-level safeguards.

### Why Immediate Mode? (v0.1 Temporary Constraint)
Immediate-mode execution (parse-then-discard) guarantees:
- Constant memory usage (no AST building)
- No execution state to corrupt
- One-pass semantics (simple, predictable)
- No instruction pointer drift

**v1.0 will use traditional VM model** with proper process isolation.

### Why Limited Operators?
Addition-only arithmetic simplifies implementation and prevents overflow complexity. Advanced arithmetic requires careful handling in kernel context.

**This persists in v1.0** for simplicity (multiplication/division added later).

### Why Variables Only? (v0.1 Temporary Constraint)
Global variables are simple to implement correctly in bare metal. Block scope, closures, and dynamic allocation are deferred to user-mode interpreters.

**v1.0 will add function definitions, local scope, and dynamic memory**.

---

## 18. Future Extensions (Post-v0.1 / v1.0)

When the OS advances to support user mode and memory protection, these features return:

### v1.0 Additions (Enabled with MMU/User Mode)
- **Labels & Jumps** – Full `GO YE UNTO` support with label tables
- **Loops** – `FOR`, `WHILE` with break/continue
- **Conditionals** – Full `IF...THEN...ELSE` with nested control flow
- **Functions** – Subroutine definition, local scope, return values
- **Exception Handling** – Try/catch semantics for `WOE UNTO`

### v2.0+ Additions (Post-Process Isolation)
- **Arrays** – Index-based data structures
- **Floating-point** – IEEE 754 arithmetic
- **File I/O** – Read/write to disk via syscalls
- **Structs** – Record/composite types
- **Dynamic Memory** – Heap allocation with garbage collection
- **Recursion** – Full call-stack support

---

## 19. Relationship to Full WOG Spec

This specification is a **strict subset** of the **WOG 1.0 Language Specification**.

| Feature | v0.1 (Current) | v1.0 (Future) |
|---------|----------------|---------------|
| Program structure | AND GOD SAID / AND IT CAME TO PASS | ✓ Same |
| Variables | ✓ Global only | ✓ Global + local scope |
| Arithmetic | ✓ Addition only | ✓ +, -, *, /, % |
| Conditionals | ✓ IF...THEN (no ELSE) | ✓ IF...THEN...ELSE |
| Jumps | ❌ Forbidden | ✓ GO YE UNTO |
| Labels | ❌ Forbidden | ✓ Full support |
| Loops | ❌ Forbidden | ✓ FOR, WHILE |
| Functions | ❌ Forbidden | ✓ Definition & call |
| Type checking | ❌ Syntactic only | ✓ Runtime enforced |
| Error recovery | ❌ WOE UNTO halts | ✓ Exception handling |
| Execution model | ✓ Immediate-mode | Stateful VM |

**See the full WOG 1.0 specification (Appendix D) for the complete language design.**

An implementation **conforms** to this spec if:

1. ✅ Accepts all syntactically valid WOG programs as defined
2. ✅ Rejects all syntactically invalid programs gracefully
3. ✅ Executes valid programs per semantics (sections 5-11)
4. ✅ Enforces resource limits (section 13)
5. ✅ Reports errors safely (section 10)
6. ✅ Respects UI ownership rules (section 12)

---

**WOG Specification v0.1**  
**For Bare-Metal OS Interpreter**  
**Last Updated: January 2026**