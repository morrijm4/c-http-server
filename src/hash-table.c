#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include "stack-arena.c"
#include "string.c"

typedef struct {
    uint64_t hash;
    String key;
    void *value;
    void *next;
} Node;

typedef struct {
    Node *arr;
    size_t len;
} HashTable;

static uint64_t hash_djb2(String str)
{
    uint64_t hash = 5381;

    for (uint32_t i = 0; i < str.len; ++i) {
	hash = ((hash << 5) + hash) + str.ptr[i]; // hash * 33 + c
    }

    return hash;
}

static Node *getBaseNode(HashTable ht, uint64_t hash)
{
    return &ht.arr[hash % ht.len];
}

void *ht_get(HashTable ht, String key)
{
    uint64_t hash = hash_djb2(key);

    for (Node *node = getBaseNode(ht, hash); node != NULL; node = node->next) {
	if (node->hash == hash) {
	    return node->value;
	}
    }

    return NULL;
}

bool ht_set(StackArena *sa, HashTable ht, String key, void *value)
{
    uint64_t hash = hash_djb2(key);
    Node *base    = getBaseNode(ht, hash);

    // New key at index
    if (str_is_null(base->key)) {
	base->hash  = hash;
	base->key   = key;
	base->value = value;
	base->next  = NULL;
	return true;
    }

    Node *node = base;
    while (node != NULL) {
	// Key already exist
	if (base->hash == hash) {
	    base->value = value;
	    return true;
	}

	node = node->next;
    }

    // Key does not exist insert after the base node

    if (!sa_alloc_struct(sa, Node, (void **)&node)) return false;

    node->hash  = hash;
    node->key   = key;
    node->value = value;
    node->next  = base->next;

    base->next = node;

    return true;
}

void ht_print_cstr(HashTable *ht)
{
    printf("\n");
    for (int i = 0; i < ht->len; ++i) {
	Node *node = &ht->arr[i];

	printf("[%d]: %p\n", i, node);
	while (node != NULL) {
	    if (!str_is_null(node->key)) {
		printf("  - Key:   %.*s\n", node->key.len, node->key.ptr);
		printf("    Hash:  %llu\n", node->hash);
		printf("    Value: %s\n", (const char *)node->value);
		printf("    Next:  %p\n", node->next);
	    }
	    node = node->next;
	}
    }
}

bool ht_init(StackArena *sa, HashTable *ht, size_t len)
{
    void *ptr;
    if (!sa_alloc_array_zero(sa, Node, len, &ptr)) {
	return false;
    }

    ht->len = len;
    ht->arr = ptr;

    return true;
}

