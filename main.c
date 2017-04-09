#include <stdio.h>
#include <errno.h>
#include "io.h"
#include "sudoku.h"

int main(int argc, char *argv[])
{
	FILE *stream;
	struct Board board;

	if(argc > 1) {
		stream = fopen(argv[1], "r");
		if(stream == NULL) {
			perror("fopen");
			return -1;
		}
	}
	else {
		stream = stdin;
	}

	parseInput(&board, stream);
	init(&board);
	recursiveSolve(&board, 1);
	printBoard(&board, stdout);

	return 0;
}
