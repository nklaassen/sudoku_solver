#include "sudoku.h"

#define pop(value) __builtin_popcount(value)
#define LEFTMASK 0x3F
#define MIDMASK 0x1C7
#define RIGHTMASK 0x1F8
#define FIRSTMASK 0xDB
#define SECONDMASK 0x16D
#define THIRDMASK 0x1B6

int solve(struct Board *board)
{
	int i = 0, change;
	printBoard(board, stdout);
	printf("masking\n");
	mask(board);
	checkDone(board);
	printBoard(board, stdout);
	do
	{
		change = 0;
		printf("checking rows\n");
		change |= checkRows(board);
		printf("checking columns\n");
		change |= checkCols(board);
		printf("checking boxes\n");
		change |= checkBoxes(board);
		i++;
	} while (change);
	return checkDone(board);
}

int recursiveSolve(struct Board *board)
{
	struct Board copy;
	int row, col;
	unsigned int temp;
	while(1)
	{/*loop on all possible guesses until one works*/
		switch(solve(board))
		{
			case -1:/*invalid board*/
				return 0;
			case 1:/*solved*/
				return 1;
		}
		/*unable to solve by traditional means...*/
		printf("making a guess\n");
		/*make a copy of the board to revert to if guess is wrong*/
		memcpy(&copy, board, sizeof(struct Board));
		/*make a guess*/
		getFirstUndecided(board, &row, &col);
		temp = board->cell[row][col] = guess(board->cell[row][col]);
		printf("guessing at row %d and col %d", row, col);
		if(recursiveSolve(board))
		{/*guess was right, return*/
			return 1;
		}
		else
		{/*guess was wrong, mask it out and continue*/
			memcpy(board, &copy, sizeof(struct Board));
			board->cell[row][col] &= ~temp;
		}
	}
	return 1;
}

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
			if(!(pos & LEFTMASK) && (pos & ~LEFTMASK))
			{/*all allowed val in left box*/
				change |= maskBoxExceptRow(board, row, 0, ~(1 << val));
			}
			else if(!(pos & MIDMASK) && (pos & ~MIDMASK))
			{/*all allowed val in middle box*/
				change |= maskBoxExceptRow(board, row, 1, ~(1 << val));
			}
			else if(!(pos & RIGHTMASK) && (pos & ~RIGHTMASK))
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
			if(!(pos & LEFTMASK) && (pos & ~LEFTMASK))
			{/*all allowed val in top box*/
				change |= maskBoxExceptCol(board, col, 0, ~(1 << val));
			}
			else if(!(pos & MIDMASK) && (pos & ~MIDMASK))
			{/*all allowed val in middle box*/
				change |= maskBoxExceptCol(board, col, 1, ~(1 << val));
			}
			else if(!(pos & RIGHTMASK) && (pos & ~RIGHTMASK))
			{/*all allowed val in middle box*/
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
			if(!(pos & LEFTMASK) && (pos & ~LEFTMASK))
			{/*all allowed val in first row*/
				change |= maskRowExceptBox(board, box, 3 * (box / 3) + 0, ~(1 << val));
			}
			else if(!(pos & MIDMASK) && (pos & ~MIDMASK))
			{/*all allowed val in middle row*/
				change |= maskRowExceptBox(board, box, 3 * (box / 3) + 1, ~(1 << val));
			}
			else if(!(pos & RIGHTMASK) && (pos & ~RIGHTMASK))
			{/*all allowed val in bottom row*/
				change |= maskRowExceptBox(board, box, 3 * (box / 3) + 2, ~(1 << val));
			}
			if(!(pos & FIRSTMASK) && (pos & ~FIRSTMASK))
			{/*all allowed val in first column*/
				change |= maskColExceptBox(board, box, 3 * (box % 3) + 0, ~(1 << val));
			}
			else if(!(pos & SECONDMASK) && (pos & ~SECONDMASK))
			{/*all allowed val in second column*/
				change |= maskColExceptBox(board, box, 3 * (box % 3) + 1, ~(1 << val));
			}
			else if(!(pos & THIRDMASK) && (pos & ~THIRDMASK))
			{/*all allowed val in third column*/
				change |= maskColExceptBox(board, box, 3 * (box % 3) + 2, ~(1 << val));
			}
		}
	}
	return change;
}

/*1 = done, 0 = incomplete, -1 = invalid*/
int checkDone(struct Board *board)
{
	int row, col, done;
	unsigned int val, cell;
	unsigned int rows[9] = {0}, cols[9] = {0}, boxes[9] = {0};
	done = 1;
	for(row = 0; row < 9; row++)
	{
		for(col = 0; col < 9; col++)
		{
			val = board->cell[row][col];
			if(rows[row] & val) {
				printf("error at row %d col %d\n", row, col);
				printf("mask is %x, should not contain %x\n", val, rows[row] & val);
				return -1;
			}
			if(cols[col] & val) {
				printf("error at row %d col %d\n", row, col);
				printf("mask is %x, should not contain %x\n", val, cols[col] & val);
				return -1;
			}
			if(boxes[3 * (row / 3) + (col / 3)] & val) {
				getCell(board, &cell, row, col);
				printf("error at row %d col %d\n", row, col);
				printf("too many %d in box %d\n", cell, 3 * (row / 3) + (col / 3));
				return -1;
			}
			switch(pop(board->cell[row][col]))
			{
				case 0:
					printf("no allowed values for cell %d,%d\n", row, col);
					return -1;
				case 1:
					rows[row] |= val;
					cols[col] |= val;
					boxes[3 * (row / 3) + (col / 3)] |= val;
					break;
				default:
					done = 0;
			}
		}
	}
	return done;
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
			{/*if this cell is decided, mask out all relevent cells*/
				mask = ~(board->cell[row][col]);
				for(i = 0; i < 9; i++)
				{/*mask row and column*/
					if(i != row) {
						recursiveMask(board, i, col, mask);
					}
					if(i != col) {
						recursiveMask(board, row, i, mask);
					}
				}
				/*mask other 4 cells in box*/
				otherLinesInBox(row, &row1, &row2);
				otherLinesInBox(col, &col1, &col2);
				recursiveMask(board, row1, col1, mask);
				recursiveMask(board, row1, col2, mask);
				recursiveMask(board, row2, col1, mask);
				recursiveMask(board, row2, col2, mask);
			}
		}
	}
	return 0;
}

int recursiveMask(struct Board *board, int row, int col, unsigned int mask)
{
	int i, change, row1, col1, row2, col2;
	change = 0;
	if(pop(board->cell[row][col]) > 1)
	{
		change = board->cell[row][col] & ~mask;/*return non-zero if change*/
		board->cell[row][col] &= mask;
		if(pop(board->cell[row][col]) == 1)
		{/*if cell is now decided, recurse on all affected cells*/
			
			printBoard(board, stdout);
			printf("\n\n\n");

			mask = ~(board->cell[row][col]);
			for(i = 0; i < 9; i++)
			{/*mask row and column*/
				if(i != row) {
					recursiveMask(board, i, col, mask);
				}
				if(i != col) {
					recursiveMask(board, row, i, mask);
				}
			}
			/*mask other 4 cells in box*/
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

int maskBoxExceptRow(struct Board *board, int row, int box, unsigned int mask)
{/*box is relative to the row, in [0,2]*/
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
{/*box is relative to the column, in [0,2]*/
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
{/*box is left->right, top->bottom, in [0,8]*/
	int col, change;
	change = 0;
	for(col = 0; col < 9; col++)
	{
		if((col / 3) != (box % 3)) {
			change |= recursiveMask(board, row, col, mask);
		}
	}
	return change;
}

int maskColExceptBox(struct Board *board, int box, int col, unsigned int mask)
{/*box is left->right, top->bottom, in [0,8]*/
	int row, change;
	change = 0;
	for(row = 0; row < 9; row++)
	{
		if((row / 3) != (box / 3)) {
			change |= recursiveMask(board, row, col, mask);
		}
	}
	return change;
}

unsigned int getPosInRow(struct Board *board, int val, int row)
{/*returns bit vector of possible positions in row for val*/
	unsigned int col, pos;
	pos = 0;
	for(col = 0; col < 9; col++)
	{
		pos |= ((board->cell[row][col] >> val) & 1) << (8 - col);
	}
	return pos;
}

unsigned int getPosInCol(struct Board *board, int val, int col)
{/*returns bit vector of possible positions in col for val*/
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
{/*returns bit vector of possible positions in box for val*/
	int i, row, col;
	unsigned int pos;
	pos = 0;
	for(i = 0; i < 9; i++)
	{
		row = 3*(box / 3) + (i / 3);
		col = 3*(box % 3) + (i % 3);
		pos |= ((board->cell[row][col] >> val) & 1) << (8 - i);
	}
	return pos;
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

int getFirstUndecided(struct Board *board, int *row, int *col)
{
	for((*row) = 0; (*row) < 9; (*row)++)
	{
		for((*col) = 0; (*col) < 9; (*col)++)
		{
			if(board->cell[(*row)][(*col)] != 1) {
				return 1;
			}
		}
	}
	return 0;
}

unsigned int guess(unsigned int val)
{
	unsigned int guess = 1;
	while(!(guess & val)) {
		guess <<= 1;
	}
	return guess;
}
