#ifndef QBASIC_H
#define QBASIC_H

#include <stdint.h>
#include <stdbool.h>

// QBASIC Token Types
typedef enum {
    TOKEN_EOF,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_IDENTIFIER,
    TOKEN_KEYWORD,
    TOKEN_OPERATOR,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_COMMA,
    TOKEN_COLON,
} TokenType;

// QBASIC Keywords
typedef enum {
    KW_PRINT,
    KW_INPUT,
    KW_IF,
    KW_THEN,
    KW_ELSE,
    KW_FOR,
    KW_TO,
    KW_NEXT,
    KW_WHILE,
    KW_WEND,
    KW_DIM,
    KW_END,
} Keyword;

// Token Structure
typedef struct {
    TokenType type;
    Keyword keyword;
    char value[256];
    int32_t numValue;
} Token;

// Function Declarations
void qbasic_init(void);
void qbasic_run(const char* code);
Token qbasic_next_token(const char* code, int* pos);
bool qbasic_execute_line(const char* line);
void qbasic_print(const char* str);
void qbasic_cleanup(void);

#endif // QBASIC_H