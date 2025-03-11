#include <assert.h>
#include <stdint.h>

#include "utils.c"
#include "../src/hash.c"
#include "../src/string.c"

void cstr_and_normal_should_be_same()
{
    uint64_t h1 = hash_djb2(str_from_cstr("Via"));
    uint64_t h2 = hash_djb2_cstr("Via");
    assert(h1 == h2);
}

void should_hash_string()
{
    String str = str_from_cstr("Content-Type");
    uint64_t hash = hash_djb2(str);
    assert(hash);
}

void should_be_different()
{
    String s1 = str_from_cstr("Host");
    String s2 = str_from_cstr("Content-Type");

    uint64_t h1 = hash_djb2(s1);
    uint64_t h2 = hash_djb2(s2);

    assert(h1 != h2);
}

void test_hash_all()
{
    it("should hash a string", should_hash_string);
    it("should hash different hashes", should_be_different);
    it("normal and cstr impl should produce same output", cstr_and_normal_should_be_same);
}
