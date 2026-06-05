#include "tests.h"
#include "dynamic_array.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static bool test_double_array(void) {
    printf("\nЗапуск автотестов для double\n");
    DynamicArray* arr = x_mass(&DoubleType);
    double vals[] = {-1.5, 3.14};
    xpush(arr, &vals[0]); xpush(arr, &vals[1]);
    
    double addend = 1.0;
    DynamicArray* mapped = xmap(arr, double_add, &addend);
    assert(*(double*)xget(mapped, 0) > -0.51 && *(double*)xget(mapped, 0) < -0.49);
    
    DynamicArray* filtered = xwhere(arr, double_is_positive);
    assert(filtered->size == 1);
    
    xfree(arr); xfree(mapped); xfree(filtered);
    printf("  [ACCEPT] Все тесты DOUBLE пройдены успешно!\n");
    return true;
}

static bool test_string_array(void) {
    printf("\nЗапуск автотестов для string\n");
    DynamicArray* arr = x_mass(&StringType);
    const char* vals[] = {"banana", "apple", "kiwi", "ox"};
    for (int i = 0; i < 4; i++) xpush(arr, &vals[i]);
    
    xsort(arr, 1);
    assert(strcmp(*(char**)xget(arr, 0), "apple") == 0);
    
    DynamicArray* mapped = xmap(arr, string_add_suffix, "!");
    assert(strcmp(*(char**)xget(mapped, 0), "apple!") == 0);
    
    DynamicArray* filtered = xwhere(arr, string_len_greater_than_3);
    assert(filtered->size == 3);
    
    for (size_t i = 0; i < mapped->size; i++) free(*(char**)xget(mapped, i));
    xfree(arr); xfree(mapped); xfree(filtered);
    printf("  [ACCEPT] Все тесты STRING пройдены успешно!\n");
    return true;
}

void run_all_tests(void) {
    printf("АВТОМАТИЧЕСКОЕ ТЕСТИРОВАНИЕ ЧЕРЕЗ ASSERT\n");
    test_double_array();
    test_string_array();
}
