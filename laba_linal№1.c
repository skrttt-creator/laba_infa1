#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h> 

typedef struct {
    size_t size;
    int (*comp)(const void*, const void*);
    void (*print)(const void*);
} TypeInf;

// реализация для инт

int int_comp(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

int int_comp_desc(const void* a, const void* b) {
    return (*(int*)b - *(int*)a);
}

void int_print(const void* a) {
    printf("%d", *(int*)a);
}

TypeInf IntType = { sizeof(int), int_comp, int_print };

// реализация для дабл

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

TypeInf DoubleType = { sizeof(double), double_comp, double_print };

// структура

typedef struct {
    void* data;
    size_t size;
    size_t capacity;
    TypeInf* type;
} DynamicArray;

// основные функции

//создание нового массива
DynamicArray* x_mass(TypeInf* type) {
    DynamicArray* x = (DynamicArray*)malloc(sizeof(DynamicArray));
    if (x == NULL) {
        printf("Ошибка: не удалось выделить память под структуру!\n");
        exit(1);
    }
    
    x->size = 0;
    x->capacity = 4;
    x->type = type;
    x->data = malloc(x->capacity * type->size);
    
    if (x->data == NULL) {
        printf("Ошибка: не удалось выделить память под данные!\n");
        free(x);
        exit(1);
    }
    return x;
}

//очистка массива
void xfree(DynamicArray* x) {
    if (x) {
        free(x->data);
        free(x);
    }
}
//добавление элемента в конец
void xpush(DynamicArray* x, const void* val) {
    if (x->size == x->capacity) {
        x->capacity *= 2;
        void* new_data = realloc(x->data, x->capacity * x->type->size);
        if (new_data == NULL) {
            printf("Ошибка: не удалось перевыделить память!\n");
            exit(1);
        }
        x->data = new_data;
    }
    void* target = (char*)x->data + (x->size * x->type->size);
    memcpy(target, val, x->type->size);
    x->size++;
}

//получение индекса
void* xget(DynamicArray* x, size_t index) {
    if (index >= x->size) return NULL;
    return (char*)x->data + (index * x->type->size);
}

//вывод массива
void xprint(DynamicArray* x) {
    printf("[");
    for (size_t i = 0; i < x->size; i++) {
        x->type->print(xget(x, i));
        if (i < x->size - 1) printf(", ");
    }
    printf("]\n");
}

//сортировка
void xsort(DynamicArray* x, int order) {
    if (order == 1) {
        qsort(x->data, x->size, x->type->size, x->type->comp);
    } else if (order == 2) {
        if (x->type->size == sizeof(int)) {
            qsort(x->data, x->size, x->type->size, int_comp_desc);
        } else if (x->type->size == sizeof(double)) {
            qsort(x->data, x->size, x->type->size, double_comp_desc);
        }
    }
}

// склейка массивов(конкатенация)
DynamicArray* xconcat(DynamicArray* a, DynamicArray* b) {
    if (a->type != b->type) {
        printf("Ошибка: попытка склеить массивы разных типов!\n");
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
    if (res->data == NULL) {xfree(res); exit(1);}

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

// тестовые функции для мап

void int_multiply_by(void* val, void* context) {
    int multiplier = *(int*)context;
    *(int*)val *= multiplier;
}

void double_add(void* val, void* context) {
    double addend = *(double*)context;
    *(double*)val += addend;
}

// предикаты для where

bool int_is_even(const void* val) {
    return (*(int*)val % 2) == 0;
}

bool double_is_positive(const void* val) {
    return *(double*)val > 0.0;
}

// автоматические тесты

bool test_int_array() {
    printf("\nзапуск автотестов для int\n");
    DynamicArray* arr = x_mass(&IntType);
    
    // Тест Push
    int vals[] = {5, 2, 9, 1};
    for (int i = 0; i < 4; i++) xpush(arr, &vals[i]);
    assert(arr->size == 4); // 
    assert(*(int*)xget(arr, 0) == 5);
    
    // Тест Sort
    xsort(arr, 1);
    assert(*(int*)xget(arr, 0) == 1);
    assert(*(int*)xget(arr, 3) == 9);
    
    // Тест Map
    int mult = 10;
    DynamicArray* mapped = xmap(arr, int_multiply_by, &mult);
    assert(*(int*)xget(mapped, 0) == 10);
    assert(*(int*)xget(mapped, 3) == 90);
    
    // Тест Where
    DynamicArray* filtered = xwhere(arr, int_is_even);
    assert(filtered->size == 1);
    assert(*(int*)xget(filtered, 0) == 2);
    
    // Тест Concat 
    DynamicArray* arr2 = x_mass(&IntType);
    int val = 99;
    xpush(arr2, &val);
    DynamicArray* concated = xconcat(arr, arr2);
    assert(concated->size == 5);
    assert(*(int*)xget(concated, 4) == 99);
    
    xfree(arr); xfree(mapped); xfree(filtered); xfree(arr2); xfree(concated);
    printf("  [ACCEPT] Все тесты INT пройдены успешно!\n");
    return true;
}

bool test_double_array() {
    printf("\ запуск автотестов для doble\n");
    DynamicArray* arr = x_mass(&DoubleType);
    
    double vals[] = {-1.5, 3.14};
    for (int i = 0; i < 2; i++) xpush(arr, &vals[i]);
    assert(arr->size == 2);
    
    double addend = 1.0;
    DynamicArray* mapped = xmap(arr, double_add, &addend);
    // Проверяем с небольшой погрешностью, т.к. это double
    assert(*(double*)xget(mapped, 0) > -0.51 && *(double*)xget(mapped, 0) < -0.49);
    
    DynamicArray* filtered = xwhere(arr, double_is_positive);
    assert(filtered->size == 1);
    
    xfree(arr); xfree(mapped); xfree(filtered);
    printf("  [ACCEPT] Все тесты DOUBLE пройдены успешно!\n");
    return true;
}

void run_all_tests() {
    printf("АВТОМАТИЧЕСКОЕ ТЕСТИРОВАНИЕ ЧЕРЕЗ ASSERT\n");
    test_int_array();
    test_double_array();
}


void manual_mode() {
    printf("Выберите тип данных:\n");
    printf("1. int\n");
    printf("2. double\n");
    int type_choice;
    scanf("%d", &type_choice);
    
    TypeInf* type;
    if (type_choice == 1) {
        type = &IntType;
    } else {
        type = &DoubleType;
    }
    
    DynamicArray* arr = x_mass(type);
    
    printf("Сколько элементов добавить? ");
    int n;
    scanf("%d", &n);
    
    printf("Введите %d элементов:\n", n);
    if (type == &IntType) {
        for (int i = 0; i < n; i++) {
            int val;
            scanf("%d", &val);
            xpush(arr, &val);
        }
    } else {
        for (int i = 0; i < n; i++) {
            double val;
            scanf("%lf", &val);
            xpush(arr, &val);
        }
    }
    
    printf("\nИсходный массив: ");
    xprint(arr);
    
    // Сортировка
    printf("\nВыберите сортировку:\n");
    printf("1. По возрастанию\n");
    printf("2. По убыванию\n");
    int order;
    scanf("%d", &order);
    xsort(arr, order);
    printf("После сортировки: ");
    xprint(arr);
    
    // Map
    printf("\nПрименить Map? (1-да, 0-нет): ");
    int do_map;
    scanf("%d", &do_map);
    if (do_map) {
        if (type == &IntType) {
            int mult;
            printf("Введите множитель: ");
            scanf("%d", &mult);
            DynamicArray* mapped = xmap(arr, int_multiply_by, &mult);
            printf("После Map: ");
            xprint(mapped);
            xfree(mapped);
        } else {
            double add;
            printf("Введите число для прибавления: ");
            scanf("%lf", &add);
            DynamicArray* mapped = xmap(arr, double_add, &add);
            printf("После Map: ");
            xprint(mapped);
            xfree(mapped);
        }
    }
    
    // Where
    printf("\nПрименить Where? (1-да, 0-нет): ");
    int do_where;
    scanf("%d", &do_where);
    if (do_where) {
        DynamicArray* filtered;
        if (type == &IntType) {
            filtered = xwhere(arr, int_is_even);
            printf("После Where (чётные): ");
        } else {
            filtered = xwhere(arr, double_is_positive);
            printf("После Where (положительные): ");
        }
        xprint(filtered);
        xfree(filtered);
    }
    

    printf("\nХотите склеить этот массив с новым? (1-да, 0-нет): ");
    int do_concat;
    scanf("%d", &do_concat);
    if (do_concat) {
        DynamicArray* arr_second = x_mass(type);
        printf("Добавим 1 элемент во второй массив для наглядности.\nВведите элемент: ");
        if (type == &IntType) {
            int val; scanf("%d", &val); xpush(arr_second, &val);
        } else {
            double val; scanf("%lf", &val); xpush(arr_second, &val);
        }
        DynamicArray* concated = xconcat(arr, arr_second);
        printf("Результат склейки: ");
        xprint(concated);
        xfree(arr_second);
        xfree(concated);
    }
    
    xfree(arr);
}



int main() {
    int choice;
    while (1) {
        printf("1. Запустить автоматические тесты \n");
        printf("2. Ручной режим \n");
        printf("3. Выход\n");
        printf("Ваш выбор: ");
        
        if (scanf("%d", &choice) != 1) break;
        
        switch (choice) {
            case 1:
                run_all_tests();
                break;
            case 2:
                manual_mode();
                break;
            case 3:
                printf("Выход из программы.\n");
                return 0;
            default:
                printf("Неверный выбор. Попробуйте ещё раз.\n");
        }
    }
    return 0;
}
