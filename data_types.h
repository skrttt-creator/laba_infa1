#ifndef TYPES_H
#define TYPES_H

#include "dynamic_array.h"

extern TypeInf DoubleType;
extern TypeInf StringType;

char* my_strdup(const char* s);

void double_add(void* val, void* context);
bool double_is_positive(const void* val);

void string_add_suffix(void* val, void* context);
bool string_len_greater_than_3(const void* val);

#endif
