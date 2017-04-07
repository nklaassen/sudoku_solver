#include "io.h"

int parseInput(struct Board *board, FILE *stream)
{
	int i, j;
	char *buf, *nodestr;
	long node;
	size_t size;
	ssize_t numChars;
	for(i = 0; i < 9; i++)
	{
		numChars = getline(&buf, &size, stream);
		if(-1 == numChars) {
			return -1;
		}
		nodestr = strtok(buf, ",\n");
		if(!nodestr) {
			return -1;
		}
		node = strtol(nodestr, NULL, 10);
		if(node < 0 || node > 9) {
			return -1;
		}
		j = 0;
		setNode(board, node, i, j);
		for(j = 1; j < 9; j++)
		{
			nodestr = strtok(NULL, ",\n");
			if(!nodestr) {
				return -1;
			}
			node = strtol(nodestr, NULL, 10);
			if(node < 0 || node > 9) {
				return -1;
			}
			setNode(board, node, i, j);
		}
	}
	return 0;
}

int printBoard(struct Board *board, FILE *stream)
{
	int i, j;
	unsigned int node;
	for(i = 0; i < 9; i++)
	{
		for(j = 0; j < 9; j++)
		{
			getNode(board, &node, i, j);
			fprintf(stream, "%d", node);
		}
		fprintf(stream, "\n");
	}
	return 0;
}
