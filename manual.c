#include "dynamic_array.h"

void manual_mode() {
    printf("\n--- РУЧНОЙ РЕЖИМ ---\n");
    printf("Выберите тип (1 - int, 2 - double): ");
    int choice; scanf("%d", &choice);
    TypeInf* type = (choice == 1) ? &IntType : &DoubleType;
    
    DynamicArray* arr = x_mass(type);
    printf("Сколько элементов? ");
    int n; scanf("%d", &n);
    
    for (int i = 0; i < n; i++) {
        if (type == &IntType) { int v; scanf("%d", &v); xpush(arr, &v); }
        else { double v; scanf("%lf", &v); xpush(arr, &v); }
    }
    
    printf("Результат: "); xprint(arr);
    xfree(arr);
}
