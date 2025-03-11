#pragma once

#include <stdio.h>

void describe(const char *msg, void (func()))
{
    printf("========= %s =========\n", msg);
    func();
}

void it(const char *msg, void (func()))
{
    printf("    - %s... ", msg);
    func();
    printf("PASSED\n");
}
