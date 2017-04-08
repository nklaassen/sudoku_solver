#ifndef SUDOKU_HEADER
#define SUDOKU_HEADER

struct Board
{
	unsigned int node[9][9];
};

int setNode(struct Board *board, unsigned int node, int i, int j);
int getNode(struct Board *board, unsigned int *node, int i, int j);
int solve(struct Board *board);

int mask(struct Board *board);
int checkRows(struct Board *board);
int checkCols(struct Board *board);
int checkBoxes(struct Board *board);
int checkDone(struct Board *board);
unsigned int getPosInRow(struct Board *board, int val, int row);
unsigned int getPosInCol(struct Board *board, int val, int col);
int maskBoxExceptRow(struct Board *board, int row, int box, unsigned int mask);
int maskBoxExceptCol(struct Board *board, int col, int box, unsigned int mask);

#include "io.h"

#endif
