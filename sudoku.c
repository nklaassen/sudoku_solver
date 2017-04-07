#include "sudoku.h"

#define pop(value) __builtin_popcount(value)
#define LEFTMASK 0x1C0
#define MIDMASK 0x38
#define RIGHTMASK 0x7


int setNode(struct Board *board, unsigned int node, int i, int j)
{
	if(0 == node) {
		board->node[i][j] = 0x1FF;
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

int solve(struct Board *board)
{
	int change;
	do
	{
		change = mask(board);
		change |= checkRows(board);
		change |= checkCols(board);
		change |= checkBoxes(board);
	} while (change);
	return checkDone(board);
}

int mask(struct Board *board)
{
	int row, col, i;
	unsigned int mask;
	for(row = 0; row < 9; row++)
	{
		for(col = 0; col < 9; col++)
		{
			if(1 == pop(board->node[row][col]))
			{
				mask = ~(board->node[row][col]);
				for(i = 0; i < 9; i++)
				{
					if(i != row) {
						board->node[i][col] &= ~board->node[row][col];
					}
					if(i != col) {
						board->node[row][i] &= ~board->node[row][col];
					}
				}
				maskBoxExceptRow(board, row, col / 3, mask);
				maskBoxExceptCol(board, col, row / 3, mask);
			}
		}
	}
	return 0;
}

unsigned int getPosInRow(struct Board *board, unsigned int val, unsigned int row)
{
	unsigned int col, pos;
	pos = 0;
	for(col = 0; col < 9; col++)
	{
		pos |= ((board->node[row][col] >> val) & 1) << (8 - col);
	}
	return pos;
}

unsigned int getPosInCol(struct Board *board, unsigned int val, unsigned int col)
{
	unsigned int row, pos;
	pos = 0;
	for(row = 0; row < 9; row++)
	{
		pos |= ((board->node[row][col] >> val) & 1) << (8 - row);
	}
	return pos;
}

int maskBoxExceptRow(struct Board *board, int row, int box, unsigned int mask)
{
	int a, b;
	switch(row % 3) {
		case 0:
			a = row + 1;
			b = row + 2;
			break;
		case 1:
			a = row - 1;
			b = row + 1;
			break;
		default:
			a = row - 2;
			b = row - 1;
	}
	board->node[a][3*box] &= mask;
	board->node[a][3*box + 1] &= mask;
	board->node[a][3*box + 2] &= mask;
	board->node[b][3*box] &= mask;
	board->node[b][3*box + 1] &= mask;
	board->node[b][3*box + 2] &= mask;
	return 0;
}

int maskBoxExceptCol(struct Board *board, int col, int box, unsigned int mask)
{
	int a, b;
	switch(col % 3) {
		case 0:
			a = col + 1;
			b = col + 2;
			break;
		case 1:
			a = col - 1;
			b = col + 1;
			break;
		default:
			a = col - 2;
			b = col - 1;
	}
	board->node[3*box][a] &= mask;
	board->node[3*box + 1][a] &= mask;
	board->node[3*box + 2][a] &= mask;
	board->node[3*box][b] &= mask;
	board->node[3*box + 1][b] &= mask;
	board->node[3*box + 2][b] &= mask;
	return 0;
}

int checkRows(struct Board *board)
{
	unsigned int row, val, pos;
	for(row = 0; row < 9; row++)
	{
		for(val = 0; val < 9; val++)
		{
			pos = getPosInRow(board, val, row);
			if(!(pos & LEFTMASK))
			{/*all allowed val in left box*/
				maskBoxExceptRow(board, row, 0, ~(1 << val));
			}
			else if(!(pos & MIDMASK))
			{/*all allowed val in middle box*/
				maskBoxExceptRow(board, row, 1, ~(1 << val));
			}
			else if(!(pos & RIGHTMASK))
			{/*all allowed val in right box*/
				maskBoxExceptRow(board, row, 2, ~(1 << val));
			}
		}
	}
	return 0;
}

int checkCols(struct Board *board)
{
	unsigned int col, val, pos;
	for(col = 0; col < 9; col++)
	{
		for(val = 0; val < 9; val++)
		{
			pos = getPosInCol(board, val, col);
			if(!(pos & LEFTMASK))
			{/*all allowed val in left box*/
				maskBoxExceptCol(board, col, 0, ~(1 << val));
			}
			else if(!(pos & MIDMASK))
			{/*all allowed val in middle box*/
				maskBoxExceptCol(board, col, 1, ~(1 << val));
			}
			else if(!(pos & RIGHTMASK))
			{/*all allowed val in right box*/
				maskBoxExceptCol(board, col, 2, ~(1 << val));
			}
		}
	}
	return 0;
}

int checkBoxes(struct Board *board)
{
	(void) board;
	return 0;
}

int checkDone(struct Board *board)
{
	(void) board;
	return 0;
}

