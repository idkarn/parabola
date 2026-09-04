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

    float a = -1;

    if (val.code == INST_ID)
    {
        var_t v = {};
        strcpy(v.name, val.repr);
        vstore_get(&intr->vars, &v);
        a = v.value;
    }
    else if (val.code == INST_VALUE) {
        a = val.payload;
        if (strlen(val.repr) > 0)
            a = atoi(val.repr);
    }

    printf("%f\n", a);
    fflush(stdout);
}

void sum(interpreter_t* intr)
{
    inst_t ai;
    stack_pop(&intr->stack, &ai);

    inst_t bi;
    stack_pop(&intr->stack, &bi);

    float a, b;

    if (ai.code == INST_ID)
    {
        var_t av = {};
        strcpy(av.name, ai.repr);
        vstore_get(&intr->vars, &av);
        a = av.value;
    }
    else if (ai.code == INST_VALUE) {
        a = ai.payload;
        if (strlen(ai.repr) > 0)
            a = atoi(ai.repr);
    }

    if (bi.code == INST_ID)
    {
        var_t bv = {};
        strcpy(bv.name, bi.repr);
        vstore_get(&intr->vars, &bv);
        b = bv.value;
    }
    else if (bi.code == INST_VALUE) {
        b = bi.payload;
        if (strlen(bi.repr) > 0)
            b = atoi(bi.repr);
    }

    stack_push(&intr->stack, &(inst_t){.code = INST_VALUE, .payload = a + b});
}