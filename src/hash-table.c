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
} Node;

typedef struct {
    Node *arr;
    size_t len;
} HashTable;

void ht_init(HashTable *ht, size_t len)
{
    memset(ht->arr, 0, sizeof(Node) * len);
    ht->len = len;
}

HashTable ht_init_with_arena(Arena *arena, size_t size)
{
    HashTable ht;

    ht.arr = arena_alloc(arena, sizeof(Node) * size);
    ht_init(&ht, size);

    return ht;
}

bool ht_get(HashTable *ht, String *key, void *value) { }
bool ht_set(HashTable *ht, String key, void *value) { }

bool ht_get_with_cstr(HashTable *ht, const char *key, void *value) { }
bool ht_set_with_cstr(HashTable *ht, const char *key, void *value) { }

