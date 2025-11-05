#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

ASTNode* ast_new(ASTNodeType type){
    ASTNode* node = calloc(1, sizeof(ASTNode));
    node->type = type;
    return node;
}

void ast_free(ASTNode* node){
    if(!node) return;

    switch(node->type){

        case AST_PROGRAM:
            for(int i = 0; i < node->program.count ; i++){
                ast_free(node->program.statements[i]);
            }
            free(node->program.statements);
            break;

        case AST_VAR_DECL:
            free(node->var_decl.name);
            ast_free(node->var_decl.value);
        
        case AST_IF_STMT:
            ast_free(node->if_stmt.condition);
            for(int i = 0; i < node->if_stmt.then_count; i++){
                ast_free(node->if_stmt.then_body[i]);
            }
            for(int i = 0; i < node->if_stmt.else_count; i++){
                ast_free(node->if_stmt.else_body[i]);
            }

        case AST_ECHO_STMT:
            ast_free(node->echo_stmt.expr);

        case AST_LITERAL:
            free(node->literal.value);
            break;

        case AST_IDENTIFIER:
            free(node->identifier.name);
            break;
        
        default:
            break;
        
    }

    free(node);

}

void ast_print(ASTNode* node, int indent) {
    if (!node) return;
    for (int i = 0; i < indent; i++) printf("  ");

    switch (node->type) {
        case AST_PROGRAM:
            printf("Program:\n");
            for (int i = 0; i < node->program.count; i++)
                ast_print(node->program.statements[i], indent + 1);
            break;

        case AST_VAR_DECL:
            printf("VarDecl: %s =\n", node->var_decl.name);
            ast_print(node->var_decl.value, indent + 1);
            break;

        case AST_LITERAL:
            printf("Literal: %s\n", node->literal.value);
            break;

        case AST_IDENTIFIER:
            printf("Identifier: %s\n", node->identifier.name);
            break;

        case AST_ECHO_STMT:
            printf("Echo:\n");
            ast_print(node->echo_stmt.expr, indent + 1);
            break;

        case AST_BINARY_EXPR:
            printf("BinaryExpr: %s\n", node->binary_expr.op);
            ast_print(node->binary_expr.left, indent + 1);
            ast_print(node->binary_expr.right, indent + 1);
            break;

        case AST_IF_STMT:
            printf("IfStmt:\n");
            for (int i = 0; i < indent + 1; i++) printf("  ");
            printf("Condition:\n");
            ast_print(node->if_stmt.condition, indent + 2);
            for (int i = 0; i < indent + 1; i++) printf("  ");
            printf("Then:\n");
            for (int i = 0; i < node->if_stmt.then_count; i++)
                ast_print(node->if_stmt.then_body[i], indent + 2);
            if (node->if_stmt.else_count > 0) {
                for (int i = 0; i < indent + 1; i++) printf("  ");
                printf("Else:\n");
                for (int i = 0; i < node->if_stmt.else_count; i++)
                    ast_print(node->if_stmt.else_body[i], indent + 2);
            }
            break;

        default:
            printf("Unknown node type\n");
    }
}