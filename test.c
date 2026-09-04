#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include "variables.h"

#include <stdio.h>


int main()
{
    /*char *code = "{\n"
                 "  int a;\n"
                 "  a := 123;\n"
                 "  print(a + 47);\n"
                 "}";*/

    char code[512];
    FILE *f = fopen("test.pb", "rb");

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    while (fread(code, 1, size, f));
    fclose(f);
    printf("code=%s\n", code);

    token_t tokens[64] = {};
    size_t tokens_size = analyze_into(code, tokens);

    parser_t parser;
    parser_init(&parser, tokens, tokens_size);
    parse_prog(&parser);

    interpreter_t intr;
    intr_init(&intr, parser.rpn);
    parser_free(&parser);

    intr_evaluate(&intr);

    return 0;
}