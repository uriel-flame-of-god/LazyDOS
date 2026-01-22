#include "../io/vga.h"
#include "../io/keyboard.h"
#include "../lib/string.h"
#include "../apps/qbasic.h"
#include <stdbool.h>

#define MAX_LINES 100
#define MAX_LINE_LEN 128
#define MAX_VARS 50
#define MAX_CODE_LEN 5000
#define MAX_FOR_STACK 10

typedef enum { VAR_INT, VAR_STR } var_type;

typedef struct {
    char name[32];
    var_type type;
    union {
        int32_t int_val;
        char str_val[128];
    } val;
} variable;

/* Line entry: maps line number to code position */
typedef struct {
    int line_num;
    size_t code_pos;
} line_entry;

/* FOR loop state */
typedef struct {
    char var_name[32];
    int32_t to_val;
    size_t loop_start_pos;
    int depth;
} for_loop_state;

typedef struct {
    char code[MAX_CODE_LEN];
    size_t code_len;
    variable vars[MAX_VARS];
    int var_count;
    
    line_entry lines[MAX_LINES];
    int line_count;
    
    for_loop_state for_stack[MAX_FOR_STACK];
    int for_depth;
    
    size_t exec_pos;  /* current execution position */
} qbasic_state;

static qbasic_state qb;
static char editor_buf[MAX_CODE_LEN];
static size_t editor_pos = 0;

static void print_str(const char *s) { terminal_writestring(s); }
static void print_char(char c) { terminal_putchar(c); }
static void print_nl(void) { terminal_putchar('\n'); }

static void set_color(uint8_t color) { terminal_setcolor(color); }

/* ========== Variable management ========== */
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

/* ========== Line parsing (scan for line numbers and labels) ========== */
static void parse_line_map(void)
{
    qb.line_count = 0;
    size_t pos = 0;
    
    while (pos < qb.code_len && qb.line_count < MAX_LINES) {
        /* Skip leading whitespace */
        while (pos < qb.code_len && (qb.code[pos] == ' ' || qb.code[pos] == '\t')) pos++;
        
        /* Record this position */
        size_t line_start = pos;
        
        /* Check if it starts with a number (line number) */
        int line_num = 0;
        if (pos < qb.code_len && qb.code[pos] >= '0' && qb.code[pos] <= '9') {
            while (pos < qb.code_len && qb.code[pos] >= '0' && qb.code[pos] <= '9') {
                line_num = line_num * 10 + (qb.code[pos] - '0');
                pos++;
            }
        } else {
            line_num = qb.line_count;  /* auto-assign */
        }
        
        qb.lines[qb.line_count].line_num = line_num;
        qb.lines[qb.line_count].code_pos = line_start;
        qb.line_count++;
        
        /* Skip to end of line */
        while (pos < qb.code_len && qb.code[pos] != '\n') pos++;
        if (pos < qb.code_len && qb.code[pos] == '\n') pos++;
    }
}

static size_t find_line_by_number(int line_num)
{
    for (int i = 0; i < qb.line_count; i++) {
        if (qb.lines[i].line_num == line_num) {
            return qb.lines[i].code_pos;
        }
    }
    return 0;
}

/* Find label in code */
static size_t find_label(const char *label_name)
{
    size_t pos = 0;
    while (pos < qb.code_len) {
        /* Skip whitespace and line numbers */
        while (pos < qb.code_len && (qb.code[pos] == ' ' || qb.code[pos] == '\t')) pos++;
        while (pos < qb.code_len && qb.code[pos] >= '0' && qb.code[pos] <= '9') pos++;
        while (pos < qb.code_len && (qb.code[pos] == ' ' || qb.code[pos] == '\t')) pos++;
        
        /* Check for label: (must be followed by ':') */
        if (pos < qb.code_len && qb.code[pos] >= 'A' && qb.code[pos] <= 'Z') {
            char potential_label[32];
            size_t lpos = 0;
            size_t check_pos = pos;
            while (check_pos < qb.code_len && lpos < 31 && 
                   ((qb.code[check_pos] >= 'A' && qb.code[check_pos] <= 'Z') ||
                    (qb.code[check_pos] >= '0' && qb.code[check_pos] <= '9'))) {
                potential_label[lpos++] = qb.code[check_pos++];
            }
            potential_label[lpos] = '\0';
            
            check_pos = pos + lpos;
            while (check_pos < qb.code_len && (qb.code[check_pos] == ' ' || qb.code[check_pos] == '\t')) check_pos++;
            
            if (check_pos < qb.code_len && qb.code[check_pos] == ':' && !strcmp(potential_label, label_name)) {
                return pos;
            }
        }
        
        /* Skip to next line */
        while (pos < qb.code_len && qb.code[pos] != '\n') pos++;
        if (pos < qb.code_len && qb.code[pos] == '\n') pos++;
    }
    return 0;
}

/* ========== Parsing utilities ========== */
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
    while (i < max - 1 && *line && *line != ' ' && *line != '\t' && *line != '=' && 
           *line != '"' && *line != ',' && *line != ':') {
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

/* ========== Statement execution ========== */
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
                if((var_name[0]>='0')&&(var_name[0]<='9')||((var_name[0]=='-')&&(var_name[1]>='0')&&(var_name[1]<='9'))) {
                    char buf[32];
                    int_to_str(parse_int(var_name), buf);
                    print_str(buf);
                } else {
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

        print_str("? ");
        char buf[128];
        read_input_line(buf, sizeof(buf));
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

/* ========== Control flow: GOTO, FOR, NEXT ========== */
static int execute_line(char *line);

static void execute_goto(char *target)
{
    target = trim_start(target);
    char label_or_num[32];
    extract_token(target, label_or_num, sizeof(label_or_num));
    
    /* Try as line number first */
    if (is_number(label_or_num)) {
        int line_num = parse_int(label_or_num);
        size_t pos = find_line_by_number(line_num);
        if (pos > 0) {
            qb.exec_pos = pos;
        }
    } else {
        /* Try as label */
        size_t pos = find_label(label_or_num);
        if (pos > 0) {
            qb.exec_pos = pos;
        }
    }
}

static void execute_for(char *args)
{
    args = trim_start(args);
    char var_name[32];
    args = extract_token(args, var_name, sizeof(var_name));
    args = trim_start(args);
    
    if (*args == '=') args++;
    args = trim_start(args);
    
    /* Parse initial value */
    char init_str[32];
    args = extract_token(args, init_str, sizeof(init_str));
    args = trim_start(args);
    int32_t init_val = parse_int(init_str);
    
    /* Expect TO */
    char to_kw[32];
    args = extract_token(args, to_kw, sizeof(to_kw));
    args = trim_start(args);
    
    /* Parse end value */
    char to_str[32];
    args = extract_token(args, to_str, sizeof(to_str));
    int32_t to_val = parse_int(to_str);
    
    /* Create/update loop variable */
    variable *v = find_var(var_name);
    if (!v) v = create_var(var_name, VAR_INT);
    v->val.int_val = init_val;
    
    /* Push FOR state - store position AFTER the FOR statement */
    if (qb.for_depth < MAX_FOR_STACK) {
        for_loop_state *fs = &qb.for_stack[qb.for_depth];
        strcpy(fs->var_name, var_name);
        fs->to_val = to_val;
        fs->loop_start_pos = qb.exec_pos;  /* Position to jump back to (first statement in loop) */
        fs->depth = qb.for_depth;
        qb.for_depth++;
    }
}

static void execute_next(char *var_name_arg)
{
    if (qb.for_depth == 0) return;
    
    for_loop_state *fs = &qb.for_stack[qb.for_depth - 1];
    variable *v = find_var(fs->var_name);
    
    if (v) {
        /* Increment the loop variable */
        v->val.int_val++;
        
        if (v->val.int_val <= fs->to_val) {
            /* Loop condition still true: jump back to start of loop body */
            qb.exec_pos = fs->loop_start_pos;
        } else {
            /* Loop is done: pop the FOR stack and continue past NEXT */
            qb.for_depth--;
        }
    }
}

/* ========== Main line executor ========== */
static int execute_line(char *line)
{
    line = trim_start(line);
    if (!*line || *line == '\'') return 1;  /* skip empty/comment lines */
    
    /* Skip line number if present */
    while (*line >= '0' && *line <= '9') line++;
    line = trim_start(line);
    
    /* Check for label (label:) */
    char *colon = line;
    while (*colon && *colon != ':' && *colon != '\n') colon++;
    if (*colon == ':' && (colon[1] == '\n' || colon[1] == '\0' || colon[1] == ' ')) {
        /* This is a label-only line, skip it */
        return 1;
    }
    
    /* Parse command */
    char cmd[32];
    line = extract_token(line, cmd, sizeof(cmd));
    line = trim_start(line);
    
    if (!strcmp(cmd, "PRINT")) {
        execute_print(line);
    } else if (!strcmp(cmd, "LET")) {
        execute_let(line);
    } else if (!strcmp(cmd, "INPUT")) {
        execute_input(line);
    } else if (!strcmp(cmd, "GOTO")) {
        execute_goto(line);
        return 0;  /* Signal to restart from exec_pos */
    } else if (!strcmp(cmd, "FOR")) {
        execute_for(line);
    } else if (!strcmp(cmd, "NEXT")) {
        execute_next(line);
        return 0;  /* Signal to jump or continue */
    } else if (!strcmp(cmd, "IF")) {
        char buf[256];
        strcpy(buf, line);
        char *then_pos = NULL;
        
        /* Find THEN */
        for (char *p = buf; *p; p++) {
            if (p[0] == 'T' && p[1] == 'H' && p[2] == 'E' && p[3] == 'N') {
                then_pos = p;
                break;
            }
        }
        
        if (then_pos) {
            *then_pos = '\0';
            char *action = trim_start(then_pos + 4);
            char *else_pos = NULL;
            for (char *p = action; *p; p++) {
                if (p[0] == 'E' && p[1] == 'L' && p[2] == 'S' && p[3] == 'E') {
                    else_pos = p;
                    break;
                }
            }
            
            if (eval_condition(buf)) {
                if (else_pos) {
                    *else_pos = '\0';
                    execute_line(action);
                } else {
                    execute_line(action);
                }
            } else {
                if (else_pos) {
                    execute_line(trim_start(else_pos + 4));
                }
            }
        }
    }
    
    return 1;  /* Normal progression */
}

/* ========== Program execution ========== */
static void run_program(void)
{
    qb.var_count = 0;
    qb.for_depth = 0;
    qb.exec_pos = 0;
    
    parse_line_map();
    
    while (qb.exec_pos < qb.code_len) {
        /* Extract current line */
        char line_buf[MAX_LINE_LEN];
        size_t pos = qb.exec_pos;
        size_t i = 0;
        
        while (pos < qb.code_len && qb.code[pos] != '\n' && i < MAX_LINE_LEN - 1) {
            line_buf[i++] = qb.code[pos++];
        }
        line_buf[i] = '\0';
        
        /* Calculate next line position before executing */
        size_t next_line_pos = pos;
        if (pos < qb.code_len && qb.code[pos] == '\n') next_line_pos = pos + 1;
        
        /* Check if this is a FOR statement - update loop_start_pos to next line */
        char test_cmd[32];
        char *test_line = trim_start(line_buf);
        while (*test_line >= '0' && *test_line <= '9') test_line++;
        test_line = trim_start(test_line);
        extract_token(test_line, test_cmd, sizeof(test_cmd));
        
        if (!strcmp(test_cmd, "FOR")) {
            /* Will be set by execute_for, but we need next line position */
            size_t saved_exec_pos = qb.exec_pos;
            qb.exec_pos = next_line_pos;  /* Temporarily move to next line */
            execute_line(line_buf);
            if (qb.for_depth > 0) {
                /* Update the for stack's loop_start_pos to the line after FOR */
                qb.for_stack[qb.for_depth - 1].loop_start_pos = next_line_pos;
            }
            continue;
        }
        
        int should_advance = execute_line(line_buf);
        if (should_advance) {
            qb.exec_pos = next_line_pos;
        }
    }
}

/* ========== Editor ========== */
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
        
        if (c == 0x12) {   /* Ctrl+R - RUN */
            qb.code_len = editor_pos;
            memcpy(qb.code, editor_buf, editor_pos);
            
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

        if (c == 0x18) {   /* Ctrl+X - EXIT */
            return;
        }
        
        if (c == '\b' || c == 0x7F) {
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

void qbasic_run(const char* code)
{
    if (code && *code) {
        size_t i = 0;
        while (i < MAX_CODE_LEN - 1 && code[i]) {
            editor_buf[i] = code[i];
            i++;
        }
        editor_pos = i;
        editor_buf[editor_pos] = '\0';

        qb.code_len = editor_pos;
        memcpy(qb.code, editor_buf, editor_pos);

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
        editor_loop();
    }
}