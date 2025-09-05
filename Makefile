CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
MAIN = main.c
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJ) $(MAIN)
	$(CC) $(CFLAGS) $(OBJ) $(MAIN) -o $(TARGET)

clean:
	rm -f src/*.o $(TARGET)