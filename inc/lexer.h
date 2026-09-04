#ifndef _LEXER_H
#define _LEXER_H

// ==== std libs ====
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// ==== headers ====
#include "tokens.h"

// ==== enums ====

typedef enum
{
    WAITING,
    TERMINAL,
    SPECIAL1,
    SPECIAL2
} lexer_state_t;

// ==== structures ====

typedef struct
{
    lexer_state_t state;
} lexer_t;

// ==== functions ====

size_t analyze_into(char *code, token_t *tokens);

#endif