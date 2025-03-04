#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

void *Malloc(size_t size) {
  void *ptr;

  if ((ptr = malloc(size)) == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  return ptr;
}

void *Realloc(void *ptr, size_t size) {
  if (realloc(ptr, size) == NULL) {
    perror("realloc");
    exit(EXIT_FAILURE);
  }

  return ptr;
}
