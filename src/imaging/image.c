/*
Image objects
Loading
Saving
Reading

NOTE:
supports imgs of size max size 128x128
*/

#include <stdlib.h>

// Image structures
struct Image{
	short size;
	float **data;
};

struct CImage{
	short size;
	float **re;
	float **im;
};

// allocate memory and zero init image
void initImage(struct Image image, short size){
	image.size = size;
	image.data = malloc(size * sizeof(float *));
	for(short row = 0; row < size; i++){
		image.data[row] = malloc(size * sizeof(float));
		for(short col = 0; col < image.size){
			image.data[row][col] = 0;
		}
	}
}

// allocate memory for complex img representation
void initComplex(struct CImage image, short size){
	image.size = size;
	image.re = malloc(size * sizeof(float *));
	image.im = malloc(size * sizeof(float *));
	for(short row = 0; row < size; i++){
		image.re[row] = malloc(size * sizeof(float));
		image.im[row] = malloc(size * sizeof(float));
		for(short col = 0; col < image.size){
			image.re[row][col] = 0;
			image.im[row][col] = 0;
		}
	}
}

// zero out content of image
void resetImage(struct Image image){
	for(short row = 0; row < image.size){
		for(short col = 0; col < image.size){
			image.data[row][col] = 0;
		}
	}
}

// zero out content of image
void resetImage(struct CImage image){
	for(short row = 0; row < image.size){
		for(short col = 0; col < image.size){
			image.re[row][col] = 0;
			image.im[row][col] = 0;
		}
	}
}

// deallocate memory in image
void deinitImage(struct Image image){
	for(short row = 0; row < image.size){
		free(image.data[row]);
	}
	free(image.data);
}


// deallocate memory for complex represenation
void deinitImage(struct CImage image){
	for(short row = 0; row < image.size){
		free(image.re[row]);
		free(image.im[row]);
	}
	free(image.re);
	free(image.im);
}

// load entire image into structure
void loadImage(struct Image image, float *data_stream){
	for(short row = 0; row < image.size){
		for(short col = 0; col < image.size){
			image.data[row][col] = data_stream[row + image.size * col];
		}
	}
}

// load entire image into complex structure
void loadComplexImage(struct CImage image, float *data_stream){
	for(short row = 0; row < image.size){
		for(short col = 0; col < image.size){
			image.re[row][col] = data_stream[row + image.size * col];
		}
	}
}

// load a single row into img structure
void loadRow(struct Image image, float *row_data, short index){
	for(short col = 0; col < image.size; ++column){
		image.data[index][column] = row_data[column];
	}
}

// load a single row into complex img structure
void loadComplexRow(struct CImage image, float *row_data, short index){
	for(short col = 0; col < image.size; ++column){
		image.re[index][column] = row_data[column];
	}
}