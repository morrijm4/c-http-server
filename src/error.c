#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

void print_stderr(const char *msg)
{
    fprintf(stderr, "%s\n", msg);
}

bool error_false(const char *msg)
{
    print_stderr(msg);
    return false;
}

void error_fail(const char* msg)
{
    print_stderr(msg);
    exit(EXIT_FAILURE);
}

void *error_null(const char *msg)
{
    print_stderr(msg);
    return NULL;
}

bool perror_false(const char *msg)
{
    perror(msg);
    return false;
}

int perror_int(const char *msg)
{
    perror(msg);
    return EXIT_FAILURE;
}
