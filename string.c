#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  const char *ptr;
  int len;
} String;

String str_from(const char *ptr, int len) {
  String str;

  str.ptr = ptr;
  str.len = len;

  return str;
}

String str_from_cstr(const char *ptr) {
  return str_from(ptr, strlen(ptr));
}

bool str_eq(String a, String b) {
  if (a.len != b.len) {
    return false;
  } else {
    return memcmp(a.ptr, b.ptr, a.len) == 0;
  }
}

bool str_try_chop_by_delim(String *str, char delim, String *result) {
  int i = 0;
  while (i < str->len && str->ptr[i] != delim) {
    i += 1;
  }

  if (i == str->len) {
    return false;
  }

  if (result != NULL) {
    *result = str_from(str->ptr, i);
  }

  str->len -= i + 1;
  str->ptr += i + 1;

  return true;
}

void str_println(String str) {
  for (int i = 0; i < str.len; ++i) {
    putchar(str.ptr[i]);
  }
  putchar('\n');
}
