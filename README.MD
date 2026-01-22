# LazyDOS v0.0.6

![Release](https://img.shields.io/badge/Release-Jan_2026-green)
![QBASIC](https://img.shields.io/badge/QBASIC-✅_Stable-blue)
![WOG](https://img.shields.io/badge/WOG-🟡_QBASIC--Style-orange)
![Version](https://img.shields.io/badge/Version-v0.0.6-orange)

📅 **Release Date:** 22/01/2026

---

## 📦 What’s New in v0.0.6

LazyDOS v0.0.6 is a **minor-but-meaningful architectural update** that refines interpreter behavior and core libraries without changing the overall system scope.

This release focuses on:

* Interpreter **correctness**
* Kernel-safe execution models
* Clear separation between **QBASIC** and **WOG**
* Safer string handling

No UI changes. No marketing fluff. Just correctness.

---

## 🧠 Design Philosophy (Unchanged, Reinforced)

LazyDOS currently runs in:

* Single-threaded kernel
* Ring 0 only
* No paging
* No user space
* No MMU protection

Because of this:

> **All interpreters must be linear, blocking, and deterministic.**

v0.0.6 reinforces this rule by ensuring **WOG follows a QBASIC-style execution model**, eliminating unsafe VM-like behavior.

---

## 🧰 Interpreters Overview

### ✅ QBASIC — Stable and Feature-Rich (for LazyDOS)

QBASIC remains the most capable interpreter in LazyDOS.

#### Supported Features

* `LET` (integers and strings)
* `PRINT`
* `INPUT`
* `IF / THEN / ELSE`
* `GOTO`
* `FOR / NEXT`
* Labels
* Optional line numbers

#### Execution Model

* Explicit execution pointer
* Line map + label resolution
* Bounded FOR/NEXT stack
* Deterministic control flow

QBASIC is intentionally **DOS-like**, not modern.

---

### 🟡 WOG — Word of God Language (QBASIC-Style)

In v0.0.6, WOG is **officially redefined** as:

> **A linear, immediate-mode language with no persistent execution state.**

#### Program Structure

```text
AND GOD SAID
  <statements>
AND IT CAME TO PASS
```

#### Supported Statements

* `BEHOLD` — print
* `THOU SHALT` — assignment
* `LET THERE BE` — declaration
* `IF <condition> THEN <single statement>`
* `WOE UNTO "message"` — error + halt

#### Explicitly Not Supported (by design)

* ❌ Labels
* ❌ GOTO / jumps
* ❌ Loops
* ❌ Stored AST / VM
* ❌ Multi-line control flow

This keeps WOG **kernel-safe** and predictable.

---

## 📏 Hard Limits (Enforced)

| Resource       | Limit             |
| -------------- | ----------------- |
| Variables      | 128               |
| Line length    | 256 bytes         |
| Program size   | 8 KB              |
| Variable name  | 32 bytes          |
| String literal | 256 bytes         |
| Integer size   | 32-bit            |
| Execution      | Linear only (WOG) |

All limits are **hard**, not advisory.

---

## ⌨️ Controls

* **Ctrl+R** — Run program
* **Ctrl+X** — Exit interpreter
* **Backspace** — Delete character
* **Enter** — New line

During execution:

* Editor UI is frozen
* Interpreter owns the terminal
* Screen is reset once per run

---

## 📁 Project Structure (Relevant Parts)

```
src/
└── kernel/
    ├── apps/
    │   ├── qbasic.c   # QBASIC interpreter
    │   ├── wog.c      # WOG (QBASIC-style)
    │   ├── qbasic.h
    │   └── wog.h
    ├── io/
    └── lib/
        ├── string.c
        └── string.h   # includes strncmp()
```

---

## 📚 String Library Update (v0.0.6)

The string library now includes:

```c
int strncmp(const char* a, const char* b, size_t n);
```

This enables:

* Safer keyword matching
* Bounded comparisons
* Reduced risk of buffer overreads

Both interpreters rely exclusively on **bounded string operations**.

---

## 🧪 Example: WOG Program

```text
AND GOD SAID
LET THERE BE x INT
THOU SHALT x AND 5
IF x > 3 THEN BEHOLD "Greater than three"
BEHOLD x
AND IT CAME TO PASS
```

---

## 🧪 Example: QBASIC Program

```basic
10 FOR i = 1 TO 3
20 PRINT "i = ", i
30 NEXT i
40 PRINT "Done"
```

---

## 🐛 Known Limitations (Intentional)

### WOG

* No loops
* No jumps
* Single-statement IF only
* Linear execution

### QBASIC

* Fixed memory limits
* Integer-only arithmetic
* No arrays yet
* No filesystem I/O

These are **architectural constraints**, not bugs.

---

## 🔮 What’s Next

* Improve diagnostics (both interpreters)
* More example programs
* Editor quality-of-life improvements

Future language expansion **requires paging or user mode** and will not be rushed.

---

## 🎯 Versioning Note

LazyDOS follows **semantic versioning**:

```
Major.Minor.Patch
```

* Architectural refinements → **patch bump**
* This release qualifies → **v0.0.6**

---

## 🧭 Final Thoughts

LazyDOS v0.0.6 is about **discipline over ambition**:

* No unsafe abstractions
* No pretending Ring 0 is user space
* No accidental virtual machines

QBASIC and WOG now each do **exactly what they are supposed to do** — no more, no less.

---

> *“Simplicity is prerequisite for reliability.”* — Edsger W. Dijkstra
> *“If it runs in the kernel, it must behave.”* — LazyDOS

**LazyDOS v0.0.6 — Small Update, Correct Direction.**
