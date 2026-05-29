#include "dynamic_array.h"
//
DynamicArray* x_mass(TypeInf* type) {
    DynamicArray* x = (DynamicArray*)malloc(sizeof(DynamicArray));
    if (!x) exit(1);
    
    x->size = 0;
    x->capacity = 4;
    x->type = type;
    x->data = malloc(x->capacity * type->size);
    if (!x->data) { free(x); exit(1); }
    
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
        x->data = realloc(x->data, x->capacity * x->type->size);
        if (!x->data) exit(1);
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
        // Здесь используется знание, что тип int или double
        // Если хочешь полной изоляции, можно добавить указатель на comp_desc в TypeInf
        extern int int_comp_desc(const void*, const void*);
        extern int double_comp_desc(const void*, const void*);
        
        if (x->type->size == sizeof(int))
            qsort(x->data, x->size, x->type->size, int_comp_desc);
        else
            qsort(x->data, x->size, x->type->size, double_comp_desc);
    }
}

DynamicArray* xconcat(DynamicArray* a, DynamicArray* b) {
    if (a->type != b->type) return NULL;
    DynamicArray* res = x_mass(a->type);
    res->capacity = a->size + b->size;
    res->size = res->capacity;
    
    res->data = realloc(res->data, res->capacity * res->type->size);
    if (!res->data) { xfree(res); exit(1); }
    
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
    if (!res->data) { xfree(res); exit(1); }
    
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
        if (predicate(el)) xpush(res, el);
    }
    return res;
}
