/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"
#define MASK 0b00000001
// Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	// YOUR CODE HERE
	assert(image && row >= 0 && col >= 0);
	assert(image->image[row * image->cols + col]);
	Color *currentCOlor = image->image[row * image->cols + col];
	Color *newColor = malloc(sizeof(Color));
	if (currentCOlor->B & MASK)
	{
		free(currentCOlor);
		*newColor = (Color){
			.R = 255,
			.G = 255,
			.B = 255};
	}
	else
	{
		free(currentCOlor);
		*newColor = (Color){
			.R = 0,
			.G = 0,
			.B = 0};
	}
	return newColor;
}

// Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	// YOUR CODE HERE
	assert(image && image->image);
	Image *newImage = (Image *)malloc(sizeof(Image));
	newImage->cols = image->cols;
	newImage->rows = image->rows;
	newImage->image = (Color **)malloc(sizeof(Color *) * (image->rows) * (image->cols));
	for (int i = 0; i < newImage->rows; i++)
	{
		for (int j = 0; j < newImage->cols; j++)
		{
			newImage->image[i * newImage->cols + j] = evaluateOnePixel(image, i, j);
		}
	}
	return newImage;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image,
where each pixel is black if the LSB of the B channel is 0,
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
void processCLI(int argc, char **argv, char **filename)
{
	if (argc != 2)
	{
		printf("usage: %s filename\n", argv[0]);
		printf("filename is an ASCII PPM file (type P3) with maximum value 255.\n");
		exit(-1);
	}
	*filename = argv[1];
}
int main(int argc, char **argv)
{
	// YOUR CODE HERE
	Image *image;
	// uint32_t rule;
	char *filename;
	processCLI(argc, argv, &filename);
	image = readData(filename);
	image = steganography(image);
	writeData(image);
	freeImage(image);
}
