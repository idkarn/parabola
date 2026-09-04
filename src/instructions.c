#include "interpreter.h"

#include <stdio.h>

void assign(interpreter_t *intr)
{
    inst_t id;
    stack_pop(&intr->stack, &id);

    inst_t val;
    stack_pop(&intr->stack, &val);

    // get variable data (for type) from vstore
    var_t v = {};
    strcpy(v.name, id.repr);
    vstore_get(&intr->vars, &v);

    switch (v.type)
    { // todo: fill other
    case VAR_FLOAT:
        v.value = atof(val.repr);
        break;
    case VAR_INT:
        v.value = atoi(val.repr);
        break;
    case VAR_BOOL:
    default:
        break;
    }

    vstore_set(&intr->vars, &v);
}

void print(interpreter_t *intr)
{
    inst_t val;
    stack_pop(&intr->stack, &val);

    var_t v = {};
    strcpy(v.name, val.repr);
    vstore_get(&intr->vars, &v);

    printf("%f\n", v.value);
}