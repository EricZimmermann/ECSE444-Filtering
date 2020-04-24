/*
Image objects
Loading
Saving
Reading

NOTE:
supports imgs of size max size unsigned short
*/

#include <stdlib.h>
#include "image.h"

// allocate memory and zero init image

struct Image initImage(struct Image image, short size){
	image.size = size;
	image.data = malloc(size * sizeof(float*));

	for(int row = 0; row < size; row++){
		image.data[row] = malloc(size * sizeof(float));
		for(int col = 0; col < image.size; col++){
			image.data[row][col] = 0;
		}
	}
	return image;
}

// allocate memory for complex img representation

struct CImage initComplex(struct CImage image, short size){
	image.size = size;
	image.re = malloc(size * sizeof(float *));
	image.im = malloc(size * sizeof(float *));
	for(short row = 0; row < size; row++){
		image.re[row] = malloc(size * sizeof(float));
		image.im[row] = malloc(size * sizeof(float));
		for(short col = 0; col < image.size; col++){
			image.re[row][col] = 0;
			image.im[row][col] = 0;
		}
	}
	return image;
}

// zero out content of image
struct Image resetImage(struct Image image){
	for(short row = 0; row < image.size; row++){
		for(short col = 0; col < image.size; col++){
			image.data[row][col] = 0;
		}
	}
	return image;
}

// zero out content of image
struct CImage resetCImage(struct CImage image){
	for(short row = 0; row < image.size; row++){
		for(short col = 0; col < image.size; col++){
			image.re[row][col] = 0;
			image.im[row][col] = 0;
		}
	}
	return image;
}

// deallocate memory in image
void deinitImage(struct Image image){
	for(short row = 0; row < image.size; row++){
		free(image.data[row]);
	}
	free(image.data);
}


// deallocate memory for complex represenation
void deinitCImage(struct CImage image){
	for(short row = 0; row < image.size; row++){
		free(image.re[row]);
		free(image.im[row]);
	}
	free(image.re);
	free(image.im);
}

// load entire image into structure
struct Image loadImage(struct Image image, float *data_stream){
	for(short row = 0; row < image.size; row++){
		for(short col = 0; col < image.size; col++){
			image.data[row][col] = data_stream[row + image.size * col];
		}
	}
	return image;
}

// load entire image into complex structure
struct CImage loadComplexImage(struct CImage image, float *data_stream){
	for(short row = 0; row < image.size; row++){
		for(short col = 0; col < image.size; col++){
			image.re[row][col] = data_stream[row + image.size * col];
		}
	}
	return image;
}

// load a single row into img structure
struct Image loadRow(struct Image image, float *row_data, short index){
	for(short col = 0; col < image.size; col++){
		image.data[index][col] = row_data[col];
	}
	return image;
}

// load a single row into complex img structure
struct CImage loadComplexRow(struct CImage image, float *row_data, short index){
	for(short col = 0; col < image.size; col++){
		image.re[index][col] = row_data[col];
	}
	return image;
}
