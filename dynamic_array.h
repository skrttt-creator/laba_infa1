#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stddef.h>
#include <stdbool.h>

typedef struct {
    size_t size;
    int (*comp)(const void*, const void*);
    int (*comp_desc)(const void*, const void*); 
    void (*print)(const void*);
} TypeInf;

typedef struct {
    void* data;
    size_t size;
    size_t capacity;
    TypeInf* type;
} DynamicArray;

DynamicArray* x_mass(TypeInf* type);
void xfree(DynamicArray* x);
void xpush(DynamicArray* x, const void* val);
void* xget(DynamicArray* x, size_t index);
void xprint(DynamicArray* x);
void xsort(DynamicArray* x, int order);
DynamicArray* xconcat(DynamicArray* a, DynamicArray* b);
DynamicArray* xmap(DynamicArray* x, void (*func)(void*, void*), void* context);
DynamicArray* xwhere(DynamicArray* x, bool (*predicate)(const void*));

#endif
