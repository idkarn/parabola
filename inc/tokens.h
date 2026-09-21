#ifndef _TOKENS_H
#define _TOKENS_H

// ==== headers ====

#include "variables.h"

// ==== enums ====

typedef enum
{
    NONE,          // 0
    EOF_TOKEN,     // 1
    SEMICOLON,     // 2
    BRACE_OPEN,    // 3
    BRACE_CLOSING, // 4
    PAR_OPEN,      // 5
    PAR_CLOSING,   // 6
    VALUE,         // 7
    ID,            // 8
    TYPE,          // 9
    COMMA,         // 10
    ASSIGN,        // 11
    PRINT,         // 12
    INPUT,         // 13
    EXPR_OPR,      // 14
    WHILE,
} token_kind;

// ==== structs ====

typedef struct
{
    token_kind kind;
    char raw_value[64];
    float as_num;
} token_t;

#endif