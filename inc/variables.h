#ifndef _VARIABLES_H
#define _VARIABLES_H

// ==== std libs ====
#include <string.h>

// ==== enums ====

typedef enum
{
    VAR_INT,
    VAR_FLOAT,
    VAR_BOOL,
} var_type;

// ==== structs ====

typedef struct
{
    var_type type;
    char name[32];
    float value;
} var_t;

typedef struct
{
    var_t arr[64];
    int size;
} vstore_t;

// ==== functions ====

void vstore_set(vstore_t *vs, var_t *var);
void vstore_get(vstore_t *vs, var_t *var);

#endif