#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack-arena.c"

#define str_null str_from(NULL, 0)

typedef struct {
    const char *ptr;
    int len;
} String;

String str_from(const char *ptr, int len) 
{
    String str;
 
    str.ptr = ptr;
    str.len = len;
 
    return str;
}

String str_from_cstr(const char *ptr) {
  return str_from(ptr, strlen(ptr));
}

bool str_eq(String a, String b) 
{
    if (a.len != b.len) {
	return false;
    } else {
        return memcmp(a.ptr, b.ptr, a.len) == 0;
    }
}

bool str_is_null(String str)
{
    return str_eq(str, str_null);
}

bool str_eq_cstr(String a, const char *b) 
{
    return str_eq(a, str_from_cstr(b));
}

bool str_try_chop_by_delim(String *str, char delim, String *result) 
{
    int i = 0;
    while (i < str->len && str->ptr[i] != delim) {
	i += 1;
    }

    if (i == str->len) {
	return false;
    }

    if (result != NULL) {
	*result = str_from(str->ptr, i);
    }

    str->len -= i + 1;
    str->ptr += i + 1;

    return true;
}

void str_println(String str) 
{
    for (int i = 0; i < str.len; ++i) {
	putchar(str.ptr[i]);
    }
    putchar('\n');
}

bool str_clone_with_stack_arena(StackArena *sa, String *dst, String *src)
{
    dst->len = src->len;
    if (!sa_alloc(sa, src->len * sizeof(char), (void **)&dst->ptr)) return false;

    memcpy((void *)dst->ptr, src->ptr, src->len);

    return true;
}

bool str_clone_cstr_with_stack_arena(StackArena *sa, String *dst, const char *src)
{
    String tmp = str_from_cstr(src);
    if (!str_clone_with_stack_arena(sa, dst, &tmp)) return false;
    return true;
}
