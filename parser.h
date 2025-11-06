#ifndef BASC_PARSER_H
#define BASC_PARSER_H

#include "lexer.h"
#include "ast.h"

typedef struct {
    Lexer* lexer;
    Token* current;
} Parser;

Parser* parser_new(Lexer* lexer);

ASTNode* parse_program(Parser* p);

#endif