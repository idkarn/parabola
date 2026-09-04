#ifndef _INTERPRETER_H
#define _INTERPRETER_H

// ==== headers ====
#include "common.h"
#include "instructions.h"
#include "tokens.h"
#include "variables.h"

// ==== structs ====

typedef struct
{
    inst_t *rpn;
    unsigned int size;
    int pc;
    stack_t stack;
    vstore_t vars;
} interpreter_t;

// ==== structs ====

void intr_init(interpreter_t *intr, stack_t *rpn);
void intr_free(interpreter_t *intr);
void intr_evaluate(interpreter_t *intr);

void assign(interpreter_t *intr);
void print(interpreter_t *intr);

#endif