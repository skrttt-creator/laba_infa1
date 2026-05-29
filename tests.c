#include "dynamic_array.h"
#include <assert.h>

// Вспомогательные функции для тестирования
void int_multiply_by(void* val, void* context) { *(int*)val *= *(int*)context; }
bool int_is_even(const void* val) { return (*(int*)val % 2) == 0; }
void double_add(void* val, void* context) { *(double*)val += *(double*)context; }
bool double_is_positive(const void* val) { return *(double*)val > 0.0; }

void test_int_array() {
    DynamicArray* arr = x_mass(&IntType);
    int vals[] = {5, 2, 9, 1};
    for (int i = 0; i < 4; i++) xpush(arr, &vals[i]);
    
    xsort(arr, 1);
    assert(*(int*)xget(arr, 0) == 1);
    
    int mult = 10;
    DynamicArray* mapped = xmap(arr, int_multiply_by, &mult);
    assert(*(int*)xget(mapped, 3) == 90);
    
    xfree(arr); xfree(mapped);
    printf("  [OK] Тесты INT пройдены.\n");
}

void test_double_array() {
    DynamicArray* arr = x_mass(&DoubleType);
    double vals[] = {-1.5, 3.14};
    for (int i = 0; i < 2; i++) xpush(arr, &vals[i]);
    
    double add = 1.0;
    DynamicArray* mapped = xmap(arr, double_add, &add);
    assert(*(double*)xget(mapped, 1) > 4.13 && *(double*)xget(mapped, 1) < 4.15);
    
    xfree(arr); xfree(mapped);
    printf("  [OK] Тесты DOUBLE пройдены.\n");
}

void run_all_tests() {
    printf("\nЗАПУСК АВТОТЕСТОВ\n");
    test_int_array();
    test_double_array();
    printf("==========================\n");
}
