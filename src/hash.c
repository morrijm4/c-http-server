#pragma once

#include <stdint.h>

#include "string.c"

uint64_t hash_djb2(String str)
{
    uint64_t hash = 5381;

    for (uint32_t i = 0; i < str.len; ++i) {
	hash = ((hash << 5) + hash) + str.ptr[i]; // hash * 33 + c
    }

    return hash;
}

uint64_t hash_djb2_cstr(const char *str)
{
    uint64_t hash = 5381;
    char c;

    while ((c = *str++)) {
	hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }

    return hash;
}
