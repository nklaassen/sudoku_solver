#include <stdio.h>
#include "io.h"
#include "sudoku.h"

int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;

	struct Board board;
	parseInput(&board, stdin);
	solve(&board);
	//printBoard(&board, stdout);

	return 0;
}
