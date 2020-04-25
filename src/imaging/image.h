#ifndef IMAGE_H
#define IMAGE_H

/*
Image struct prototype declarations
*/

#include <stdlib.h>
#include <stdio.h>


typedef struct Image{
	short size;
	float **data;
}Image;


typedef struct CImage{
	short size;
	float **re;
	float **im;
} CImage;

// allocate memory and zero init image
void initImage(struct Image *image, short size);

// allocate memory for complex img representation
void initComplex(struct CImage *image, short size);

// zero out content of image
void resetImage(struct Image *image);

// zero out content of image
void resetCImage(struct CImage *image);

// deallocate memory in image
void deinitImage(struct Image *image);

// deallocate memory for complex represenation
void deinitCImage(struct CImage *image);

// load entire image into structure
void loadImage(struct Image *image, float *data_stream);

// load entire image into complex structure
void loadComplexImage(struct CImage *image, float *data_stream);

// load a single row into img structure
void loadRow(struct Image *image, float *row_data, short index);

// load a single row into complex img structure
void loadComplexRow(struct CImage *image, float *row_data, short index);

#endif