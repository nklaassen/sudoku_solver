#include "io.h"
#include "sudoku.h"

int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;
	struct Board board;
	int i, j;
	unsigned int pos;
	for (i = 0; i < 9; i++)
	{
		for(j = 0; j < 9; j++)
		{
			setCell(&board, j + 1, i, j);
		}
	}

	printBoard(&board, stdout);

	for (i = 0; i < 9; i++)
	{
		for(j = 0; j < 9; j++)
		{
			pos = getPosInRow(&board, j, i);
			if(pos != (unsigned int)1 << (8 - j)) {
				printf("getPosInRow error at row %d, value %d\n", i, j+1);
			}
		}
	}

	setCell(&board, 9, 0, 7);
	if(getPosInRow(&board, 8, 0) != 3) {
		printf("getPosInRow error");
	}

	return 0;
}
