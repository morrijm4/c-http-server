#pragma once

#include <assert.h>

#include "utils.c"
#include "../src/stack-arena.c"

void can_init_array()
{
    StackArena sa = sa_init();

    typedef struct { int x, y; } Point;

    Point *p;
    assert(sa_alloc_array(&sa, Point, 10, (void **)&p));
    assert(sa.offset == sizeof(Point) * 10);
}

void can_init_struct()
{
    StackArena sa = sa_init();

    typedef struct { int x, y; } Point;

    Point *p;
    assert(sa_alloc_struct(&sa, Point, (void **)&p));
    assert(sa.offset == sizeof(Point));
}

void can_reset_after_allocation()
{
    StackArena sa = sa_init();

    long *x;
    assert(sa_alloc(&sa, sizeof(long), (void **)&x));
    *x = 1;
    assert(*x == 1);

    sa_reset(&sa);

    long *y;
    assert(sa_alloc(&sa, sizeof(long), (void **)&y));
    *y = 2;
    assert(*y == 2);

    assert(sa.offset == sizeof(long));
}

void can_alloc_data()
{
    StackArena sa = sa_init();

    int *x;
    assert(sa_alloc(&sa, sizeof(int), (void **)&x));

    *x = 12;

    assert(*x == 12);
}

void can_init_empty_arena()
{
    StackArena sa = sa_init();
    assert(sa.offset == 0);
}

void test_stack_arena_all()
{
    it("can init an empty arena", can_init_empty_arena);
    it("can allocate date", can_alloc_data);
    it("can reset after allocation", can_reset_after_allocation);
    it("can init a struct", can_init_struct);
    it("can init an array", can_init_array);
}
