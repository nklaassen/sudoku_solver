#include "sudoku.h"

#define pop(value) __builtin_popcount(value)

int setNode(struct Board *board, unsigned int node, int i, int j)
{
	if(0 == node) {
		board->node[i][j] = 0;
	}
	else {
		board->node[i][j] = 1 << (node - 1);
	}
	return 0;
}

int getNode(struct Board *board, unsigned int *node, int i, int j)
{
	unsigned int index;
	index = board->node[i][j];
	if(1 == pop(index)) {
			*node = 1;
			while(index >>= 1) (*node)++;		
	}
	else {
		*node = 0;
	}
	return 0;
}

int maskNode(struct Board *board, int i, int j);

int clearBoard(struct Board *board);

int solve(struct Board *board);
