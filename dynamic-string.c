#pragma once

#include <stdlib.h>

#include "error.c"
#include "string.c"
#include "arena.c"

typedef struct {
    const char *ptr;
    int len;
    int capacity;
} DynamicString;

bool ds_arena_clone(Arena *arena, DynamicString *ds, String *str) 
{
    ds->capacity = str->len;
    ds->len      = str->len;
    ds->ptr      = arena_alloc(arena, str->len * sizeof(char));
 
    if (ds->ptr == NULL) return error_false("Arena out of memory");

    memcpy((void *)ds->ptr, str->ptr, str->len);
 
    return true;
}

bool ds_arena_clone_cstr(Arena *arena, DynamicString *ds, const char *str)
{
    String tmp = String_from_cstr(str);
    if (!ds_arena_clone(arena, ds, &tmp)) return false;
    return true;
}
