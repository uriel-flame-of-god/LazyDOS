/*  calculator.c  –  LazyDOS “slightly-bloated” stack calculator  */
#include "calculator.h"
#include "../io/vga.h"
#include "../io/keyboard.h"
#include "../lib/string.h"
#include <stdbool.h>

#ifndef LAZY_TYPE
#define LAZY_TYPE 0          /* 0=double  1=float  2=int32_t */
#endif
#if LAZY_TYPE == 0
typedef double   num_t;
#elif LAZY_TYPE == 1
typedef float    num_t;
#else
typedef int32_t  num_t;
#endif

#define STACK_MAX 64
#define LEX_BUF   256

static num_t   val_st[STACK_MAX];
static uint8_t op_st[STACK_MAX];
static int val_top, op_top;

static inline void val_push(num_t v) { val_st[val_top++] = v; }
static inline num_t val_pop(void)    { return val_st[--val_top]; }
static inline void op_push(uint8_t o){ op_st[op_top++] = o; }
static inline uint8_t op_pop(void)   { return op_st[--op_top]; }
static inline uint8_t op_peek(void)  { return op_st[op_top - 1]; }

/* ---------- tiny helpers ---------- */
static void print(const char *s) { terminal_writestring(s); }
static void println(const char *s){ terminal_writestring(s); terminal_putchar('\n'); }

static void pr_ok(const char *s) {
    terminal_setcolor(VGA_COLOR_GREEN);   terminal_writestring(s);
    terminal_setcolor(VGA_COLOR_LIGHT_GREY);
}
static void pr_err(const char *s) {
    terminal_setcolor(VGA_COLOR_RED);     terminal_writestring(s);
    terminal_setcolor(VGA_COLOR_LIGHT_GREY);
}

/* ---------- lazy strtoX / numtoa ---------- */
static num_t strtonum(const char *s, char **end)
{
#if LAZY_TYPE == 0 || LAZY_TYPE == 1
    bool neg = false;
    if (*s == '-') { neg = true; ++s; }
    num_t intp = 0, fracp = 0, div = 1;
    const char *p = s;
    while (*p >= '0' && *p <= '9') intp = intp * 10 + (*p++ - '0');
    if (*p == '.') {
        ++p;
        while (*p >= '0' && *p <= '9') { fracp = fracp * 10 + (*p - '0'); div *= 10; ++p; }
    }
    *end = (char *)p;
    num_t v = intp + fracp / div;
    return neg ? -v : v;
#else
    char *e; long v = strtol(s, &e, 10); *end = e; return (num_t)v;
#endif
}

static void numtoa(num_t v, char *buf, int prec)
{
#if LAZY_TYPE == 2          /* plain integer */
    char tmp[16], *p = tmp;
    int32_t x = (int32_t)v, neg = 0;
    if (x < 0) { neg = 1; x = -x; }
    do { *p++ = '0' + x % 10; x /= 10; } while (x);
    if (neg) *p++ = '-';
    *p-- = '\0';
    char *b = buf;
    while (p >= tmp) *b++ = *p--; *b = '\0';

#else                       /* float / double  –  32-bit only */
    bool neg = false;
    if (v < 0) { neg = true; v = -v; }
    uint32_t ip = (uint32_t)v;                 /* whole part */
    uint32_t fp = (uint32_t)((v - ip) * 1e6f); /* 6-digit frac */
    char tmp[24], *p = tmp;
    do { *p++ = '0' + ip % 10; ip /= 10; } while (ip);
    if (neg) *p++ = '-';
    *p-- = '\0';
    char *b = buf;
    while (p >= tmp) *b++ = *p++;
    *b++ = '.';
    uint32_t div = 100000;
    while (prec-- && div) { *b++ = '0' + fp / div; fp %= div; div /= 10; }
    *b = '\0';
#endif
}
/* ---------- token stream ---------- */
typedef enum { END, NUM, ADD, SUB, MUL, DIV, LP, RP, ERR } tok_t;
static struct { tok_t t; num_t v; } tok;
static const char *expr;

static void next_tok(void)
{
    while (*expr == ' ') ++expr;
    char c = *expr;
    if (c == '\0') { tok.t = END; return; }
    switch (c) {
    case '+': tok.t = ADD; ++expr; return;
    case '-': tok.t = SUB; ++expr; return;
    case '*': tok.t = MUL; ++expr; return;
    case '/': tok.t = DIV; ++expr; return;
    case '(': tok.t = LP;  ++expr; return;
    case ')': tok.t = RP;  ++expr; return;
    }
    if ((c >= '0' && c <= '9') || c == '.') {
        char *end; tok.v = strtonum(expr, &end);
        if (end == expr) { tok.t = ERR; return; }
        expr = end; tok.t = NUM; return;
    }
    tok.t = ERR;
}

static int prec(tok_t op)
{
    switch (op) {
    case ADD: case SUB: return 1;
    case MUL: case DIV: return 2;
    default:            return 0;
    }
}
static bool left_assoc(tok_t op) { (void)op; return true; }

static bool apply_op(void)
{
    if (val_top < 2 || op_top == 0) return false;
    uint8_t op = op_pop();
    num_t b = val_pop(), a = val_pop(), r = 0;
    switch (op) {
    case ADD: r = a + b; break;
    case SUB: r = a - b; break;
    case MUL: r = a * b; break;
    case DIV:
        if (b == 0) { pr_err("ERROR: divide by zero\n"); return false; }
        r = a / b; break;
    default: return false;
    }
    val_push(r);
    return true;
}

static num_t evaluate(const char *s)
{
    val_top = op_top = 0;
    expr = s;
    next_tok();
    while (tok.t != END) {
        if (tok.t == NUM) { val_push(tok.v); next_tok(); continue; }
        if (tok.t == LP) { op_push(LP); next_tok(); continue; }
        if (tok.t == RP) {
            while (op_top && op_peek() != LP) if (!apply_op()) goto fail;
            if (op_top == 0) goto fail;
            op_pop(); /* drop '(' */
            next_tok();
            continue;
        }
        if (tok.t >= ADD && tok.t <= DIV) {
            while (op_top && op_peek() != LP &&
                   (prec(op_peek()) >  prec(tok.t) ||
                   (prec(op_peek()) == prec(tok.t) && left_assoc(tok.t))))
                if (!apply_op()) goto fail;
            op_push(tok.t);
            next_tok();
            continue;
        }
        goto fail;
    }
    while (op_top) if (!apply_op()) goto fail;
    if (val_top != 1) goto fail;
    return val_pop();

fail:
    pr_err("ERROR: syntax\n");
    return 0;
}

/* ---------- interactive shell ---------- */
void calculator_run(void)
{
    println("");
    pr_ok("=== LazyDOS Calculator ===\n");
    println("Type expression or 'exit' to quit");
    char line[LEX_BUF];
    for (;;) {
        print("calc> ");
        size_t i = 0;
        for (;;) {
            char c = lazy_getchar();
            if (c == '\n') { line[i] = '\0'; break; }
            if (c == '\b' && i) { --i; terminal_putchar('\b'); continue; }
            if (i < sizeof(line)-1 && c >= ' ') { line[i++] = c; terminal_putchar(c); }
        }
        terminal_putchar('\n');
        if (!strcmp(line, "exit") || !strcmp(line, "quit")) break;
        num_t v = evaluate(line);
        if (line[0]) { numtoa(v, line, 6); println(line); }
    }
    println("Calculator exited");
}

int calc_is_command(const char *s)
{
    return !strcmp(s, "calc") || !strcmp(s, "calculator");
}