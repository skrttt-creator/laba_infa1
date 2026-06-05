#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* my_strdup(const char* s) {
    char* d = malloc(strlen(s) + 1);
    if (d) strcpy(d, s);
    return d;
}

int double_comp(const void* a, const void* b) {
    double diff = (*(double*)a - *(double*)b);
    return (diff > 0) - (diff < 0);
}
int double_comp_desc(const void* a, const void* b) {
    double diff = (*(double*)b - *(double*)a);
    return (diff > 0) - (diff < 0);
}
void double_print(const void* a) {
    printf("%.2f", *(double*)a);
}
void double_add(void* val, void* context) {
    double addend = *(double*)context;
    *(double*)val += addend;
}
bool double_is_positive(const void* val) {
    return *(double*)val > 0.0;
}

TypeInf DoubleType = { sizeof(double), double_comp, double_comp_desc, double_print };



int string_comp(const void* a, const void* b) {
    return strcmp(*(const char**)a, *(const char**)b);
}
int string_comp_desc(const void* a, const void* b) {
    return strcmp(*(const char**)b, *(const char**)a);
}
void string_print(const void* a) {
    printf("\"%s\"", *(const char**)a);
}
void string_add_suffix(void* val, void* context) {
    const char* original = *(const char**)val;
    const char* suffix = (const char*)context;
    char* new_str = (char*)malloc(strlen(original) + strlen(suffix) + 1);
    strcpy(new_str, original);
    strcat(new_str, suffix);
    *(char**)val = new_str;
}
bool string_len_greater_than_3(const void* val) {
    return strlen(*(const char**)val) > 3;
}

TypeInf StringType = { sizeof(char*), string_comp, string_comp_desc, string_print };
