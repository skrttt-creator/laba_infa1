#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H
#include "data_types.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    void* data;
    size_t size;
    size_t capacity;
    TypeInf* type;
} DynamicArray;

DynamicArray* x_mass(TypeInf* type);
void xfree(DynamicArray* x);
void xpush(DynamicArray* x, const void* val);
void xsort(DynamicArray* x, int order);
// (остальные функции: xmap, xwhere, xconcat)
#endif
