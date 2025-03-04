#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "alloc.c"

typedef struct {
  const char *buf;
  size_t len;
} String;

typedef struct {
  String *str;
  void *next;
} StringList;

String String_new(char *buf, size_t size) {
  String str = {0};

  str.len = size;
  str.buf = buf;

  return str;
}

String String_new_static(const char *str) {
  size_t len = strlen(str);

  return (String){
      .buf = str,
      .len = len,
  };
}

String String_new_empty() { return String_new("", 0); }

char String_at(String *str, int i) {
  if (str->len < i) {
    printf("Out of range error!\n");
    exit(EXIT_FAILURE);
  }

  return str->buf[i];
}

bool String_starts_with(String *str, String *value) {
  if (str->len < value->len)
    return false;

  for (int i = 0; i < value->len; ++i) {
    if (String_at(str, i) != String_at(value, i))
      return false;
  }

  return true;
}

bool String_cmp(String *s1, String *s2) {
  if (s1->len != s2->len) {
    return false;
  }

  return String_starts_with(s1, s2);
}

int String_index_of(String *str, char ch) {
  int i = 0;
  while (i < str->len && String_at(str, i) != ch) {
    i += 1;
  }

  if (i > str->len) return -1;

  return i;
}

StringList String_split(String *str, String *delim) {
  StringList list = {0};

  for (int i = 0; i < str->len; ++i) {

    // Compare str and delim
    for (int j = i; j < delim->len && j < str->len; ++j) {
      if (String_at(str, j) != String_at(delim, j)) {
        break;
      }
    }
  }

  return list;
}
