#ifndef IO_HEADER
#define IO_HEADER

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sudoku.h"


int parseInput(struct Board *board, FILE *stream);
int printBoard(struct Board *board, FILE *stream);

#endif
