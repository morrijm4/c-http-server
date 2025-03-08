#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

bool math_add_u32(uint32_t a, uint32_t b, uint32_t *result)
{
    if (b > UINT32_MAX - a) {
	fprintf(stderr, "Adding %d and %d will result in an u32 overflow", a, b);
	return false;
    }

    *result = a + b;
    return true;
}

bool math_divide_u32(uint32_t a, uint32_t b, uint32_t *result)
{
    if (b == 0) {
	fprintf(stderr, "Dividing %d and %d will result in undefined behavior", a, b);
	return false;
    }

    *result = a / b;
    return true;
}

bool math_multiply_u32(uint32_t a, uint32_t b, uint32_t *result)
{
    uint32_t tmp;
    if (!math_divide_u32(UINT32_MAX, b, &tmp)) return false;

    if (a > tmp) {
	fprintf(stderr, "Multiplying %d and %d will result in u32 overflow", a, b);
	return false;
    }

    *result = a * b;
    return true;
}


