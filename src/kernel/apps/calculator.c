/* LazyCalculator – integer only, no suicide version */
#include "calculator.h"
#include "../io/vga.h"
#include "../io/keyboard.h"
#include "../lib/string.h"
#include "../lib/int.h"
#include <stdbool.h>

#define BUF_SZ 256

static void print(const char *s)  { terminal_writestring(s); }
static void println(const char *s){ terminal_writestring(s); terminal_putchar('\n'); }

static void pr_ok(const char *s) {
    terminal_setcolor(VGA_COLOR_GREEN);   terminal_writestring(s);
    terminal_setcolor(VGA_COLOR_LIGHT_GREY);
}

/* ---- read one integer, advance pointer ---- */
static bool read_int(const char **p, int32 *out)
{
    bool neg = false;
    int32 v = 0;

    while (**p == ' ') (*p)++;

    if (**p == '-') { neg = true; (*p)++; }

    if (**p < '0' || **p > '9') return false;

    while (**p >= '0' && **p <= '9') {
        v = v * 10 + (**p - '0');
        (*p)++;
    }

    *out = neg ? -v : v;
    return true;
}

/* ---- evaluate left-to-right expression ---- */
static bool eval_expr(const char *s, int32 *result)
{
    int32 acc;
    if (!read_int(&s, &acc)) return false;

    for (;;) {
        while (*s == ' ') s++;
        char op = *s++;
        if (op == '\0') break;

        int32 rhs;
        if (!read_int(&s, &rhs)) return false;

        switch (op) {
        case '+': acc += rhs; break;
        case '-': acc -= rhs; break;
        case '*': acc *= rhs; break;
        case '/':
            if (rhs == 0) {
                print("ERROR: divide by zero\n");
                return false;
            }
            acc = int32_div(acc, rhs);
            break;
        default:
            return false;
        }
    }

    *result = acc;
    return true;
}

/* ---- int to string ---- */
static void itoa(int32 v, char *buf)
{
    char tmp[12];
    int i = 0;
    bool neg = false;

    if (v == 0) { buf[0] = '0'; buf[1] = '\0'; return; }

    if (v < 0) { neg = true; v = -v; }

    while (v) {
        tmp[i++] = '0' + (v % 10);
        v /= 10;
    }

    if (neg) tmp[i++] = '-';

    int j = 0;
    while (i--) buf[j++] = tmp[i];
    buf[j] = '\0';
}

/* ---- interactive shell ---- */
void calculator_run(void)
{
    static char line[BUF_SZ];
    static char out[32];

    println("");
    pr_ok("=== LazyDOS Calculator (int-only) ===");
    println("");
    println("Left-to-right evaluation. Type 'exit' to quit.");

    for (;;) {
        print("calc> ");

        int i = 0;
        for (;;) {
            char c = lazy_getchar();
            if (c == '\n' || c == '\r') {
                terminal_putchar('\n');
                line[i] = '\0';
                break;
            }
            if ((c == '\b' || c == 0x7F) && i > 0) {
                i--;
                terminal_putchar('\b');
                terminal_putchar(' ');
                terminal_putchar('\b');
                continue;
            }
            if (c >= 32 && c <= 126 && i < BUF_SZ - 1) {
                line[i++] = c;
                terminal_putchar(c);
            }
        }

        if (!strcmp(line, "exit") || !strcmp(line, "quit"))
            break;

        int32 res;
        if (eval_expr(line, &res)) {
            itoa(res, out);
            println(out);
        } else {
            print("ERROR: invalid expression\n");
        }
    }

    println("Calculator exited");
}
