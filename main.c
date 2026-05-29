#include "dynamic_array.h"

// Объявляем функции из других файлов, чтобы компилятор знал о них
void run_all_tests();
void manual_mode();

int main() {
    int choice;
    while (1) {
        printf("\n1. Запустить автотесты\n");
        printf("2. Ручной режим\n");
        printf("3. Выход\n");
        printf("Ваш выбор: ");
        
        if (scanf("%d", &choice) != 1) break;
        
        switch (choice) {
            case 1: run_all_tests(); break;
            case 2: manual_mode(); break;
            case 3: printf("Выход.\n"); return 0;
            default: printf("Неверно.\n");
        }
    }
    return 0;
}
