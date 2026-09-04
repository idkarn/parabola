#include "variables.h"
#include <stdio.h>

void vstore_set(vstore_t *vs, var_t *var)
{
    for (int i = 0; i < vs->size; i++)
    {
        if (strcmp(vs->arr[i].name, var->name) == 0)
        {
            vs->arr[i].value = var->value;
            return;
        }
    }
    var_t *nvar = &vs->arr[vs->size++];
    strcpy(nvar->name, var->name);
    nvar->value = var->value;
    nvar->type = var->type;
}

void vstore_get(vstore_t *vs, var_t *var)
{
    for (int i = 0; i < vs->size; i++)
    {
        if (strcmp(vs->arr[i].name, var->name) == 0)
        {
            var->value = vs->arr[i].value;
            var->type = vs->arr[i].type;
            return;
        }
    }
}