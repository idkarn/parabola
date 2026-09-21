#include "parser.h"

#include <stdio.h>

static jmp_buf err_buf;
static void parse_scope(parser_t *p);

void parser_init(parser_t *p, token_t *tokens, size_t tokens_size)
{
    // refactor: WTF? replace it with values instead of ptrs
    p->rpn = malloc(sizeof(stack_t));
    stack_init(p->rpn, sizeof(inst_t));

    p->tokens = malloc(sizeof(stack_t));
    p->tokens->cap = tokens_size + 1;
    stack_init(p->tokens, sizeof(token_t));

    for (int i = tokens_size - 1; i >= 0; i--)
        stack_push(p->tokens, &tokens[i]);
}

void parser_free(parser_t *p)
{
    free(p->rpn);
    p->rpn = NULL;

    free(p->tokens);
    p->tokens = NULL;
}

void parse_token(parser_t *p)
{
    token_t tok;
    stack_peek(p->tokens, &tok);

    if (tok.kind == VALUE)
    {
        stack_pop(p->tokens, &tok);

        inst_t i = {
            .code = INST_VALUE,
            .type = VAR_RAW,
        };

        strcpy(i.repr, tok.raw_value);
        stack_push(p->rpn, &i);
    }
    else if (tok.kind == ID)
    {
        stack_pop(p->tokens, &tok);
        inst_t i = {.code = INST_ID};
        strcpy(i.repr, tok.raw_value);
        stack_push(p->rpn, &i);
    }
    else if (strcmp(tok.raw_value, "not") == 0)
    {
        stack_pop(p->tokens, &tok);
        parse_token(p);
        stack_push(p->rpn, &(inst_t){.code = INST_NOT});
    }
}

// parse High Priority operations
void parse_operation_hp(parser_t *p)
{
    parse_token(p);

    token_t tok;
    stack_peek(p->tokens, &tok);

    inst_code code;
    do
    {
        if (!strcmp(tok.raw_value, "*"))
            code = MULT;
        else if (!strcmp(tok.raw_value, "/"))
            code = DIV;
        else if (!strcmp(tok.raw_value, "%"))
            code = MOD;
        else if (!strcmp(tok.raw_value, "//"))
            code = TRUEDIV;
        else if (!strcmp(tok.raw_value, "and"))
            code = AND;
        else
            break;

        stack_pop(p->tokens, &tok);
        parse_token(p);
        stack_push(p->rpn, &(inst_t){.code = code});
        stack_peek(p->tokens, &tok);
    } while (1);
}

void parse_operation(parser_t *p)
{
    parse_operation_hp(p);

    token_t tok;
    stack_peek(p->tokens, &tok);

    inst_code code;
    do
    {
        if (!strcmp(tok.raw_value, "+"))
            code = SUM;
        else if (!strcmp(tok.raw_value, "-"))
            code = DIFF;
        else if (!strcmp(tok.raw_value, "or"))
            code = OR;
        else
            break;

        stack_pop(p->tokens, &tok);
        parse_operation_hp(p);
        stack_push(p->rpn, &(inst_t){.code = code});
        stack_peek(p->tokens, &tok);
    } while (1);
}

void parse_comparison(parser_t *p)
{
    parse_operation(p);

    token_t tok;
    stack_peek(p->tokens, &tok);

    int code = -1; // note: enum holds on int
    if (!strcmp(tok.raw_value, ">"))
        code = GREATER;
    else if (!strcmp(tok.raw_value, "<"))
        code = LESS;
    else if (!strcmp(tok.raw_value, "="))
        code = EQUAL;
    else if (!strcmp(tok.raw_value, "!="))
        code = NEQUAL;
    if (code != -1)
        stack_pop(p->tokens, &tok);

    parse_operation(p);

    if (code != -1)
        stack_push(p->rpn, &(inst_t){.code = code});
}

void parse_statement(parser_t *p)
{
    token_t tok;
    stack_peek(p->tokens, &tok);

    if (tok.kind == TYPE)
    {
        token_t op;
        stack_pop(p->tokens, &op);

        token_t next_op;
        stack_peek(p->tokens, &next_op);
        while (p->tokens->size && next_op.kind == ID)
        {
            stack_pop(p->tokens, &next_op);
            inst_t i = {.code = INST_ID};
            strcpy(i.repr, next_op.raw_value);
            stack_push(p->rpn, &i);
            stack_peek(p->tokens, &next_op);
            if (next_op.kind == COMMA)
                stack_pop(p->tokens, &next_op);

            stack_peek(p->tokens, &next_op); // for next iteration
        }

        inst_t i = {.code = INST_TYPE};
        strcpy(i.repr, tok.raw_value);
        stack_push(p->rpn, &i);
    }
    else if (tok.kind == ID)
    {
        token_t name;
        stack_pop(p->tokens, &name);

        token_t op;
        stack_peek(p->tokens, &op);

        if (op.kind != ASSIGN)
        {
            printf("Syntax Error: assignment expected\n");
            longjmp(err_buf, 1); // Syntax Error: assignment expected
        }
        stack_pop(p->tokens, &op);

        parse_comparison(p); // possible Syntax Error: closing parenthesis expected

        inst_t i = {.code = INST_ID};
        strcpy(i.repr, name.raw_value);
        stack_push(p->rpn, &i);
        stack_push(p->rpn, &(inst_t){.code = INST_ASSIGN});
    }
    else if (tok.kind == PRINT)
    {
        stack_pop(p->tokens, &tok);

        token_t par;
        stack_peek(p->tokens, &par);
        if (par.kind != PAR_OPEN)
        {
            printf("Syntax Error: parenthesis expected after func\n");
            longjmp(err_buf, 1);
        }
        stack_pop(p->tokens, &par);

        parse_comparison(p);

        stack_peek(p->tokens, &par);
        if (par.kind != PAR_CLOSING)
        {
            printf("Syntax Error: closing parenthesis expected\n");
            longjmp(err_buf, 1);
        }
        stack_pop(p->tokens, &par);

        stack_push(p->rpn, &(inst_t){.code = INST_PRINT});
    }
    else if (tok.kind == WHILE)
    {
        unsigned int start_pc = p->rpn->size;

        // * GENERAL CONDITION PARSING
        stack_pop(p->tokens, NULL); // throw away token

        token_t par;
        stack_peek(p->tokens, &par);
        if (par.kind != PAR_OPEN)
        {
            printf("Syntax Error: parenthesis expected in condition\n");
            longjmp(err_buf, 1);
        }
        stack_pop(p->tokens, NULL); // throw away token

        parse_comparison(p);

        stack_peek(p->tokens, &par);
        if (par.kind != PAR_CLOSING)
        {
            printf("Syntax Error: closing parenthesis expected in condition\n");
            longjmp(err_buf, 1);
        }
        stack_pop(p->tokens, NULL);
        // * END

        inst_t loop = {.code = INST_WHILE};
        unsigned int a = p->rpn->size;
        stack_push(p->rpn, &loop);

        parse_scope(p);

        ((inst_t *)p->rpn->data)[a].as_int = p->rpn->size + 1;

        stack_push(p->rpn, &(inst_t){.code = INST_GOTO, .as_int = start_pc});
    }
}

void parse_scope(parser_t *p)
{
    token_t tok;
    stack_pop(p->tokens, &tok);

    if (tok.kind != BRACE_OPEN)
    {
        printf("Syntax Error: program must start with brace\n");
        longjmp(err_buf, 1); // Syntax Error: program must start with brace
    }

    while (p->tokens->size)
    {
        parse_statement(p);

        stack_peek(p->tokens, &tok);

        if (tok.kind == BRACE_CLOSING)
        {
            stack_pop(p->tokens, &tok); // throw away
            break;
        }
        else if (tok.kind != SEMICOLON)
        {
            printf("Syntax Error: missed semicolon\n");
            longjmp(err_buf, 1); // Syntax Error: missed semicolon
        }

        stack_pop(p->tokens, &tok); // consume current one
    }
}

void parse_prog(parser_t *p)
{
    // kinda try-catch thing

    if (setjmp(err_buf) == 0)
        parse_scope(p);
    else
        fprintf(stderr, "Syntax Error: EOF is not found"); // refactor: replace with internal err field
}

void print_rpn(parser_t *p)
{
    printf("RPN (size=%d):\n", p->rpn->size);
    for (int j = 0; j < p->rpn->size; j++)
    {
        token_t *t = (char *)p->rpn->data + j * p->rpn->val_size;
        printf("%d,", t->kind);
    }
    printf("\nEND\n");
}