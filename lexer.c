#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

Lexer* lexer_new(const char* src){
  Lexer* l = malloc(sizeof(Lexer));
  l->src = src;
  l->pos = 0;
  l->line = 1;
  return l;
}

char lexer_peek(Lexer* l){
  return l->src[l->pos];
}

char lexer_next(Lexer* l){
  char c = l->src[l->pos++];
  if(c=='\n'){
    l->line++;
    l->col = 1;
  }else{
    l->col++;
  }
  return c;
}

int lexer_eof(Lexer* l){
  return l->src[l->pos] == '\0';
}

Token* make_token(TokenType type, const char* value, int line, int col) {
    Token* t = malloc(sizeof(Token));
    t->type = type;
    t->value = strdup(value);
    t->line = line;
    t->col = col;
    return t;
}

Token* lexer_next_token(Lexer* l) {
    while (!lexer_eof(l)) {
        char c = lexer_peek(l);

        // Skip whitespace
        if (isspace(c)) {
            lexer_next(l);
            continue;
        }

        // Skip comments
        if (c == '/' && l->src[l->pos + 1] == '/') {
            while (!lexer_eof(l) && lexer_next(l) != '\n');
            continue;
        }

        // Identifiers / Keywords
        if (isalpha(c) || c == '_') {
            size_t start = l->pos;
            while (isalnum(lexer_peek(l)) || lexer_peek(l) == '_') lexer_next(l);
            size_t len = l->pos - start;
            char* word = strndup(l->src + start, len);

            TokenType type = TOKEN_IDENT;
            if (strcmp(word, "var") == 0) type = TOKEN_VAR;
            else if (strcmp(word, "func") == 0) type = TOKEN_FUNC;
            else if (strcmp(word, "if") == 0) type = TOKEN_IF;
            else if (strcmp(word, "else") == 0) type = TOKEN_ELSE;
            else if (strcmp(word, "for") == 0) type = TOKEN_FOR;

            return make_token(type, word, l->line, l->col);
        }

        // Numbers
        if (isdigit(c)) {
            size_t start = l->pos;
            while (isdigit(lexer_peek(l))) lexer_next(l);
            size_t len = l->pos - start;
            return make_token(TOKEN_NUMBER, strndup(l->src + start, len), l->line, l->col);
        }

        // Strings
        if (c == '"') {
            lexer_next(l); // skip opening "
            size_t start = l->pos;
            while (!lexer_eof(l) && lexer_peek(l) != '"') lexer_next(l);
            size_t len = l->pos - start;
            char* str = strndup(l->src + start, len);
            lexer_next(l); // skip closing "
            return make_token(TOKEN_STRING, str, l->line, l->col);
        }

        // Single-character tokens
        switch (c) {
            case '=': lexer_next(l); return make_token(TOKEN_OP, "=", l->line, l->col);
            case '+': lexer_next(l); return make_token(TOKEN_OP, "+", l->line, l->col);
            case '-': lexer_next(l); return make_token(TOKEN_OP, "-", l->line, l->col);
            case '*': lexer_next(l); return make_token(TOKEN_OP, "*", l->line, l->col);
            case '/': lexer_next(l); return make_token(TOKEN_OP, "/", l->line, l->col);
            case '>': lexer_next(l); return make_token(TOKEN_OP, ">", l->line, l->col);
            case '<': lexer_next(l); return make_token(TOKEN_OP, "<", l->line, l->col);
            case '(': lexer_next(l); return make_token(TOKEN_LPAREN, "(", l->line, l->col);
            case ')': lexer_next(l); return make_token(TOKEN_RPAREN, ")", l->line, l->col);
            case '{': lexer_next(l); return make_token(TOKEN_LBRACE, "{", l->line, l->col);
            case '}': lexer_next(l); return make_token(TOKEN_RBRACE, "}", l->line, l->col);
            case ';': lexer_next(l); return make_token(TOKEN_SEMI, ";", l->line, l->col);
        }

        // If we reach here → unknown char
        char bad[2] = {c, '\0'};
        lexer_next(l);
        return make_token(TOKEN_UNKNOWN, bad, l->line, l->col);
    }

    return make_token(TOKEN_EOF, "EOF", l->line, l->col);
}

