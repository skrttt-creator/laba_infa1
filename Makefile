# Имя исполняемого файла
TARGET = my_app

# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Список всех объектных файлов
OBJS = main.o dynamic_array.o data_types.o tests.o

# Правило сборки
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Правила для каждого файла
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Очистка проекта
clean:
	rm -f $(OBJS) $(TARGET)
