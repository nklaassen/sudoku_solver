#include "sudoku.h"

#define pop(value) __builtin_popcount(value)
#define LEFTMASK 0x3F
#define MIDMASK 0x1C7
#define RIGHTMASK 0x1F8
#define FIRSTMASK 0xDB
#define SECONDMASK 0x16D
#define THIRDMASK 0x1B6

int setCell(struct Board *board, unsigned int cell, int i, int j)
{
	if(0 == cell) {
		board->cell[i][j] = 0x1FF;
	}
	else {
		board->cell[i][j] = 1 << (cell - 1);
	}
	return 0;
}

int getCell(struct Board *board, unsigned int *cell, int i, int j)
{
	unsigned int index;
	index = board->cell[i][j];
	if(1 == pop(index)) {
			*cell = 1;
			while(index >>= 1) (*cell)++;		
	}
	else {
		*cell = 0;
	}
	return 0;
}

int solve(struct Board *board)
{
	int i = 0, change;
	mask(board);
	do
	{
		change = 0;
		change |= checkRows(board);
		change |= checkCols(board);
		change |= checkBoxes(board);
		i++;
	} while (!checkDone(board) && change);
	printf("Done in %d iterations\n", i);
	return checkDone(board);
}

int otherLinesInBox(const int line, int *line1, int *line2)
{
		switch(line % 3) 
		{
			case 0:
				*line1 = line + 1; *line2 = line + 2;
				break;
			case 1:
				*line1 = line - 1; *line2 = line + 1;
				break;
			default:
				*line1 = line - 2; *line2 = line - 1;
		}
		return 0;
}

int mask(struct Board *board)
{
	int row, col, i, row1, row2, col1, col2;
	unsigned int mask;
	for(row = 0; row < 9; row++)
	{
		for(col = 0; col < 9; col++)
		{
			if(1 == pop(board->cell[row][col]))
			{
				mask = ~(board->cell[row][col]);
				for(i = 0; i < 9; i++)
				{
					if(i != row) {
						board->cell[i][col] &= mask;
					}
					if(i != col) {
						board->cell[row][i] &= mask;
					}
				}
				otherLinesInBox(row, &row1, &row2);
				otherLinesInBox(col, &col1, &col2);
				board->cell[row1][col1] &= mask;
				board->cell[row1][col2] &= mask;
				board->cell[row2][col1] &= mask;
				board->cell[row2][col2] &= mask;
			}
		}
	}
	return 0;
}

int recursiveMask(struct Board *board, int row, int col, unsigned int mask)
{
	int i, change, row1, col1, row2, col2;
	change = 0;
	if(1 != pop(board->cell[row][col]))
	{
		change = board->cell[row][col] & ~mask;/*return non-zero if change*/
		board->cell[row][col] &= mask;
		if(1 == pop(board->cell[row][col]))
		{/*if cell is now decided, recurse on all affected cells*/
			mask = ~(board->cell[row][col]);
			for(i = 0; i < 9; i++)
			{
				if(i != row) {
					recursiveMask(board, i, col, mask);
				}
				if(i != col) {
					recursiveMask(board, row, i, mask);
				}
			}
			otherLinesInBox(row, &row1, &row2);
			otherLinesInBox(col, &col1, &col2);
			recursiveMask(board, row1, col1, mask);
			recursiveMask(board, row1, col2, mask);
			recursiveMask(board, row2, col1, mask);
			recursiveMask(board, row2, col2, mask);
		}
	}
	return change;
}

unsigned int getPosInRow(struct Board *board, int val, int row)
{
	unsigned int col, pos;
	pos = 0;
	for(col = 0; col < 9; col++)
	{
		pos |= ((board->cell[row][col] >> val) & 1) << (8 - col);
	}
	return pos;
}

unsigned int getPosInCol(struct Board *board, int val, int col)
{
	int row;
	unsigned int pos;
	pos = 0;
	for(row = 0; row < 9; row++)
	{
		pos |= ((board->cell[row][col] >> val) & 1) << (8 - row);
	}
	return pos;
}

unsigned int getPosInBox(struct Board *board, int val, int box)
{
	int i, row, col;
	unsigned int pos;
	pos = 0;
	for(i = 0; i < 9; i++)
	{
		row = (box / 3) + (i / 3);
		col = (box % 3) + (i % 3);
		pos |= ((board->cell[row][col] >> val) & 1) << (8 - i);
	}
	return pos;
}

int maskBoxExceptRow(struct Board *board, int row, int box, unsigned int mask)
{
	int row1, row2, i, change;
	change = 0;
	otherLinesInBox(row, &row1, &row2);
	for(i = 0; i < 3; i++)
	{
		change |= recursiveMask(board, row1, 3*box + i, mask);
		change |= recursiveMask(board, row2, 3*box + i, mask);
	}
	return change;
}

int maskBoxExceptCol(struct Board *board, int col, int box, unsigned int mask)
{
	int col1, col2, i, change;
	change = 0;
	otherLinesInBox(col, &col1, &col2);
	for(i = 0; i < 3; i++)
	{
		change |= recursiveMask(board, 3*box + i, col1, mask);
		change |= recursiveMask(board, 3*box + i, col2, mask);
	}
	return change;
}

int maskRowExceptBox(struct Board *board, int box, int row, unsigned int mask)
{
	int col, change;
	change = 0;
	for(col = 0; col < 9; col++)
	{
		if((col / 3) != box) {
			change |= recursiveMask(board, row, col, mask);
		}
	}
	return change;
}

int maskColExceptBox(struct Board *board, int box, int col, unsigned int mask)
{
	int row, change;
	change = 0;
	for(row = 0; row < 9; row++)
	{
		if((row / 3) != box) {
			change |= recursiveMask(board, row, col, mask);
		}
	}
	return change;
}

int checkRows(struct Board *board)
{
	int row, val, change;
	unsigned int pos;
	change = 0;
	for(row = 0; row < 9; row++)
	{
		for(val = 0; val < 9; val++)
		{
			pos = getPosInRow(board, val, row);
			if(!(pos & LEFTMASK))
			{/*all allowed val in left box*/
				change |= maskBoxExceptRow(board, row, 0, ~(1 << val));
			}
			else if(!(pos & MIDMASK))
			{/*all allowed val in middle box*/
				change |= maskBoxExceptRow(board, row, 1, ~(1 << val));
			}
			else if(!(pos & RIGHTMASK))
			{/*all allowed val in right box*/
				change |= maskBoxExceptRow(board, row, 2, ~(1 << val));
			}
		}
	}
	return change;
}

int checkCols(struct Board *board)
{
	int col, val, change;
	unsigned int pos;
	change = 0;
	for(col = 0; col < 9; col++)
	{
		for(val = 0; val < 9; val++)
		{
			pos = getPosInCol(board, val, col);
			if(!(pos & LEFTMASK))
			{/*all allowed val in left box*/
				change |= maskBoxExceptCol(board, col, 0, ~(1 << val));
			}
			else if(!(pos & MIDMASK))
			{/*all allowed val in middle box*/
				change |= maskBoxExceptCol(board, col, 1, ~(1 << val));
			}
			else if(!(pos & RIGHTMASK))
			{/*all allowed val in right box*/
				change |= maskBoxExceptCol(board, col, 2, ~(1 << val));
			}
		}
	}
	return change;
}

int checkBoxes(struct Board *board)
{
	int box, val, change;
	unsigned int pos;
	change = 0;
	for(box = 0; box < 9; box++)
	{
		for(val = 0; val < 9; val++)
		{
			pos = getPosInBox(board, val, box);
			if(!(pos & LEFTMASK))
			{/*all allowed val in first row*/
				change |= maskRowExceptBox(board, box, (box / 3) + 0, ~(1 << val));
			}
			else if(!(pos & MIDMASK))
			{/*all allowed val in middle row*/
				change |= maskRowExceptBox(board, box, (box / 3) + 1, ~(1 << val));
			}
			else if(!(pos & RIGHTMASK))
			{/*all allowed val in bottom row*/
				change |= maskRowExceptBox(board, box, (box / 3) + 2, ~(1 << val));
			}
			if(!(pos & FIRSTMASK))
			{/*all allowed val in first column*/
				change |= maskColExceptBox(board, box, (box % 3) + 0, ~(1 << val));
			}
			else if(!(pos & SECONDMASK))
			{/*all allowed val in second column*/
				change |= maskColExceptBox(board, box, (box % 3) + 1, ~(1 << val));
			}
			else if(!(pos & THIRDMASK))
			{/*all allowed val in third column*/
				change |= maskColExceptBox(board, box, (box % 3) + 2, ~(1 << val));
			}
		}
	}
	return change;
}

int checkDone(struct Board *board)
{
	int row, col;
	for(row = 0; row < 9; row++)
	{
		for(col = 0; col < 9; col++)
		{
			if(1 != pop(board->cell[row][col]))
				return 0;
		}
	}
	return 1;
}

