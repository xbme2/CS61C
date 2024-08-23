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

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
char p3[6];
Image *readData(char *filename) 
{
	FILE *fp = fopen(filename,"r");
	
	int scale = 0;
	if(!fp){
		fprintf(stderr,"can not open the file!");
		exit(1);
	}
	Image *newImage = (Image *)malloc(sizeof(struct Image));
	fscanf(fp,"%s",p3);
	fscanf(fp,"%u %u",newImage->cols,newImage->rows);
	fscanf(fp,"%d",scale);
	newImage->image = malloc(sizeof(struct Color *)* newImage->cols *newImage->rows);
	for (size_t i = 0; i < newImage->rows; i++)
	{
		for (size_t j = 0; j < newImage->cols; j++)
		{
			newImage->image[i*newImage->rows+j] = malloc(sizeof( Color));
			fscanf(fp,"%d %d %d",newImage->image[i*newImage->rows+j]->R,newImage->image[i*newImage->rows+j]->G,newImage->image[i*newImage->rows+j]->B);
		}
		// fscanf(fp)
	}
	fclose(fp);

	
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	//YOUR CODE HERE
	fprintf(stdout,"%s",p3);
}

//Frees an image
void freeImage(Image *image)
{
	//YOUR CODE HERE
}