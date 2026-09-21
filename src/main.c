#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include "variables.h"

#include <stdio.h>

void load_file(char *buf)
{
    FILE *f = fopen("test.pb", "rb");

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    while (fread(buf, 1, size, f))
        ;
    fclose(f);
}

int main()
{
    char code[512];
    load_file(code);
    printf("code=%s\n", code);

    token_t tokens[64] = {};
    size_t tokens_size = analyze_into(code, tokens);

    print_tokens(tokens, tokens_size);

    parser_t parser;
    parser_init(&parser, tokens, tokens_size);
    parse_prog(&parser);

    print_rpn(&parser);

    interpreter_t intr;
    intr_init(&intr, parser.rpn);
    parser_free(&parser);

    intr_evaluate(&intr);

    // print_vstore(&intr.vars);

    return 0;
}