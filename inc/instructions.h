#ifndef _INSTRUCTIONS_H
#define _INSTRUCTIONS_H

// ==== enums ====

typedef enum
{
    SUM,
    DIFF,
    DIV,
    MULT,
    MOD,
    TRUEDIV,
    AND,
    OR,
    LESS,
    GREATER,
    EQUAL,
    NEQUAL,
    INST_ASSIGN,
    INST_NOT,
    INST_VALUE,
    INST_ID,
    INST_TYPE,
    INST_PRINT,
    INST_INPUT,
} inst_code;

// ==== structs ====

typedef struct
{
    inst_code code;
    char repr[64];
    float payload;
} inst_t;

#endif