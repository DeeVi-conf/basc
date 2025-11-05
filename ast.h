#ifndef BASC_AST_H
#define BASC_AST_H

#include "lexer.h"

typedef enum {
    AST_PROGRAM,
    AST_VAR_DECL,
    AST_FUNC_DECL,  // Not yet implemented
    AST_IF_STMT,
    AST_FOR_STMT,   // Not yet implemented
    AST_ECHO_STMT,
    AST_BINARY_EXPR,
    AST_LITERAL,
    AST_IDENTIFIER
} ASTNodeType;

typedef struct ASTNode ASTNode;

struct ASTNode{
    ASTNodeType type;
    union{

        // Variable declaration
        struct{
            char* name;
            ASTNode* value;
        }var_decl;

        // If statement
        struct{
            ASTNode* condition;
            ASTNode** then_body;
            int then_count;
            ASTNode** else_body;
            int else_count;
        }if_stmt;

        // Binary expression
        struct{
            char* op;
            ASTNode* left;
            ASTNode* right;
        }binary_expr;

        // Literal 
        struct{
            char* value;
        }literal;

        // Identifier
        struct{
            char* name;
        }identifier;

        // Echo
        struct{
            ASTNode* expr;
        }echo_stmt;

        // Root Program Node
        struct{
            ASTNode** statements;
            int count;
        }program;
    };
};

ASTNode* ast_new(ASTNodeType type);
void ast_free(ASTNode* node);
void ast_print(ASTNode* node, int indent);


#endif