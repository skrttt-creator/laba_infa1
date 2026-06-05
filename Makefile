CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = lab26

SRCS = main.c dynamic_array.c types.c tests.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
