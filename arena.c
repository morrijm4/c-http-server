#pragma once

#include <stdint.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "error.c"
#include "math.c"

#define ARENA_GROWTH_FACTOR 2

typedef struct {
  void *base;
  uint32_t offset;
  uint32_t capacity;
} Arena;

bool arena_init(Arena *arena, size_t size) {
  arena->base = malloc(size);

  if (arena->base == NULL) return perror_false("malloc");

  arena->offset = 0;
  arena->capacity = size;

  return true;
}

bool arena_free(Arena *arena) {
  if (arena->base == NULL) error_fail("Error: arena has already been freed");

  free(arena->base);
  arena->base = NULL;
  arena->offset = 0;
  arena->capacity = 0;

  return true;
}

bool arena_grow(Arena *arena, size_t new_size) {
    if (new_size < arena->capacity) return true;

    void *ptr = realloc(arena->base, new_size);
    if (ptr == NULL) perror_false("realloc");

    arena->base     = ptr;
    arena->capacity = new_size;
    return true;
}

/**
 * @return ptr to allocated size bytes. If out of memory it returns NULL.
 */
void *arena_alloc(Arena *arena, size_t size) {
    uint32_t new_offset;
    math_add_u32(arena->offset, size, &new_offset);

    if (new_offset > arena->capacity) {
	uint32_t new_capacity = arena->capacity;

	while (new_capacity < new_offset) {
	    math_multiply_u32(new_capacity, ARENA_GROWTH_FACTOR, &new_capacity);
	}

	if (!arena_grow(arena, new_capacity)) return NULL;
    }

    void *ptr = arena->base + arena->offset;
    arena->offset = new_offset;
 
    return ptr;
}

