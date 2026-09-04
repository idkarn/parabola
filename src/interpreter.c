#include "interpreter.h"

#include <stdio.h>

void intr_init(interpreter_t *intr, stack_t *rpn)
{
    intr->rpn = malloc(rpn->size * sizeof(inst_t));
    memcpy(intr->rpn, rpn->data, rpn->size * rpn->val_size);
    intr->size = rpn->size;

    stack_init(&intr->stack, sizeof(inst_t));
}

void intr_free(interpreter_t *intr)
{
    free(intr->rpn);
    intr->rpn = NULL;
    stack_free(&intr->stack);
}

void intr_evaluate(interpreter_t *intr)
{
    while (intr->pc < intr->size)
    {
        inst_t inst = intr->rpn[intr->pc];
        intr->pc++;

        switch (inst.code)
        {
        case INST_TYPE:
            while (intr->stack.size > 0)
            {
                inst_t currvar;
                stack_pop(&intr->stack, &currvar);

                // * DEFINE VARIABLE FROM STACK *
                var_t v = {
                    .value = 0,
                };
                strcpy(v.name, currvar.repr);
                if (strcmp(inst.repr, "int") == 0)
                    v.type = VAR_INT;
                else if (strcmp(inst.repr, "float") == 0)
                    v.type = VAR_FLOAT;
                else if (strcmp(inst.repr, "bool") == 0)
                    v.type = VAR_BOOL;

                vstore_set(&intr->vars, &v);
            }
            break;
        case INST_ID:
        case INST_VALUE:
            stack_push(&intr->stack, &inst);
            break;
        case INST_ASSIGN:
            assign(intr);
            break;
        case INST_PRINT:
            print(intr);
            break;
        case SUM:
            sum(intr);
            break;
        default:
            break;
        }
    }
}
