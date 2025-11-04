#ifndef BASC_LEXER_H
#define BASC_LEXER_H

#include <stddef.h>

typedef enum {
    TOKEN_VAR,
    TOKEN_FUNC,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_FOR,
    TOKEN_IDENT,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_OP,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_SEMI,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char* value;
    int line;
    int col;
} Token;

typedef struct {
    const char* src;
    size_t pos;
    int line;
    int col;
} Lexer;

Lexer* lexer_new(const char* src);
Token* lexer_next_token(Lexer* l);

#endif
