#ifndef BASC_CODEGEN_H
#define BASC_CODEGEN_H

#include "ast.h"

char* codegen(ASTNode* node);

void codegen_to_file(ASTNode* root, const char* filename);

#endif