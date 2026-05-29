#ifndef DATA_TYPES_H
#define DATA_TYPES_H
#include <stdio.h>

typedef struct {
    size_t size;
    int (*comp)(const void*, const void*);
    void (*print)(const void*);
} TypeInf;

extern TypeInf IntType;
extern TypeInf DoubleType;
#endif
