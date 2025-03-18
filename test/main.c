#include <stdio.h>

#include "utils.c"

#include "hash-table.test.c"
#include "stack-arena.test.c"

int main()
{
    printf("Running test suite...\n");
    printf("\n");

    describe("hash table", test_hash_table_all);
    describe("stack arena", test_stack_arena_all);

    printf("\n");
    printf("All test passed!");
    printf("\n");
}
