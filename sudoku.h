#ifndef SUDOKU_HEADER
#define SUDOKU_HEADER

struct Board
{
	unsigned int node[9][9];
};

int setNode(struct Board *board, unsigned int node, int i, int j);
int getNode(struct Board *board, unsigned int *node, int i, int j);
int maskNode(struct Board *board, int i, int j);
int clearBoard(struct Board *board);
int solve(struct Board *board);

#endif
