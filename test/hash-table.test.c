#include <assert.h>
#include <stdint.h>

#include "utils.c"
#include "../src/hash-table.c"
#include "../src/stack-arena.c"
#include "../src/string.c"

void hash_table_set_and_get()
{
    StackArena sa = sa_init();
    HashTable ht;

    assert(ht_init(&sa, &ht, 64));

    String k1 = str_from_cstr("Host");
    String v1 = str_from_cstr("localhost:3000");

    assert(ht_set(&sa, ht, k1, &v1));

    String *result = ht_get(ht, k1);

    assert(result != NULL);
    assert(str_eq(v1, *result));

    String k2 = str_from_cstr("Content-Length");
    assert(ht_set(&sa, ht, k2, "32"));
    assert(strcmp(ht_get(ht, k2), "32") == 0);

    String k3 = str_from_cstr("x-correlation-id");
    assert(ht_set(&sa, ht, k3, "1234"));
    assert(ht_set(&sa, ht, k3, "5678"));
    assert(strcmp(ht_get(ht, k3), "5678") == 0);
}

void hash_table_can_set_new_data()
{
    StackArena sa = sa_init();

    HashTable ht1;
    assert(ht_init(&sa, &ht1, 3));

    assert(ht_set(&sa, ht1, str_from_cstr("a"), "1"));
    assert(ht_set(&sa, ht1, str_from_cstr("b"), "2"));
    assert(ht_set(&sa, ht1, str_from_cstr("c"), "3"));
    assert(ht_set(&sa, ht1, str_from_cstr("c"), "4"));

    sa_reset(&sa);

    HashTable ht2;
    assert(ht_init(&sa, &ht2, 1));

    assert(ht_set(&sa, ht2, str_from_cstr("a"), "1"));
    assert(ht_set(&sa, ht2, str_from_cstr("a"), "2"));
    assert(ht_set(&sa, ht2, str_from_cstr("a"), "3"));
}

void init_hash_table()
{
    StackArena sa = sa_init();
    size_t size = 64;
    HashTable ht;

    assert(ht_init(&sa, &ht, size));

    assert(ht.len == size);

    for (int i = 0; i < ht.len; ++i) {
	assert(ht.arr[i].key.len == 0);
	assert(ht.arr[i].key.ptr == NULL);
	assert(ht.arr[i].value == NULL);
	assert(ht.arr[i].next == NULL);
    }
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

void test_hash_table_all()
{
    it("should hash a string", should_hash_string);
    it("should hash different hashes", should_be_different);
    it("should init a hash table of size len", init_hash_table);
    it("should be able to set a new value", hash_table_can_set_new_data);
    it("should be able to set and get a value", hash_table_set_and_get);
}
