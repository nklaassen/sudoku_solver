#ifndef SUDOKU_HEADER
#define SUDOKU_HEADER

#include <string.h>

struct Board
{
	unsigned int cell[9][9];
};

int solve(struct Board *board);
int recursiveSolve(struct Board *board, int depth);
int setCell(struct Board *board, unsigned int cell, int i, int j);
int getCell(struct Board *board, unsigned int *cell, int i, int j);

int checkRows(struct Board *board);
int checkCols(struct Board *board);
int checkBoxes(struct Board *board);
int checkDone(struct Board *board);
int init(struct Board *board);
int recursiveMask(struct Board *board, int row, int col, unsigned int mask);
int maskBoxExceptRow(struct Board *board, int row, int box, unsigned int mask);
int maskBoxExceptCol(struct Board *board, int col, int box, unsigned int mask);
int maskRowExceptBox(struct Board *board, int box, int row, unsigned int mask);
int maskColExceptBox(struct Board *board, int box, int col, unsigned int mask);
unsigned int getPosInRow(struct Board *board, int val, int row);
unsigned int getPosInCol(struct Board *board, int val, int col);
unsigned int getPosInBox(struct Board *board, int val, int box);
int otherLinesInBox(const int line, int *line1, int *line2);
int getFirstUndecided(struct Board *board, int *row, int *col);
unsigned int guess(unsigned int val);

#include "io.h"

#endif
