#include "data_types.h"

int int_comp(const void* a, const void* b) { return (*(int*)a - *(int*)b); }
void int_print(const void* a) { printf("%d", *(int*)a); }
TypeInf IntType = { sizeof(int), int_comp, int_print };

int double_comp(const void* a, const void* b) { 
    double diff = (*(double*)a - *(double*)b); 
    return (diff > 0) - (diff < 0); 
}
void double_print(const void* a) { printf("%.2f", *(double*)a); }
TypeInf DoubleType = { sizeof(double), double_comp, double_print };
