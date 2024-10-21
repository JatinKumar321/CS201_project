# Makefile
CC = gcc
CFLAGS = -Wall -Wextra
TARGET = rtree_project
SRC = src/main.c src/rtree.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

