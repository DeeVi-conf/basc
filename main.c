#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "ast.h"
#include "parser.h"

#define ANSI_RED "\x1b[31m"
#define ANSI_GREEN "\x1b[32m"
#define ANSI_YELLOW "\x1b[33m"
#define ANSI_BLUE "\x1b[34m"
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_CYAN "\x1b[35m"
#define ANSI_RESET "\x1b[0m"

static char* read_file(const char* filename);

int main(int argc, char* argv[]) {
    if (argc == 1) {
        fprintf(stderr, ANSI_RED "No input files.\n" ANSI_RESET);
        fprintf(stderr, "Use '%s --help' for usage.\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            printf("Basc alpha 0.0.1\n");
            continue;
        }

        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            printf("Usage:\n  %s [-h|--help] [-v|--version] [input.basc]\n", argv[0]);
            continue;
        }

        char* code = read_file(argv[i]);
        if (!code) {
            fprintf(stderr, ANSI_RED "Error: could not open '%s'\n" ANSI_RESET, argv[i]);
            continue;
        }

        Lexer* lexer = lexer_new(code);

/*
        while (1) {

            Token* t = lexer_next_token(lexer);
            
            printf("%d\t%-10s\t%s\n", t->line, (char*[]){
                "VAR", "FUNC", "IF", "ELSE", "FOR", "IDENT", "NUMBER", "STRING",
                "OP", "LPAREN", "RPAREN", "LBRACE", "RBRACE", "SEMI", "EOF", "UNKNOWN"
            }[t->type], t->value);

            if (t->type == TOKEN_EOF) {
                free(t->value);
                free(t);
                break;
            }

            free(t->value);
            free(t);
        }
*/
        Parser* parser = parser_new(lexer);
        ASTNode* root = parse_program(parser);
        ast_print(root, 0);

        free(code);
        free(lexer);
        free(root);
    }

    return EXIT_SUCCESS;
}

static char* read_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char* buffer = malloc(size + 1);
    if (!buffer) {
        fclose(file);
        return NULL;
    }

    size_t read_size = fread(buffer, 1, size, file);
    buffer[read_size] = '\0';
    fclose(file);
    return buffer;
}
