#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parser.h"
#include "ast.h"
#include "lexer.h"

static bool debug = false;

static Token* advance(Parser* p) {
    p->current = lexer_next_token(p->lexer);
    return p->current;
}

static Token* expect(Parser* p, TokenType type) {
    if (p->current->type != type) {
        fprintf(stderr, "Syntax error at line %d: expected %d but found %d ('%s')\n",
                p->current->line, type, p->current->type, p->current->value);
        exit(EXIT_FAILURE);
    }
    Token* tok = p->current;
    advance(p);
    return tok;
}

Parser* parser_new(Lexer* lexer) {
    Parser* p = calloc(1, sizeof(Parser));
    p->lexer = lexer;
    p->current = lexer_next_token(lexer);
    return p;
}

static ASTNode* parse_statement(Parser* p);
static ASTNode* parse_expression(Parser* p);

ASTNode* parse_program(Parser* p) {
    if (debug) printf("DEBUG: parse_program entered\tType: %d\tValue: %s\n", p->current->type, p->current->value ? p->current->value : "<null>");

    ASTNode* prog = ast_new(AST_PROGRAM);
    prog->program.count = 0;
    prog->program.statements = NULL;

    while (p->current->type != TOKEN_EOF) {
        ASTNode* stmt = parse_statement(p);
        prog->program.statements = realloc(prog->program.statements,
                                           sizeof(ASTNode*) * (prog->program.count + 1));
        prog->program.statements[prog->program.count++] = stmt;
    }

    return prog;
}

static ASTNode* parse_statement(Parser* p) {
    if(debug) printf("DEBUG: token=%s type=%d\n", p->current->value, p->current->type);

    // var x = 5;
    if (p->current->type == TOKEN_VAR) {
        advance(p); // skip 'var'
        Token* name = expect(p, TOKEN_IDENT);
        expect(p, TOKEN_OP); // should probably check it's "="
        ASTNode* value = parse_expression(p);
        expect(p, TOKEN_SEMI);

        ASTNode* node = ast_new(AST_VAR_DECL);
        node->var_decl.name = strdup(name->value);
        node->var_decl.value = value;
        return node;
    }

    // echo("Hello");
    if (p->current->type == TOKEN_IDENT && strcmp(p->current->value, "echo") == 0) {
        advance(p); // skip 'echo'
        expect(p, TOKEN_LPAREN);
        ASTNode* arg = parse_expression(p);
        expect(p, TOKEN_RPAREN);
        expect(p, TOKEN_SEMI);

        ASTNode* node = ast_new(AST_ECHO_STMT);
        node->echo_stmt.expr = arg;
        return node;
    }

    fprintf(stderr, "Unexpected token '%s' at line %d\n", p->current->value, p->current->line);
    exit(EXIT_FAILURE);
}

static ASTNode* parse_expression(Parser* p) {
    // Literals
    if (p->current->type == TOKEN_NUMBER || p->current->type == TOKEN_STRING) {
        ASTNode* lit = ast_new(AST_LITERAL);
        lit->literal.value = strdup(p->current->value);
        advance(p);
        return lit;
    }

    // Identifiers
    if (p->current->type == TOKEN_IDENT) {
        ASTNode* id = ast_new(AST_IDENTIFIER);
        id->identifier.name = strdup(p->current->value);
        advance(p);
        return id;
    }

    fprintf(stderr, "Unexpected expression start '%s'\n", p->current->value);
    exit(EXIT_FAILURE);
}
