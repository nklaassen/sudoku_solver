#include <stdio.h>
#include "io.h"
#include "sudoku.h"

int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;

	struct Board board;
	parseInput(&board, stdin);
	if(recursiveSolve(&board) == 1) {
		printf("Solved\n");
	}
	else {
		printf("Failed\n");
	}
	printBoard(&board, stdout);

	return 0;
}
