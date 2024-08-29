/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

int dx[] = {0, 0, 1, 1, 1, -1, -1, -1};
int dy[] = {1, -1, 1, -1, 0, 1, -1, 0};

// Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
// Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
// and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	Color *next = malloc(sizeof(Color));
	int rNearNeighbourNum = 0, gNearNeighbourNum = 0, bNearNeighbourNum = 0;
	Color *current = image->image[row * image->cols + col];
	assert(current);
	int rAlive = current->R == 255;
	int gAlive = current->G == 255;
	int bAlive = current->B == 255;

	for (size_t i = 0; i < 8; i++)
	{
		int newrow = (row + dx[i]) % image->rows;
		int newcol = (col + dy[i]) % image->cols;

		if (image->image[newrow * image->cols + newcol]->R == 255)
			rNearNeighbourNum++;
		if (image->image[newrow * image->cols + newcol]->G == 255)
			gNearNeighbourNum++;
		if (image->image[newrow * image->cols + newcol]->B == 255)
			bNearNeighbourNum++;
	}
	int rShift = 9 * rAlive + rNearNeighbourNum;
	int gShift = 9 * gAlive + gNearNeighbourNum;
	int bShift = 9 * bAlive + bNearNeighbourNum;

	if (rule & 1 << rShift)
		next->R = 255;
	else
		next->R = 0;
	if (rule & 1 << gShift)
		next->G = 255;
	else
		next->G = 0;
	if (rule & 1 << bShift)
		next->B = 255;
	else
		next->B = 0;
	return next;
}

// The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
// You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	// YOUR CODE HERE
	assert(image && image->image);
	Image *nextImage = (Image *)malloc(sizeof(Image));
	nextImage->cols = image->cols;
	nextImage->rows = image->rows;
	nextImage->image = (Color **)malloc(sizeof(Color *) * (image->rows) * (image->cols));
	for (int i = 0; i < nextImage->rows; i++)
	{
		for (int j = 0; j < nextImage->cols; j++)
		{
			nextImage->image[i * nextImage->cols + j] = evaluateOneCell(image, i, j, rule);
		}
	}
	return nextImage;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
void processCLI(int argc, char **argv, char **filename, uint32_t *rule)
{
	if (argc != 3)
	{
		printf("usage: ./gameOfLife filename rule\nfilename is an ASCII PPM file (type P3) with maximum value 255.\nrule is a hex number beginning with 0x; Life is 0x1808.\n");
		exit(-1);
	}
	*filename = argv[1];
	*rule = strtol(argv[2], NULL, 16);
}
int main(int argc, char **argv)
{
	// YOUR CODE HERE
	Image *image,*nextimage;
	uint32_t rule;
	char *filename;
	processCLI(argc, argv, &filename, &rule);
	image = readData(filename);
	nextimage = life(image,rule);
	writeData(nextimage);
	freeImage(image);
	freeImage(nextimage);
	return 0;
}
