/*
Image struct prototype declarations
*/

#include <stdlib.h>
#include <stdio.h>

struct Image{
	unsigned short size;
	float **data;
};

struct CImage{
	unsigned short size;
	float **re;
	float **im;
};

// Public functions

// init and free
void initImage(struct Image image, unsigned short size);
void initComplex(struct CImage image, unsigned short size);
void deinitImage(struct Image image);
void deinitCImage(struct CImage image);

// reset
void resetImage(struct Image image);
void resetCImage(struct CImage image);

// loading
void loadImage(struct Image image, float *data_stream);
void loadCImage(struct CImage image, float *data_stream);
