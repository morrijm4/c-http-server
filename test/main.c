#include <stdio.h>

#include "utils.c"
#include "hash.test.c"

int main()
{
    printf("Running test suite...\n");
    printf("\n");
    describe("hash", test_hash_all); 
    printf("\n");
    printf("All test passed!");
    printf("\n");
}
