#include "interpreter.h"

#include <stdio.h>

void intr_init(interpreter_t *intr, stack_t *rpn)
{
    intr->pc = 0;
    intr->rpn = malloc(rpn->size * sizeof(inst_t));
    memcpy(intr->rpn, rpn->data, rpn->size * rpn->val_size);
    intr->size = rpn->size;
    vstore_init(&intr->vars);

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
        inst_t inst = intr->rpn[intr->pc++];

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

                printf("[INST_TYPE] trying to define a variable...\n");

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
        case DIFF:
            diff(intr);
            break;
        case INST_WHILE:
        {
            inst_t cond;

            stack_pop(&intr->stack, &cond);

            char is_true;

            if (cond.code == INST_ID)
            {
                var_t v = {};
                strcpy(v.name, cond.repr);
                vstore_get(&intr->vars, &v);
                is_true = v.value;
            }
            else
            {
                switch (cond.type)
                {
                case VAR_FLOAT:
                    is_true = cond.as_float;
                    printf("[WHILE] is_true=%d code=%d payload=%f, repr='%s'\n", is_true, cond.code, cond.as_float, cond.repr);
                    break;
                case VAR_INT:
                    is_true = cond.as_int; // todo: replace with truthy()
                    printf("[WHILE] is_true=%d code=%d payload=%d, repr='%s'\n", is_true, cond.code, cond.as_int, cond.repr);

                    break;
                default:
                    break;
                }
            }

            if (is_true == 0)
            {
                intr->pc = inst.as_int;
                printf("[WHILE] skip_to=%d\n", intr->pc);
            }
            break;
        }
        case INST_GOTO:
            intr->pc = inst.as_int;
            break;
        default:
            break;
        }
    }
}
