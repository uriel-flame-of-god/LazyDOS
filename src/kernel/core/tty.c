/*  TTY + integrated shell  –  LazyDOS “everything-in-one”  */
#include "../io/vga.h"
#include "../io/keyboard.h"
#include "../apps/calculator.h"   /* <-- updated path */
#include "../lib/string.h"
#include "../io/port.h"
#include <stdbool.h>

#define PROMPT  "BetterDOS> "
#define BUF_SZ  256

static char input[BUF_SZ];

/*  ----------  helpers  ----------  */
static void print(const char *s) { terminal_writestring(s); }
static void println(const char *s){ print(s); terminal_putchar('\n'); }

/*  ----------  commands  ----------  */
static void cmd_help(void)
{
    println("LazyDOS v0.1 – Available commands:");
    println("  help      – this screen");
    println("  calc      – calculator");
    println("  cfetch    – quick system info");
    println("  info      – more details");
    println("  reboot    – restart");
    println("  shutdown  – power off (sort of)");
    println("  clear     – clear screen");
    println("  echo      – echo text");
}

static void cmd_cfetch(void)
{
    /*  logo:  LDOS  (matching your ASCII)  */
    println("     ##:::::::'########:::'#######:::'######::");
    println("     ##::::::: ##.... ##:'##.... ##:'##... ##:");
    println("     ##::::::: ##:::: ##: ##:::: ##: ##:::..::");
    println("     ##::::::: ##:::: ##: ##:::: ##:. ######::");
    println("     ##::::::: ##:::: ##: ##:::: ##::..... ##:");
    println("     ##::::::: ##:::: ##: ##:::: ##:'##::: ##:");
    println("     ########: ########::. #######::. ######::");
    println("     ........::........::::.......::::......:::");
    println("");
    println("LazyDOS v0.1        –  works well enough");
    println("CPU : i486 (probably)");
    println("RAM : 640 KB conventional");
    println("Boot: Multiboot");
    println("Shell: LazyTTY (built-in)");
}

static void cmd_info(void)
{
    println("=== LazyDOS System Information ===");
    println("Kernel Version : 0.1.0");
    println("Architecture   : 32-bit x86");
    println("Boot Method    : Multiboot 1.0");
    println("Memory Layout  : 1 MB load, stack elsewhere");
    println("Drivers        : VGA text, polling keyboard");
    println("==================================");
}

static void cmd_reboot(void)
{
    println("Rebooting…");
    /* keyboard-controller reset (works on real hardware + QEMU/BOCHS) */
    while (inb(0x64) & 0x02) ;
    outb(0x64, 0xFE);
    for(;;) asm volatile ("hlt");
}

static void cmd_shutdown(void)
{
    println("Shutdown – please power-off manually.");
    /* QEMU/BOCHS shortcut if you want: outw(0x604, 0x2000); */
    for(;;) asm volatile ("hlt");
}

static void cmd_clear(void)
{
    terminal_initialize();
}

static void cmd_echo(void)
{
    /* echo everything after “echo ” until newline */
    char *p = input + 5;
    while (*p == ' ') p++;
    println(p);
}

/*  ----------  dispatcher  ----------  */
typedef struct { const char *name; void (*fn)(void); } cmd_t;
static const cmd_t cmds[] = {
    {"help",      cmd_help},
    {"calc",      calculator_run},
    {"calculator",calculator_run},
    {"cfetch",    cmd_cfetch},
    {"info",      cmd_info},
    {"reboot",    cmd_reboot},
    {"shutdown",  cmd_shutdown},
    {"clear",     cmd_clear},
    {"cls",       cmd_clear},
    {"echo",      cmd_echo},
    {NULL, NULL}
};

static void run_cmd(void)
{
    for (const cmd_t *c = cmds; c->name; ++c)
        if (!strcmp(input, c->name)) { c->fn(); return; }

    /* not found */
    print("Unknown: ");
    println(input);
}

/*  ----------  main TTY loop  ----------  */
/*  ----------  main TTY loop  ----------  */
/*  ----------  main TTY loop  ----------  */
void tty_main(void)
{
    terminal_initialize();
    println("LazyDOS TTY – integrated shell");
    println("Type 'help' for commands");

    for (;;) {
        print(PROMPT);
        size_t i = 0;
        input[0] = '\0';  /* Clear input buffer */
        
        while (1) {
            char c = lazy_getchar();
            
            /* Handle Enter key */
            if (c == '\n' || c == '\r') {
                terminal_putchar('\n');
                input[i] = '\0';
                break;
            }
            
            /* Handle backspace */
            if (c == '\b' || c == 0x7F) {
                if (i > 0) {
                    i--;
                    terminal_putchar('\b');
                    terminal_putchar(' ');
                    terminal_putchar('\b');
                }
                continue;
            }
            
            /* Handle printable characters */
            if (i < BUF_SZ - 1 && c >= 32 && c <= 126) {
                input[i++] = c;
                terminal_putchar(c);
            }
        }
        
        /* Skip empty commands */
        if (input[0] == '\0') {
            continue;
        }
        
        /* Execute command */
        run_cmd();
    }
}