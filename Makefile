#SRC=$(wildcard *.c)
SRC=io.c sudoku.c
HEADERS=$(wildcard *.h)
OBJS=$(SRC:.c=.o)
TARGET=sudoku.out
TARGET_SRC=main.c
TARGET_OBJ=main.o
TEST=testsudoku.out
TEST_SRC=test.c
TEST_OBJ=test.o
CC=gcc
CFLAGS=-Wextra -Wall -Wshadow

all: $(TARGET) $(TEST)

$(TARGET): $(OBJS) $(TARGET_OBJ)
	$(CC) -Wall $(OBJS) $(TARGET_OBJ) -o $(TARGET)

$(TEST): $(OBJS) $(TEST_OBJ)
	$(CC) -Wall $(OBJS) $(TEST_OBJ) -o $(TEST)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

fresh: clean all

clean:
	rm -fv *.o *.out
