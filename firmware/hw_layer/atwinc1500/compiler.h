#pragma once

#include <stddef.h>
#include <stdbool.h>

void* malloc(size_t);
void free(void*);

#define min(a, b) ((a)<(b)?(a):(b))

// TODO: what is this for?
