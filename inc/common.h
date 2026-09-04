#ifndef _COMMON_H
#define _COMMON_H

// ==== std libs ====
#include <stdlib.h>
#include <string.h>

// ==== structs ====

typedef struct
{
    void *data;
    size_t val_size;
    unsigned int size;
    unsigned int cap;
} stack_t;

// ==== functions ====

void stack_init(stack_t *s, size_t val_size);
void stack_push(stack_t *s, const void *val_ptr);
void stack_pop(stack_t *s, void *out_ptr);
void stack_peek(stack_t *s, void *out_ptr);
void stack_free(stack_t *s);

#endif