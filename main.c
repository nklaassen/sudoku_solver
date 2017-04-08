#include <stdio.h>
#include "io.h"
#include "sudoku.h"

int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;

	struct Board board;
	parseInput(&board, stdin);
	if(solve(&board)) {
		printBoard(&board, stdout);
	}
	else {
		printf("Unable to solve");
	}

	return 0;
}
