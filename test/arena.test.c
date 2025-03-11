#pragma once

#include <assert.h>

#include "utils.c"
#include "../src/arena.c"

void arena_can_be_reset()
{
    size_t capacity = 1024;

    Arena arena;
    arena_init(&arena, capacity);

    size_t bytes = 512;
    void *ptr = arena_alloc(&arena, bytes);

    assert(arena.base == ptr);
    assert(arena.capacity == capacity);
    assert(arena.offset == bytes);

    arena_reset(&arena);

    assert(arena.base == ptr);
    assert(arena.capacity == capacity);
    assert(arena.offset == 0);

    arena_free(&arena);
}

void allocation_grows_multiple_time()
{
    size_t capacity = 16;

    Arena arena;
    arena_init(&arena, capacity);

    assert(arena.capacity == capacity);
    assert(arena.offset == 0);
    assert(arena.base != NULL);

    void *p1 = arena_alloc(&arena, 72);

    assert(arena.capacity == 128);
    assert(arena.offset == 72);
    assert(arena.base == p1);

    void *p2 = arena_alloc(&arena, 16);

    assert(arena.capacity == 128);
    assert(arena.offset == 72 + 16);
    assert(arena.base == p1);
    assert(arena.base + 72 == p2);

    arena_free(&arena);
}

void allocation_grows_capacity()
{
    size_t capacity = 32;

    Arena arena;
    arena_init(&arena, capacity);

    assert(arena.capacity == capacity);
    assert(arena.offset == 0);
    assert(arena.base != NULL);

    size_t bytes = 64;
    void *p1 = arena_alloc(&arena, bytes);

    assert(arena.capacity == capacity * ARENA_GROWTH_FACTOR);
    assert(arena.offset == bytes);
    assert(arena.base == p1);

    arena_free(&arena);
}

void allocate_memory()
{
    size_t capacity = 64;

    Arena arena;
    arena_init(&arena, capacity);

    size_t bytes = 32;
    void *p1 = arena_alloc(&arena, bytes);

    assert(arena.offset == bytes);
    assert(arena.capacity == capacity);
    assert(arena.base == p1);

    void *p2 = arena_alloc(&arena, bytes);

    assert(arena.offset == bytes * 2);
    assert(arena.capacity == capacity);
    assert(arena.base + bytes == p2);

    arena_free(&arena);
}

void do_nothing_if_new_capacity_is_smaller()
{
    size_t capacity = 64;

    Arena arena;
    arena_init(&arena, capacity);

    arena_grow(&arena, 32);

    assert(arena.base != NULL);
    assert(arena.offset == 0);
    assert(arena.capacity == capacity);

    arena_free(&arena);
}

void grow_arena()
{
    size_t capacity = 64;
    size_t scale_factor = 2;

    Arena arena;
    arena_init(&arena, capacity);

    arena_grow(&arena, capacity * scale_factor);

    assert(arena.base != NULL);
    assert(arena.offset == 0);
    assert(arena.capacity == capacity * scale_factor);

    arena_free(&arena);
}

void simple_init_and_free()
{
    size_t capacity = 1024;

    Arena arena;
    arena_init(&arena, capacity);

    assert(arena.capacity == capacity);
    assert(arena.offset == 0);
    assert(arena.base != NULL);

    arena_free(&arena);

    assert(arena.capacity == 0);
    assert(arena.offset == 0);
    assert(arena.base == NULL);
}

void test_arena_all()
{
    it("should init and free without any allocations", simple_init_and_free);
    it("should grow the capacity", grow_arena);
    it("should do nothing if new capacity is smaller than current capacity", do_nothing_if_new_capacity_is_smaller);
    it("should allocate the number of bytes given", allocate_memory);
    it("should grow capacity if needed bytes is larger than current capacity", allocation_grows_capacity);
    it("should grow capacity multiple times if needed bytes is larger than current capacity", allocation_grows_multiple_time);
    it("should be able to be reset", arena_can_be_reset);
}
