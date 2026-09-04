#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include "variables.h"

#include <stdio.h>

int main()
{
    char *code = "{\n"
                 "  int a;\n"
                 "  a := 123;\n"
                 "  print(a);\n"
                 "}";

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