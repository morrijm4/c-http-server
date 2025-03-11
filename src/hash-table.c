#pragma once

#include <stdbool.h>
#include <string.h>
#include <stddef.h>

#include "arena.c"
#include "hash.c"
#include "string.c"

typedef struct {
    String key;
    void *value;
    void *next;
} HashNode;

typedef struct {
    HashNode *arr;
    size_t len;
} HashTable;

HashTable ht_init(Arena *arena, size_t len)
{
    HashTable ht;

    ht.arr = arena_alloc(arena, sizeof(HashNode) * len);
    memset(ht.arr, 0, sizeof(HashNode) * len);
    ht.len = len;

    return ht;
}

bool ht_get(HashTable *ht, String *key, void *value) { }
bool ht_set(HashTable *ht, String key, void *value) { }

bool ht_get_with_cstr(HashTable *ht, const char *key, void *value) { }
bool ht_set_with_cstr(HashTable *ht, const char *key, void *value) { }

