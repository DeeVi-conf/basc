#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"

static char* strclone(const char* s){
    
    char* r = malloc(strlen(s) + 1);
    strcpy(r, s);
    
    return r;

}

static char* strappend(char* base, const char* add){
    if(!base){
        
        return strclone(add);
    
    }else{

        size_t newlen = strlen(base) + strlen(add);
        base = realloc(base, newlen);
        strcat(base, add);
        return base;

    }

}

char* codegen(ASTNode* node) {
    if (!node) return strclone("");

    char* out = NULL;
    char buf[512];

    switch (node->type) {
        case AST_PROGRAM:
            for (int i = 0; i < node->program.count; i++) {
                char* stmt = codegen(node->program.statements[i]);
                out = strappend(out, stmt);
                out = strappend(out, "\n");
                free(stmt);
            }
            break;

        case AST_VAR_DECL:
            snprintf(buf, sizeof(buf), "%s=%s", 
                     node->var_decl.name, 
                     codegen(node->var_decl.value));
            out = strclone(buf);
            break;

        case AST_LITERAL:
            out = strclone(node->literal.value);
            break;

        case AST_ECHO_STMT: {
            ASTNode* expr = node->echo_stmt.expr;

            if (expr->type == AST_LITERAL) {
                // echo("Hello")
                snprintf(buf, sizeof(buf), "echo \"%s\"", expr->literal.value);
                out = strclone(buf);
            }
            else if (expr->type == AST_IDENTIFIER) {
                // echo(x)
                snprintf(buf, sizeof(buf), "echo \"$%s\"", expr->identifier.name);
                out = strclone(buf);
                    }
            else {
                // echo(some + expression)
                char* inner = codegen(expr);
                snprintf(buf, sizeof(buf), "echo \"%s\"", inner);
                free(inner);
                out = strclone(buf);
            }
            break;
        }


        case AST_IDENTIFIER:
            out = strclone(node->identifier.name);
            break;

        case AST_BINARY_EXPR: {
            char* left = codegen(node->binary_expr.left);
            char* right = codegen(node->binary_expr.right);
            snprintf(buf, sizeof(buf), "$((%s %s %s))", 
                     left, node->binary_expr.op, right);
            out = strclone(buf);
            free(left);
            free(right);
            break;
        }

        default:
            out = strclone("# Unsupported node");
    }

    return out;
}

void codegen_to_file(ASTNode* root, const char* filename) {
    char* bash = codegen(root);
    FILE* f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "Could not open output file %s\n", filename);
        free(bash);
        return;
    }

    fprintf(f, "#!/usr/bin/env bash\n\n%s\n", bash);
    fclose(f);
    free(bash);
}