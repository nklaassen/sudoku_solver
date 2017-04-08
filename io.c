#include "io.h"

int parseInput(struct Board *board, FILE *stream)
{
	int i, j;
	char *buf, *cellstr;
	long cell;
	size_t size;
	ssize_t numChars;
	for(i = 0; i < 9; i++)
	{
		numChars = getline(&buf, &size, stream);
		if(-1 == numChars) {
			return -1;
		}
		cellstr = strtok(buf, ",\n");
		if(!cellstr) {
			return -1;
		}
		cell = strtol(cellstr, NULL, 10);
		if(cell < 0 || cell > 9) {
			return -1;
		}
		j = 0;
		setCell(board, cell, i, j);
		for(j = 1; j < 9; j++)
		{
			cellstr = strtok(NULL, ",\n");
			if(!cellstr) {
				return -1;
			}
			cell = strtol(cellstr, NULL, 10);
			if(cell < 0 || cell > 9) {
				return -1;
			}
			setCell(board, cell, i, j);
		}
	}
	return 0;
}

int printBoard(struct Board *board, FILE *stream)
{
	int i, j;
	unsigned int cell;
	for(i = 0; i < 9; i++)
	{
		for(j = 0; j < 9; j++)
		{
			getCell(board, &cell, i, j);
			fprintf(stream, "%d", cell);
			if(j != 8) {
				fprintf(stream, ",");
			}
		}
		fprintf(stream, "\n");
	}
	return 0;
}
