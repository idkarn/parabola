#ifndef _INSTRUCTIONS_H
#define _INSTRUCTIONS_H

// ==== headers ====
#include "variables.h"

// ==== enums ====

typedef enum
{
    SUM,         // 0
    DIFF,        // 1
    DIV,         // 2
    MULT,        // 3
    MOD,         // 4
    TRUEDIV,     // 5
    AND,         // 6
    OR,          // 7
    LESS,        // 8
    GREATER,     // 9
    EQUAL,       // 10
    NEQUAL,      // 11
    INST_ASSIGN, // 12
    INST_NOT,    // 13
    INST_VALUE,  // 14
    INST_ID,     // 15
    INST_TYPE,   // 16
    INST_PRINT,  // 17
    INST_INPUT,  // 18
    INST_WHILE,
    INST_GOTO,
} inst_code;

// ==== structs ====

typedef struct
{
    inst_code code;
    char repr[64];
    var_type type;
    union
    {
        float as_float;
        int as_int;
    };
} inst_t;

#endif