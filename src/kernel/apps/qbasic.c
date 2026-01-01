#include "../io/vga.h"
#include "../io/keyboard.h"
#include "../lib/string.h"
#include "../apps/qbasic.h"
#include <stdbool.h>

#define MAX_LINES 100
#define MAX_LINE_LEN 128
#define MAX_VARS 50
#define MAX_CODE_LEN 5000

typedef enum { VAR_INT, VAR_STR } var_type;

typedef struct {
    char name[32];
    var_type type;
    union {
        int32_t int_val;
        char str_val[128];
    } val;
} variable;

typedef struct {
    char code[MAX_CODE_LEN];
    size_t code_len;
    variable vars[MAX_VARS];
    int var_count;
    int exec_line;
} qbasic_state;

static qbasic_state qb;
static char editor_buf[MAX_CODE_LEN];
static size_t editor_pos = 0;
static bool running = false;

static void print_str(const char *s) { terminal_writestring(s); }
static void print_char(char c) { terminal_putchar(c); }
static void print_nl(void) { terminal_putchar('\n'); }

static void set_color(uint8_t color) { terminal_setcolor(color); }



static variable* find_var(const char *name)
{
    for (int i = 0; i < qb.var_count; i++) {
        if (!strcmp(qb.vars[i].name, name)) return &qb.vars[i];
    }
    return NULL;
}

static variable* create_var(const char *name, var_type type)
{
    if (qb.var_count >= MAX_VARS) return NULL;
    variable *v = &qb.vars[qb.var_count++];
    strcpy(v->name, name);
    v->type = type;
    if (type == VAR_INT) v->val.int_val = 0;
    else v->val.str_val[0] = '\0';
    return v;
}

static int32_t parse_int(const char *s)
{
    int32_t val = 0;
    bool neg = false;
    if (*s == '-') { neg = true; s++; }
    while (*s >= '0' && *s <= '9') {
        val = val * 10 + (*s - '0');
        s++;
    }
    return neg ? -val : val;
}

static void int_to_str(int32_t num, char *buf)
{
    if (num == 0) { strcpy(buf, "0"); return; }
    bool neg = num < 0;
    if (neg) num = -num;
    char tmp[32];
    int i = 0;
    while (num > 0) {
        tmp[i++] = '0' + (num % 10);
        num /= 10;
    }
    if (neg) tmp[i++] = '-';
    tmp[i] = '\0';
    for (int j = 0; j < i; j++) buf[j] = tmp[i - 1 - j];
    buf[i] = '\0';
}

static char* trim_start(char *s)
{
    while (*s == ' ' || *s == '\t') s++;
    return s;
}

static char* extract_token(char *line, char *token, size_t max)
{
    line = trim_start(line);
    size_t i = 0;
    while (i < max - 1 && *line && *line != ' ' && *line != '\t' && *line != '=' && *line != '"' && *line != ',') {
        token[i++] = *line++;
    }
    token[i] = '\0';
    return line;
}

static bool is_number(const char *s)
{
    char *t = trim_start((char*)s);
    if (*t == '-') t++;
    int digits = 0;
    while (*t >= '0' && *t <= '9') { digits++; t++; }
    return digits > 0;
}

static void read_input_line(char *buf, size_t max)
{
    size_t i = 0;
    while (1) {
        char c = lazy_getchar();
        if (c == '\r' || c == '\n') {
            print_nl();
            break;
        }
        if (c == '\b' || c == 0x7F) {
            if (i > 0) {
                i--;
                terminal_putchar('\b');
                terminal_putchar(' ');
                terminal_putchar('\b');
            }
            continue;
        }
        if (c >= 32 && c <= 126 && i < max - 1) {
            buf[i++] = c;
            terminal_putchar(c);
        }
    }
    buf[i] = '\0';
}

static void execute_print(char *args)
{
    args = trim_start(args);
    while (*args) {
        if (*args == '"') {
            args++;
            while (*args && *args != '"') {
                print_char(*args++);
            }
            if (*args == '"') args++;
        } else if (*args == ',') {
            print_str("    ");
            args++;
        } else {
            char var_name[32];
            args = extract_token(args, var_name, sizeof(var_name));
            args = trim_start(args);
            variable *v = find_var(var_name);
            if (v) {
                if (v->type == VAR_INT) {
                    char buf[32];
                    int_to_str(v->val.int_val, buf);
                    print_str(buf);
                } else {
                    print_str(v->val.str_val);
                }
            } else {
                if((var_name[0]>='0')&&(var_name[0]<='9')||((var_name[0]=='-')&&(var_name[1]>='0')&&(var_name[1]<='9')))
                {
                    char buf[32];
                    int_to_str(parse_int(var_name), buf);
                    print_str(buf);
                }
                else
                {
                    print_str("?");
                }
            }
        }
    }
    print_nl();
}

static void execute_let(char *args)
{
    args = trim_start(args);
    char var_name[32];
    args = extract_token(args, var_name, sizeof(var_name));
    args = trim_start(args);
    
    if (*args == '=') args++;
    args = trim_start(args);
    
    variable *v = find_var(var_name);
    if (!v) {
        if (*args == '"') {
            v = create_var(var_name, VAR_STR);
        } else {
            v = create_var(var_name, VAR_INT);
        }
    }
    
    if (v->type == VAR_INT) {
        v->val.int_val = parse_int(args);
    } else {
        if (*args == '"') args++;
        size_t i = 0;
        while (*args && *args != '"' && i < 127) {
            v->val.str_val[i++] = *args++;
        }
        v->val.str_val[i] = '\0';
    }
}

static void execute_input(char *args)
{
    args = trim_start(args);
    char *p = args;
    while (*p) {
        char var_name[32];
        p = extract_token(p, var_name, sizeof(var_name));
        p = trim_start(p);
        if (var_name[0] == '\0') break;
        if (*p == ',') { p++; p = trim_start(p); }

        // Prompt
        print_str("?");
        print_str(" ");
        char buf[128];
        read_input_line(buf, sizeof(buf));
        // Decide type and assign
        variable *v = find_var(var_name);
        if (!v) {
            if (is_number(buf)) {
                v = create_var(var_name, VAR_INT);
            } else {
                v = create_var(var_name, VAR_STR);
            }
        }
        if (v->type == VAR_INT) {
            v->val.int_val = parse_int(buf);
        } else {
            size_t i = 0;
            char *s = buf;
            while (*s && i < sizeof(v->val.str_val) - 1) {
                v->val.str_val[i++] = *s++;
            }
            v->val.str_val[i] = '\0';
        }
    }
}

static void execute_line(char *line);

static bool eval_condition(char *cond)
{
    cond = trim_start(cond);
    char var_name[32];
    cond = extract_token(cond, var_name, sizeof(var_name));
    cond = trim_start(cond);
    
    variable *v = find_var(var_name);
    if (!v || v->type != VAR_INT) return false;
    
    int32_t val = v->val.int_val;
    if (*cond == '=' && *(cond + 1) != '=') {
        cond++;
        return val == parse_int(trim_start(cond));
    } else if (*cond == '<' && *(cond + 1) == '=') {
        cond += 2;
        return val <= parse_int(trim_start(cond));
    } else if (*cond == '>' && *(cond + 1) == '=') {
        cond += 2;
        return val >= parse_int(trim_start(cond));
    } else if (*cond == '<' && *(cond + 1) == '>') {
        cond += 2;
        return val != parse_int(trim_start(cond));
    } else if (*cond == '<') {
        cond++;
        return val < parse_int(trim_start(cond));
    } else if (*cond == '>') {
        cond++;
        return val > parse_int(trim_start(cond));
    }
    return false;
}
static void run_statement(char *stmt)
{
    stmt = trim_start(stmt);
    if (!*stmt) return;

    /* If statement begins with a string literal, print it directly */
    if (*stmt == '"') {
        execute_print(stmt);
        return;
    }

    /* Otherwise parse command and dispatch */
    char cmd[32];
    char tmp[MAX_LINE_LEN];
    strcpy(tmp, stmt);
    char *rest = extract_token(tmp, cmd, sizeof(cmd));
    rest = trim_start(rest);

    if (!strcmp(cmd, "PRINT")) {
        execute_print(rest);
    } else if (!strcmp(cmd, "LET")) {
        execute_let(rest);
    } else if (!strcmp(cmd, "INPUT")) {
        execute_input(rest);
    } else if (!strcmp(cmd, "IF")) {
        /* pass through to IF handler */
        execute_line(tmp);
    } else {
        /* not a known command: try to PRINT it (fallback) */
        execute_print(stmt);
    }
}
static void execute_line(char *line)
{
    line = trim_start(line);
    if (!*line || *line == '\'') return;
    
    char cmd[32];
    line = extract_token(line, cmd, sizeof(cmd));
    line = trim_start(line);
    
    if (!strcmp(cmd, "PRINT")) {
        execute_print(line);
    } else if (!strcmp(cmd, "LET")) {
        execute_let(line);
    } else if (!strcmp(cmd, "INPUT")) {
        execute_input(line);
    } else if (!strcmp(cmd, "IF")) {
        char buf[256];
        strcpy(buf, line);

        /* Try to find THEN first (classic form: IF ... THEN action [ELSE action]) */
        char *then_found = NULL;
        for (char *p = buf; *p; p++) {
            if (p[0] == 'T' && p[1] == 'H' && p[2] == 'E' && p[3] == 'N') {
                then_found = p;
                break;
            }
        }

        if (then_found) {
            *then_found = '\0';
            char *action = trim_start(then_found + 4);

            /* find ELSE inside action, if any */
            char *else_found = NULL;
            for (char *p = action; *p; p++) {
                if (p[0] == 'E' && p[1] == 'L' && p[2] == 'S' && p[3] == 'E') {
                    else_found = p;
                    break;
                }
            }

            if (eval_condition(buf)) {
                if (else_found) {
                    *else_found = '\0';
                    run_statement(action);
                } else {
                    run_statement(action);
                }
            } else {
                if (else_found) {
                    run_statement(trim_start(else_found + 4));
                }
            }
        } else {
            /* No THEN: allow "IF <cond> <action> [ELSE <action>]" */
            char cond_buf[256];
            char action_buf[256];

            char *s = trim_start(line);
            char varname[32];
            char *after_var = extract_token(s, varname, sizeof(varname));
            after_var = trim_start(after_var);

            /* detect operator length */
            char *op = after_var;
            int op_len = 0;
            if (op[0] == '<' && op[1] == '>') op_len = 2;
            else if ((op[0] == '<' || op[0] == '>') && op[1] == '=') op_len = 2;
            else if (op[0] == '<' || op[0] == '>' || op[0] == '=') op_len = 1;

            char *rhs = op + op_len;
            rhs = trim_start(rhs);

            /* skip optional '-' and digits of RHS to find start of action */
            char *r = rhs;
            if (*r == '-') r++;
            while (*r >= '0' && *r <= '9') r++;
            char *action = trim_start(r);

            /* copy condition (from start to r) */
            size_t con_len = (size_t)(r - s);
            if (con_len >= sizeof(cond_buf)) con_len = sizeof(cond_buf) - 1;
            memcpy(cond_buf, s, con_len);
            cond_buf[con_len] = '\0';

            /* look for ELSE inside action */
            char *else_found = NULL;
            for (char *p = action; *p; p++) {
                if (p[0] == 'E' && p[1] == 'L' && p[2] == 'S' && p[3] == 'E') {
                    else_found = p;
                    break;
                }
            }

            if (eval_condition(cond_buf)) {
                if (else_found) {
                    size_t act_len = (size_t)(else_found - action);
                    if (act_len >= sizeof(action_buf)) act_len = sizeof(action_buf) - 1;
                    memcpy(action_buf, action, act_len);
                    action_buf[act_len] = '\0';
                    run_statement(trim_start(action_buf));
                } else {
                    run_statement(trim_start(action));
                }
            } else {
                if (else_found) {
                    run_statement(trim_start(else_found + 4));
                }
            }
        }
    }
}

static void run_program(void)
{
    qb.var_count = 0;
    char line_buf[MAX_LINE_LEN];
    size_t pos = 0;
    
    while (pos < editor_pos) {
        size_t i = 0;
        while (pos < editor_pos && editor_buf[pos] != '\n' && i < MAX_LINE_LEN - 1) {
            line_buf[i++] = editor_buf[pos++];
        }
        line_buf[i] = '\0';
        if (editor_buf[pos] == '\n') pos++;
        
        execute_line(line_buf);
    }
}

static void editor_display(void)
{
    terminal_initialize();
    set_color(VGA_COLOR_CYAN);
    print_str("=== QBASIC EDITOR (Ctrl+R: Run, Ctrl+X: Exit) ===");
    print_nl();
    set_color(VGA_COLOR_LIGHT_GREY);
    
    size_t line = 1;
    size_t pos = 0;
    while (pos < editor_pos && line < 20) {
        char tmp[MAX_LINE_LEN];
        size_t i = 0;
        while (pos < editor_pos && editor_buf[pos] != '\n' && i < MAX_LINE_LEN - 1) {
            tmp[i++] = editor_buf[pos++];
        }
        tmp[i] = '\0';
        if (editor_buf[pos] == '\n') pos++;
        print_str(tmp);
        print_nl();
        line++;
    }
}

static void editor_loop(void)
{
    editor_pos = 0;
    editor_buf[0] = '\0';
    
    while (1) {
        editor_display();
        char c = lazy_getchar();
        /* Ctrl+R → RUN */
        if (c == 0x12) {   /* Ctrl+R */
            terminal_initialize();
            set_color(VGA_COLOR_GREEN);
            print_str("=== OUTPUT ===");
            print_nl();
            set_color(VGA_COLOR_LIGHT_GREY);
            run_program();
            print_nl();
            print_str("Press any key...");
            lazy_getchar();
            continue;
        }

        /* Ctrl+X → EXIT */
        if (c == 0x18) {   /* Ctrl+X */
            return;
        }
        
        else if (c == '\b' || c == 0x7F) {
            if (editor_pos > 0) editor_pos--;
        } else if (c == '\n' || c == '\r') {
            if (editor_pos < MAX_CODE_LEN - 1) {
                editor_buf[editor_pos++] = '\n';
            }
        } else if (c >= 32 && c <= 126) {
            if (editor_pos < MAX_CODE_LEN - 1) {
                editor_buf[editor_pos++] = c;
            }
        }
        editor_buf[editor_pos] = '\0';
    }
}

static void qbasic_run_interactive(void)
{
    editor_loop();
}


void qbasic_run(const char* code)
{
    if (code && *code) {
        /* run code provided as a single string */
        size_t i = 0;
        while (i < MAX_CODE_LEN - 1 && code[i]) {
            editor_buf[i] = code[i];
            i++;
        }
        editor_pos = i;
        editor_buf[editor_pos] = '\0';

        terminal_initialize();
        set_color(VGA_COLOR_GREEN);
        print_str("=== QBASIC: running code ===");
        print_nl();
        set_color(VGA_COLOR_LIGHT_GREY);
        run_program();
        print_nl();
        print_str("Press any key...");
        lazy_getchar();
    } else {
        qbasic_run_interactive();
    }
}