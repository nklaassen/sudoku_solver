#include <stdio.h>
#include <errno.h>
#include <time.h>
#include "io.h"
#include "sudoku.h"

unsigned long long timespecDiff(struct timespec *timeA_p, struct timespec *timeB_p)
{/*functionTime for measuring the time difference between two timespecs, in nanoseconds*/
	return ((timeA_p->tv_sec * 1000000000) + timeA_p->tv_nsec) -
					((timeB_p->tv_sec * 1000000000) + timeB_p->tv_nsec);
}

int main(int argc, char *argv[])
{
	FILE *stream;
	struct Board board;
	struct timespec start, stop;

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
	clock_gettime(CLOCK_MONOTONIC, &start);
	init(&board);
	recursiveSolve(&board, 1);
	clock_gettime(CLOCK_MONOTONIC, &stop);
	printBoard(&board, stdout);
	printf("Time: %lld us\n", timespecDiff(&stop, &start) / 1000);

	fclose(stream);
	return 0;
}
