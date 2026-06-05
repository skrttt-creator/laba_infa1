#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "dynamic_array.h"
#include "types.h"
#include "tests.h"

void manual_mode() {
    printf("Выберите тип данных:\n1. string (строки)\n2. double (вещественные)\nВаш выбор: ");
    int type_choice;
    scanf("%d", &type_choice);
    
    TypeInf* type = (type_choice == 1) ? &StringType : &DoubleType;
    DynamicArray* arr = x_mass(type);
    
    printf("Сколько элементов добавить? ");
    int n; scanf("%d", &n);
    
    printf("Введите %d элементов:\n", n);
    if (type == &StringType) {
        for (int i = 0; i < n; i++) {
            char buf[256]; scanf("%255s", buf);
            char* val = my_strdup(buf);
            xpush(arr, &val);
        }
    } else {
        for (int i = 0; i < n; i++) {
            double val; scanf("%lf", &val);
            xpush(arr, &val);
        }
    }
    
    printf("\nИсходный массив: "); xprint(arr);
    
    printf("\nВыберите сортировку:\n1. По возрастанию\n2. По убыванию\n");
    int order; scanf("%d", &order);
    xsort(arr, order);
    printf("После сортировки: "); xprint(arr);
    
    printf("\nПрименить Map? (1-да, 0-нет): ");
    int do_map; scanf("%d", &do_map);
    DynamicArray* mapped = NULL;
    if (do_map) {
        if (type == &StringType) {
            char suffix[256]; printf("Введите суффикс: "); scanf("%255s", suffix);
            mapped = xmap(arr, string_add_suffix, suffix);
        } else {
            double add; printf("Введите число: "); scanf("%lf", &add);
            mapped = xmap(arr, double_add, &add);
        }
        printf("После Map: "); xprint(mapped);
    }
    
    printf("\nПрименить Where? (1-да, 0-нет): ");
    int do_where; scanf("%d", &do_where);
    DynamicArray* filtered = NULL;
    if (do_where) {
        filtered = xwhere(arr, (type == &StringType) ? string_len_greater_than_3 : double_is_positive);
        printf("После Where: "); xprint(filtered);
    }
    
    if (type == &StringType) {
        if (mapped) { for (size_t i = 0; i < mapped->size; i++) free(*(char**)xget(mapped, i)); }
        for (size_t i = 0; i < arr->size; i++) free(*(char**)xget(arr, i));
    }
    if (mapped) xfree(mapped);
    if (filtered) xfree(filtered);
    xfree(arr);
}

int main() {
    int choice;
    while (1) {
        printf("\n1. Запустить автоматические тесты\n2. Ручной режим\n3. Выход\nВаш выбор: ");
        if (scanf("%d", &choice) != 1) break;
        switch (choice) {
            case 1: 
                run_all_tests(); 
                break; 
            case 2: 
                manual_mode(); 
                break;
            case 3: 
                printf("Выход.\n"); 
                return 0;
            default: 
                printf("Неверный выбор.\n");
        }
    }
    return 0;
}
