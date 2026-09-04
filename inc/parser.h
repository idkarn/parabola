#ifndef _PARSER_H
#define _PARSER_H

// ==== std libs ====
#include <setjmp.h>
#include <stdio.h>

// ==== headers ====
#include "common.h"
#include "tokens.h"
#include "instructions.h"

// ==== structures ====

typedef struct
{
    stack_t *tokens;
    stack_t *rpn;
} parser_t;

// ==== functions ====

void parser_init(parser_t *p, token_t *tokens, size_t tokens_size);
void parser_free(parser_t *p);
void parse_prog(parser_t *p);

#endif