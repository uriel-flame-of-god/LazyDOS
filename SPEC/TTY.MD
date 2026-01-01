## SPEC/TTY.MD

# LazyDOS TTY - The Simple Shell

## The Philosophy
Like DOS's COMMAND.COM, our TTY is a simple command interpreter. No scripting, no job control, no fancy features. Just type commands, see results.

## What It Does
1. **Shows a prompt**: `LazyDOS> `
2. **Reads input**: From keyboard
3. **Runs commands**: Built-in or applications
4. **Shows output**: On screen

## The Command Loop (Simple)
```c
while (1) {
    show_prompt();
    read_line();
    run_command();
}
```

## Built-in Commands
| Command | What It Does | Why It's Useful |
|---------|--------------|-----------------|
| `help` | Shows commands | When you forget |
| `calc` | Starts calculator | For math |
| `qbasic` | Starts QBASIC | For programming |
| `cfetch` | Shows system info | For showing off |
| `reboot` | Restarts system | When stuck |
| `clear` | Clears screen | For cleanliness |
| `echo` | Repeats text | For testing |

## Input Handling (Simple)
- **Backspace works**: Delete last character
- **Enter submits**: Run command
- **256 char limit**: More than enough
- **No line editing**: Just backspace

## Output Formatting
- **Green prompt**: So you know it's ready
- **Normal text**: Light grey
- **Error messages**: Red (when we have them)
- **No colors otherwise**: Keep it simple

## Application Launching
We just call functions:
```c
if (!strcmp(input, "calc")) {
    calculator_run();  // Run calculator
    return;           // Back to shell
}
```

No process creation, no context switching. Simple.

## Why This Simplicity Works
1. **Fast**: No overhead
2. **Reliable**: Few moving parts
3. **Understandable**: You can see how it works
4. **DOS-like**: Feels familiar

## What We Don't Have
- Command history
- Tab completion
- Pipes (`|`)
- Redirection (`>`, `<`)
- Variables
- Scripting
- Aliases

We might add these later, but only if they stay simple.

## The Prompt
```
LazyDOS> _
```

Simple, clear, obvious.

## Error Messages
```
Unknown: badcommand
```

Short, to the point.

## Testing
If you can type `help` and see help, it works.

## Future Ideas (Keep It Simple)
1. **Command history**: Up/down arrows
2. **File commands**: `dir`, `type`
3. **Batch files**: Simple scripting
4. **Environment**: Simple variables

**LazyDOS Principle**: A shell should get out of your way. Ours does.
