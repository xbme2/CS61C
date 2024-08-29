/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"
// Opens a .ppm P3 image file, and constructs an Image object.
// You may find the function fscanf useful.
// Make sure that you close the file with fclose before returning.
char p3[6];
#define MAX_SIZE 255
Image *readData(char *filename)
{
	FILE *fp = fopen(filename, "r");

	int scale = 0;
	if (!fp)
	{
		fprintf(stderr, "can not open the file!");
		exit(1);
	}
	Image *newImage = (Image *)malloc(sizeof(struct Image));
	assert(newImage);
	fscanf(fp, "%s", p3);
	fscanf(fp, "%u %u", &newImage->cols, &newImage->rows); // fscanf reads pointer !
	fscanf(fp, "%d", &scale);
	newImage->image = malloc(sizeof(struct Color *) * newImage->cols * newImage->rows);
	assert(newImage->image);
	for (size_t i = 0; i < newImage->rows; i++)
	{
		for (size_t j = 0; j < newImage->cols; j++)
		{
			//wrong answer:
			//Color *currentColor = newImage->image[i * (newImage->rows) + j];#(= NULL )
			//currentColor = malloc(sizeof(Color)); #only malloc currentColor not newImage->image[i * (newImage->rows) + j]
			//assert(currentColor &&newImage->image[i * (newImage->rows) + j]); #   not pass! 
			newImage->image[i * (newImage->rows) + j] = malloc(sizeof(Color));
			Color *currentColor = newImage->image[i * (newImage->rows) + j];
			fscanf(fp, "%hhu %hhu %hhu", &currentColor->R, &currentColor->G, &currentColor->B);
		}
	}
	fclose(fp);
	return newImage;
}

// Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	// YOUR CODE HERE
	fprintf(stdout, "%s\n", p3);
	fprintf(stdout, "%d %d\n", image->rows, image->cols);
	fprintf(stdout, "%d\n", MAX_SIZE);
	assert(image->cols && image->rows && image->image);
	for (size_t i = 0; i < image->rows; i++)
	{
		for (size_t j = 0; j < image->cols; j++)
		{
			Color *currentColor = image->image[i * (image->rows) + j];
			assert(currentColor);
			fprintf(stdout, "%3hhu %3hhu %3hhu%s", 
							currentColor->R, 
							currentColor->G, 
							currentColor->B,
							(j == image->cols-1)?"":"   "); 
			
		}
		fprintf(stdout, "\n");
	}

}

// Frees an image
void freeImage(Image *image)
{
	for (size_t i = 0; i < image->rows; i++)
	{
		for (size_t j = 0; j < image->cols; j++)
		{
			free(image->image[i * image->rows + j]);
		}
	}
	free(image->image);
	free(image);
}