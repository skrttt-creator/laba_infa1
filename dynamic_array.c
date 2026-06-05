#include "dynamic_array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DynamicArray* x_mass(TypeInf* type) {
    DynamicArray* x = (DynamicArray*)malloc(sizeof(DynamicArray));
    if (x == NULL) {
        printf("ошибка: не удалось выделить память\n");
        exit(1);
    }
    x->size = 0;
    x->capacity = 4;
    x->type = type;
    x->data = malloc(x->capacity * type->size);
    if (x->data == NULL) {
        printf("ошибка: не удалось выделить память!\n");
        free(x);
        exit(1);
    }
    return x;
}

void xfree(DynamicArray* x) {
    if (x) {
        free(x->data);
        free(x);
    }
}

void xpush(DynamicArray* x, const void* val) {
    if (x->size == x->capacity) {
        x->capacity *= 2;
        void* new_data = realloc(x->data, x->capacity * x->type->size);
        if (new_data == NULL) {
            printf(""ошибка: не удалось выделить память!\n");
            exit(1);
        }
        x->data = new_data;
    }
    void* target = (char*)x->data + (x->size * x->type->size);
    memcpy(target, val, x->type->size);
    x->size++;
}

void* xget(DynamicArray* x, size_t index) {
    if (index >= x->size) return NULL;
    return (char*)x->data + (index * x->type->size);
}

void xprint(DynamicArray* x) {
    printf("[");
    for (size_t i = 0; i < x->size; i++) {
        x->type->print(xget(x, i));
        if (i < x->size - 1) printf(", ");
    }
    printf("]\n");
}

void xsort(DynamicArray* x, int order) {
    if (order == 1) {
        qsort(x->data, x->size, x->type->size, x->type->comp);
    } else if (order == 2) {
        qsort(x->data, x->size, x->type->size, x->type->comp_desc);
    }
}

DynamicArray* xconcat(DynamicArray* a, DynamicArray* b) {
    if (a->type != b->type) {
        printf(""ошибка: не удалось выделить память\n");
        return NULL;
    }
    DynamicArray* res = x_mass(a->type);
    res->capacity = a->size + b->size;
    res->size = res->capacity;
    
    void* new_data = realloc(res->data, res->capacity * res->type->size);
    if (new_data == NULL) { xfree(res); exit(1); }
    res->data = new_data;

    memcpy(res->data, a->data, a->size * a->type->size);
    void* offset = (char*)res->data + (a->size * a->type->size);
    memcpy(offset, b->data, b->size * b->type->size);
    return res;
}

DynamicArray* xmap(DynamicArray* x, void (*func)(void*, void*), void* context) {
    DynamicArray* res = x_mass(x->type);
    res->capacity = x->size;
    res->size = x->size;
    res->data = realloc(res->data, res->capacity * x->type->size);
    if (res->data == NULL) { xfree(res); exit(1); }

    for (size_t i = 0; i < x->size; i++) {
        void* src = (char*)x->data + (i * x->type->size); 
        void* dst = (char*)res->data + (i * x->type->size); 
        memcpy(dst, src, x->type->size); 
        func(dst, context); 
    }
    return res;
}

DynamicArray* xwhere(DynamicArray* x, bool (*predicate)(const void*)) {
    DynamicArray* res = x_mass(x->type);
    for (size_t i = 0; i < x->size; i++) {
        void* el = xget(x, i);
        if (predicate(el)) {
            xpush(res, el);
        }
    }
    return res;
}
