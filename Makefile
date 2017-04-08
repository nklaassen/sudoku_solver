SRC=$(wildcard *.c)
HEADERS=$(wildcard *.h)
OBJS=$(SRC:.c=.o)
TARGET=sudoku.out
CC=gcc
CFLAGS=-Wextra -Wall -Wshadow

all: $(TARGET)

$(TARGET): $(OBJS) $(TARGET_OBJ)
	$(CC) $(OBJS) -o $(TARGET)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

fresh: clean all

clean:
	rm -fv *.o *.out
