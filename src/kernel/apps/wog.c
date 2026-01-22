#include "../io/vga.h"
#include "../io/keyboard.h"
#include "../lib/string.h"
#include <stdbool.h>
#include <stdint.h>

#define MAX_VARS 128
#define MAX_LINE_LEN 256
#define MAX_CODE_LEN 8192
#define MAX_VAR_NAME 32
#define MAX_STR_VAL 256

typedef enum { VAR_INT, VAR_STR } var_type;

typedef struct {
    char name[MAX_VAR_NAME];
    var_type type;
    union {
        int32_t int_val;
        char str_val[MAX_STR_VAL];
    } val;
} variable;

typedef struct {
    variable vars[MAX_VARS];
    int var_count;
    bool error_flag;
    char error_msg[128];
} wog_state;

static wog_state wog;
static char editor_buf[MAX_CODE_LEN];
static size_t editor_pos = 0;

/* Terminal I/O */
static void print_str(const char *s) { terminal_writestring(s); }
static void print_char(char c) { terminal_putchar(c); }
static void print_nl(void) { terminal_putchar('\n'); }
static void set_color(uint8_t color) { terminal_setcolor(color); }

static void wog_error(const char *msg)
{
    wog.error_flag = true;
    strcpy(wog.error_msg, msg);
    set_color(VGA_COLOR_RED);
    print_str("WOG ERROR: ");
    print_str(msg);
    print_nl();
    set_color(VGA_COLOR_LIGHT_GREY);
}

static variable* find_var(const char *name)
{
    for (int i = 0; i < wog.var_count; i++) {
        if (!strcmp(wog.vars[i].name, name)) {
            return &wog.vars[i];
        }
    }
    return NULL;
}

static variable* create_var(const char *name, var_type type)
{
    if (wog.var_count >= MAX_VARS) {
        wog_error("Variable table full");
        return NULL;
    }
    variable *v = &wog.vars[wog.var_count++];
    strcpy(v->name, name);
    v->type = type;
    if (type == VAR_INT) {
        v->val.int_val = 0;
    } else {
        v->val.str_val[0] = '\0';
    }
    return v;
}

static int32_t parse_int(const char *s)
{
    int32_t val = 0;
    bool neg = false;
    if (!s || !*s) return 0;
    if (*s == '-') { neg = true; s++; }
    while (*s >= '0' && *s <= '9') {
        val = val * 10 + (*s - '0');
        s++;
    }
    return neg ? -val : val;
}

static void int_to_str(int32_t num, char *buf, size_t max)
{
    if (num == 0) { strcpy(buf, "0"); return; }
    bool neg = num < 0;
    if (neg) num = -num;
    char tmp[32];
    int i = 0;
    while (num > 0 && i < 31) {
        tmp[i++] = '0' + (num % 10);
        num /= 10;
    }
    if (neg) tmp[i++] = '-';
    tmp[i] = '\0';
    int j = 0;
    for (int k = i - 1; k >= 0 && j < (int)max - 1; k--) {
        buf[j++] = tmp[k];
    }
    buf[j] = '\0';
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
    while (i < max - 1 && *line && *line != ' ' && *line != '\t' && *line != ':') {
        token[i++] = *line++;
    }
    token[i] = '\0';
    return line;
}

static char* extract_string(char *line, char *str, size_t max)
{
    line = trim_start(line);
    if (*line == '"') {
        line++;
        size_t i = 0;
        while (*line && *line != '"' && i < max - 1) {
            str[i++] = *line++;
        }
        str[i] = '\0';
        if (*line == '"') line++;
        return line;
    }
    str[0] = '\0';
    return line;
}

static int32_t eval_value(const char *expr)
{
    expr = trim_start((char*)expr);
    
    /* VERILY VERILY evaluates to 1 */
    if (!strncmp(expr, "VERILY", 6)) {
        return 1;
    }
    
    /* Integer literal */
    if (*expr >= '0' && *expr <= '9') {
        return parse_int(expr);
    }
    
    /* Negative integer */
    if (*expr == '-' && *(expr + 1) >= '0' && *(expr + 1) <= '9') {
        return parse_int(expr);
    }
    
    /* Variable reference */
    variable *v = find_var(expr);
    if (v && v->type == VAR_INT) {
        return v->val.int_val;
    }
    
    return 0;
}

static bool eval_condition(const char *cond)
{
    cond = trim_start((char*)cond);
    char var_name[MAX_VAR_NAME];
    char *rest = extract_token((char*)cond, var_name, sizeof(var_name));
    rest = trim_start(rest);
    
    variable *v = find_var(var_name);
    if (!v || v->type != VAR_INT) {
        return false;
    }
    
    int32_t val = v->val.int_val;
    int32_t rhs = 0;
    
    if (*rest == '=' && *(rest + 1) != '=') {
        rest++;
        rhs = parse_int(trim_start(rest));
        return val == rhs;
    } else if (*rest == '<' && *(rest + 1) == '>') {
        rest += 2;
        rhs = parse_int(trim_start(rest));
        return val != rhs;
    } else if (*rest == '<' && *(rest + 1) == '=') {
        rest += 2;
        rhs = parse_int(trim_start(rest));
        return val <= rhs;
    } else if (*rest == '>' && *(rest + 1) == '=') {
        rest += 2;
        rhs = parse_int(trim_start(rest));
        return val >= rhs;
    } else if (*rest == '<') {
        rest++;
        rhs = parse_int(trim_start(rest));
        return val < rhs;
    } else if (*rest == '>') {
        rest++;
        rhs = parse_int(trim_start(rest));
        return val > rhs;
    }
    
    return false;
}

static void execute_behold(char *args)
{
    args = trim_start(args);
    if (wog.error_flag) return;
    
    if (*args == '"') {
        char str_val[MAX_STR_VAL];
        extract_string(args, str_val, sizeof(str_val));
        print_str(str_val);
    } else {
        char expr[256];
        extract_token(args, expr, sizeof(expr));
        
        variable *v = find_var(expr);
        if (v) {
            if (v->type == VAR_INT) {
                char buf[32];
                int_to_str(v->val.int_val, buf, sizeof(buf));
                print_str(buf);
            } else {
                print_str(v->val.str_val);
            }
        } else if (*expr >= '0' && *expr <= '9') {
            print_str(expr);
        } else if (*expr == '-' && *(expr + 1) >= '0' && *(expr + 1) <= '9') {
            print_str(expr);
        }
    }
    print_nl();
}

static void execute_thou_shalt(char *args)
{
    args = trim_start(args);
    if (wog.error_flag) return;
    
    char var_name[MAX_VAR_NAME];
    args = extract_token(args, var_name, sizeof(var_name));
    args = trim_start(args);
    
    /* Skip "AND" if present */
    char keyword[32];
    if (*args) {
        char *tmp = args;
        extract_token(tmp, keyword, sizeof(keyword));
        if (!strcmp(keyword, "AND")) {
            args = tmp;
            args = extract_token(args, keyword, sizeof(keyword));
            args = trim_start(args);
        }
    }
    
    int32_t value = eval_value(args);
    
    variable *v = find_var(var_name);
    if (!v) {
        v = create_var(var_name, VAR_INT);
        if (!v) return;
    }
    
    if (v->type == VAR_INT) {
        v->val.int_val = value;
    }
}

static void execute_let_there_be(char *args)
{
    args = trim_start(args);
    if (wog.error_flag) return;
    
    char var_name[MAX_VAR_NAME];
    args = extract_token(args, var_name, sizeof(var_name));
    args = trim_start(args);
    
    var_type vtype = VAR_INT;
    if (!strcmp(args, "STRING")) {
        vtype = VAR_STR;
    }
    
    variable *v = find_var(var_name);
    if (!v) {
        create_var(var_name, vtype);
    }
}

static void execute_if(char *args);

static void execute_statement(char *stmt)
{
    stmt = trim_start(stmt);
    if (!*stmt || wog.error_flag) {
        return;
    }
    
    char cmd[MAX_VAR_NAME];
    char tmp[MAX_LINE_LEN];
    strcpy(tmp, stmt);
    char *rest = extract_token(tmp, cmd, sizeof(cmd));
    rest = trim_start(rest);
    
    if (!strcmp(cmd, "BEHOLD")) {
        execute_behold(rest);
    } else if (!strcmp(cmd, "THOU")) {
        char next[MAX_VAR_NAME];
        rest = extract_token(rest, next, sizeof(next));
        if (!strcmp(next, "SHALT")) {
            execute_thou_shalt(rest);
        }
    } else if (!strcmp(cmd, "IF")) {
        execute_if(rest);
    } else if (!strcmp(cmd, "WOE")) {
        rest = extract_token(rest, cmd, sizeof(cmd));
        if (!strcmp(cmd, "UNTO")) {
            rest = trim_start(rest);
            char msg[128];
            extract_string(rest, msg, sizeof(msg));
            set_color(VGA_COLOR_RED);
            print_str("WOE UNTO: ");
            print_str(msg);
            print_nl();
            set_color(VGA_COLOR_LIGHT_GREY);
            wog.error_flag = true;
        }
    }
}

static void execute_if(char *args)
{
    args = trim_start(args);
    if (wog.error_flag) return;
    
    char cond_buf[256];
    strcpy(cond_buf, args);
    
    /* Find THEN keyword */
    char *then_pos = NULL;
    for (char *p = cond_buf; *p; p++) {
        if (p[0] == 'T' && p[1] == 'H' && p[2] == 'E' && p[3] == 'N' &&
            (p == cond_buf || *(p-1) == ' ') &&
            (*(p+4) == ' ' || *(p+4) == '\0')) {
            then_pos = p;
            break;
        }
    }
    
    if (!then_pos) {
        wog_error("IF missing THEN");
        return;
    }
    
    *then_pos = '\0';
    char *statement = trim_start(then_pos + 4);
    
    if (eval_condition(cond_buf)) {
        execute_statement(statement);
    }
}

static void execute_line(char *line)
{
    line = trim_start(line);
    
    if (!*line || *line == '\'' || wog.error_flag) {
        return;
    }
    
    execute_statement(line);
}

static void run_program(void)
{
    wog.var_count = 0;
    wog.error_flag = false;
    
    char line_buf[MAX_LINE_LEN];
    size_t pos = 0;
    bool in_program = false;
    
    while (pos < editor_pos && !wog.error_flag) {
        size_t i = 0;
        while (pos < editor_pos && editor_buf[pos] != '\n' && i < MAX_LINE_LEN - 1) {
            line_buf[i++] = editor_buf[pos++];
        }
        line_buf[i] = '\0';
        if (editor_buf[pos] == '\n') pos++;
        
        char *trimmed = trim_start(line_buf);
        
        /* Program boundary check */
        if (!in_program) {
            if (!strcmp(trimmed, "AND GOD SAID")) {
                in_program = true;
            }
            continue;
        }
        
        /* Program end */
        if (!strcmp(trimmed, "AND IT CAME TO PASS")) {
            break;
        }
        
        /* Execute statement */
        execute_line(line_buf);
    }
}

static void editor_display(void)
{
    terminal_initialize();
    set_color(VGA_COLOR_CYAN);
    print_str("=== WOG INTERPRETER (Ctrl+R: Run, Ctrl+X: Exit) ===");
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
        
        /* Ctrl+R: Run */
        if (c == 0x12) {
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
        
        /* Ctrl+X: Exit */
        if (c == 0x18) {
            return;
        }
        
        /* Backspace */
        if (c == '\b' || c == 0x7F) {
            if (editor_pos > 0) editor_pos--;
        } 
        /* Newline */
        else if (c == '\n' || c == '\r') {
            if (editor_pos < MAX_CODE_LEN - 1) {
                editor_buf[editor_pos++] = '\n';
            }
        } 
        /* Printable */
        else if (c >= 32 && c <= 126) {
            if (editor_pos < MAX_CODE_LEN - 1) {
                editor_buf[editor_pos++] = c;
            }
        }
        editor_buf[editor_pos] = '\0';
    }
}

void wog_run(void)
{
    /* Interactive editor - WOG always runs interactively */
    editor_loop();
}