#ifndef _TOKENS_H
#define _TOKENS_H

// ==== headers ====

#include "variables.h"

// ==== enums ====

typedef enum
{
    NONE,
    EOF_TOKEN,
    SEMICOLON,
    BRACE_OPEN,
    BRACE_CLOSING,
    PAR_OPEN,
    PAR_CLOSING,
    VALUE,
    ID,
    TYPE,
    COMMA,
    ASSIGN,
    PRINT,
    INPUT,
} token_kind;

// ==== structs ====

typedef struct
{
    token_kind kind;
    char raw_value[64];
    float as_num;
} token_t;

#endif