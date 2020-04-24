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
void initImage(struct Image image, unsigned short size){
	image.size = size;
	image.data = malloc(size * sizeof(float *));
	for(unsigned short row = 0; row < size; ++row){
		image.data[row] = malloc(size * sizeof(float));
        for(unsigned short col = 0; col < image.size; ++col){
			image.data[row][col] = 0;
		}
	}
}

// allocate memory for complex img representation
void initComplex(struct CImage image, unsigned short size){
	image.size = size;
	image.re = malloc(size * sizeof(float *));
	image.im = malloc(size * sizeof(float *));
	for(unsigned short row = 0; row < size; ++row){
		image.re[row] = malloc(size * sizeof(float));
		image.im[row] = malloc(size * sizeof(float));
        for(unsigned short col = 0; col < image.size; ++col){
			image.re[row][col] = 0;
			image.im[row][col] = 0;
		}
	}
}

// zero out content of image
void resetImage(struct Image image){
    for(unsigned short row = 0; row < image.size; ++row){
        for(unsigned short col = 0; col < image.size; ++row){
			image.data[row][col] = 0;
		}
	}
}

// zero out content of image
void resetCImage(struct CImage image){
    for(unsigned short row = 0; row < image.size; ++row){
        for(unsigned short col = 0; col < image.size; ++col){
			image.re[row][col] = 0;
			image.im[row][col] = 0;
		}
	}
}

// deallocate memory in image
void deinitImage(struct Image image){
    for(unsigned short row = 0; row < image.size; ++row){
		free(image.data[row]);
	}
	free(image.data);
}


// deallocate memory for complex represenation
void deinitCImage(struct CImage image){
    for(unsigned short row = 0; row < image.size; ++row){
		free(image.re[row]);
		free(image.im[row]);
	}
	free(image.re);
	free(image.im);
}

// load entire image into structure
void loadImage(struct Image image, float *data_stream){
    for(unsigned short row = 0; row < image.size; ++row){
        for(unsigned short col = 0; col < image.size; ++row){
			image.data[row][col] = data_stream[row + image.size * col];
		}
	}
}

// load entire image into complex structure
void loadCImage(struct CImage image, float *data_stream){
    for(short row = 0; row < image.size; ++row){
        for(short col = 0; col < image.size; ++col){
			image.re[row][col] = data_stream[row + image.size * col];
		}
	}
}

// load a single row into img structure
void loadRow(struct Image image, float *row_data, unsigned short index){
	for(unsigned short col = 0; col < image.size; ++col){
		image.data[index][col] = row_data[col];
	}
}

// load a single row into complex img structure
void loadComplexRow(struct CImage image, float *row_data, unsigned short index){
	for(unsigned short col = 0; col < image.size; ++col){
		image.re[index][col] = row_data[col];
	}
}
