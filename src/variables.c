#include "variables.h"
#include <stdio.h>

void vstore_init(vstore_t* vs)
{
    //vs->arr;
    vs->size = 0;
}

void vstore_set(vstore_t *vs, var_t *var)
{
    printf("[vstore_set] looking for var\n");
    for (int i = 0; i < vs->size; i++)
    {
        if (strcmp(vs->arr[i].name, var->name) == 0)
        {
            printf("[vstore_set] found one\n");
            vs->arr[i].value = var->value;
            return;
        }
    }
    printf("[vstore_set] nvar branch\n");
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

void print_vstore(vstore_t *vs)
{
    printf("VSTORE (size=%d):\n", vs->size);
    for (int j = 0; j < vs->size; j++)
    {
        printf("'%s'=%f (%d)\n", vs->arr[j].name, vs->arr[j].value, vs->arr[j].type);
    }
    printf("END\n");
}