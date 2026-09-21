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

    if (val.code == INST_ID)
    {
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
    }
    else
    {
        switch (val.type)
        {
        case VAR_RAW:
            // cast raw value to type of variable
            switch (v.type)
            {
            case VAR_INT:
                v.value = atoi(val.repr);
                break;
            case VAR_FLOAT:
                v.value = atof(val.repr);
                break;
            default:
                break;
            }
            break;
        case VAR_FLOAT:
            v.value = val.as_float;
            break;
        case VAR_INT:
            v.value = val.as_int;
            break;
        default:
            break;
        }
    }
    printf("[ASSGN] name=%s type=%d value=%f\n", v.name, v.type, v.value);

    vstore_set(&intr->vars, &v);
}

void print(interpreter_t *intr)
{
    inst_t val;
    stack_pop(&intr->stack, &val);

    float f;
    int i;
    var_type t;

    if (val.code == INST_ID)
    {
        var_t v = {};
        strcpy(v.name, val.repr);
        vstore_get(&intr->vars, &v);
        printf("[PRINT] found: name=%s v=%f\n", v.name, v.value);
        t = v.type;
        if (t == VAR_FLOAT)
            f = v.value;
        else if (t == VAR_INT)
            i = (int)v.value;
    }
    else if (val.code == INST_VALUE)
    {
        t = val.type;
        switch (val.type)
        {
        case VAR_INT:
            i = val.as_int;
            if (strlen(val.repr) > 0)
                i = atoi(val.repr);
            break;
        case VAR_RAW:
        case VAR_FLOAT:
            f = val.as_float;
            if (strlen(val.repr) > 0)
                f = atof(val.repr);
            break;
        default:
            break;
        }
    }

    switch (t)
    {
    case VAR_FLOAT:
        printf("%f\n", f);
        break;
    case VAR_INT:
        printf("%d\n", i);
        break;
    default:
        printf("[PRNT] unknown type\n", i);
        break;
    }

    fflush(stdout);
}

void sum(interpreter_t *intr)
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
    else if (ai.code == INST_VALUE)
    {
        switch (ai.type)
        {
        case VAR_INT:
            a = ai.as_int;
            if (strlen(ai.repr) > 0)
                a = atoi(ai.repr);
            break;
        case VAR_RAW:
        case VAR_FLOAT:
            a = ai.as_float;
            if (strlen(ai.repr) > 0)
                a = atof(ai.repr);
            break;
        default:
            break;
        }
    }

    if (bi.code == INST_ID)
    {
        var_t bv = {};
        strcpy(bv.name, bi.repr);
        vstore_get(&intr->vars, &bv);
        b = bv.value;
    }
    else if (bi.code == INST_VALUE)
    {
        switch (bi.type)
        {
        case VAR_INT:
            b = bi.as_int;
            if (strlen(bi.repr) > 0)
                b = atoi(bi.repr);
            break;
        case VAR_RAW:
        case VAR_FLOAT:
            b = bi.as_float;
            if (strlen(bi.repr) > 0)
                b = atof(bi.repr);
            break;
        default:
            break;
        }
    }

    inst_t res = {
        .code = INST_VALUE,
    };
    switch (ai.type)
    {
    case VAR_INT:
        res.type = VAR_INT,
        res.as_int = a + b;
        break;
    case VAR_FLOAT:
    default:
        res.type = VAR_FLOAT,
        res.as_float = a + b;
        break;
        break;
    }

    stack_push(&intr->stack, &res);
}

// TODO: parse value from repr on parsing

void diff(interpreter_t *intr)
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
    else if (ai.code == INST_VALUE)
    {
        switch (ai.type)
        {
        case VAR_INT:
            a = ai.as_int;
            if (strlen(ai.repr) > 0)
                a = atoi(ai.repr);
            break;
        case VAR_RAW:
        case VAR_FLOAT:
            a = ai.as_float;
            if (strlen(ai.repr) > 0)
                a = atof(ai.repr);
            break;
        default:
            break;
        }
    }

    if (bi.code == INST_ID)
    {
        var_t bv = {};
        strcpy(bv.name, bi.repr);
        vstore_get(&intr->vars, &bv);
        b = bv.value;
    }
    else if (bi.code == INST_VALUE)
    {
        switch (bi.type)
        {
        case VAR_INT:
            b = bi.as_int;
            if (strlen(bi.repr) > 0)
                b = atoi(bi.repr);
            break;
        case VAR_RAW:
        case VAR_FLOAT:
            b = bi.as_float;
            if (strlen(bi.repr) > 0)
                b = atof(bi.repr);
            break;
        default:
            break;
        }
    }

    inst_t res = {
        .code = INST_VALUE,
    };
    switch (bi.type)
    {
    case VAR_INT:
        res.type = VAR_INT,
        res.as_int = b - a;
        break;
    case VAR_FLOAT:
    default:
        res.type = VAR_FLOAT,
        res.as_float = b - a;
        break;
    }

    stack_push(&intr->stack, &res);
}