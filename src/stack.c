#include "common.h"

void stack_init(stack_t *s, size_t val_size)
{
    s->val_size = val_size;
    s->size = 0;
    if (s->cap == 0)
        s->cap = 4;
    s->data = malloc(s->cap * s->val_size);
}

void stack_push(stack_t *s, const void *val_ptr)
{
    if (s->size >= s->cap)
    {
        s->cap *= 2;
        s->data = realloc(s->data, s->cap * s->val_size);
    }

    void *dst = (char *)s->data + s->size * s->val_size;
    memcpy(dst, val_ptr, s->val_size);
    s->size++;
}

void stack_pop(stack_t *s, void *out_ptr)
{
    if (s->size == 0)
        return;
    s->size--;

    void *src = (char *)s->data + s->size * s->val_size;
    if (out_ptr != NULL)
        memcpy(out_ptr, src, s->val_size);
}

void stack_peek(stack_t *s, void *out_ptr)
{
    if (s->size == 0)
        return;

    void *src = (char *)s->data + (s->size - 1) * s->val_size;
    memcpy(out_ptr, src, s->val_size);
}

void stack_free(stack_t *s)
{
    free(s->data);
    s->data = NULL;
}
