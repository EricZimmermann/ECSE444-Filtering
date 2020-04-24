/*
Image struct prototype declarations
*/

#include <stdlib.h>
#include <stdio.h>


typedef struct Image{
	unsigned short size;
	float **data;
}Image;


typedef struct CImage{
	unsigned short size;
	float **re;
	float **im;
} CImage;

// allocate memory and zero init image
void initImage(struct Image *image, short size);

// allocate memory for complex img representation
struct CImage initComplex(struct CImage image, short size);

// zero out content of image
struct Image resetImage(struct Image image);

// zero out content of image
struct CImage resetCImage(struct CImage image);

// deallocate memory in image
void deinitImage(struct Image *image);

// deallocate memory for complex represenation
void deinitCImage(struct CImage image);

// load entire image into structure
struct Image loadImage(struct Image image, float *data_stream);

// load entire image into complex structure
struct CImage loadComplexImage(struct CImage image, float *data_stream);

// load a single row into img structure
struct Image loadRow(struct Image image, float *row_data, short index);

// load a single row into complex img structure
struct CImage loadComplexRow(struct CImage image, float *row_data, short index);
