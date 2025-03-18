#pragma once

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "error.c"
#include "math.c"

#ifndef ARENA_SIZE
#define ARENA_SIZE 65536 - sizeof(uint32_t) // 64 KB - sizeof(uint32_t)
#endif

typedef struct {
    char base[ARENA_SIZE];
    uint32_t offset;
} StackArena;

bool sa_alloc(StackArena *sa, size_t size, void **dst)
{
    uint32_t new_offset;

    if (!math_add_u32(sa->offset, size, &new_offset)) {
	return false;
    }

    if (new_offset > ARENA_SIZE) {
	return error_false("Arena is out of memory");
    }

    *dst = (void *)&sa->base[sa->offset];
    sa->offset = new_offset;

    return true;
}

bool sa_alloc_zero(StackArena *sa, size_t size, void **dst)
{
    if (!sa_alloc(sa, size, dst)) return false;
    memset(*dst, 0, size);
    return true;
}

void sa_reset(StackArena *sa)
{
    sa->offset = 0;
}

#define sa_alloc_array(sa, type, count, dst) sa_alloc(sa, sizeof(type) * count, dst)
#define sa_alloc_struct(sa, type, dst) sa_alloc_array(sa, type, 1, dst)

#define sa_alloc_array_zero(sa, type, count, dst) sa_alloc_zero(sa, sizeof(type) * count, dst)
#define sa_alloc_struct_zero(sa, type, dst) sa_alloc_array_zero(sa, type, 1, dst)

#define sa_init() { .offset = 0 }
