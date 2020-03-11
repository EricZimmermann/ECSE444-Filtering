/*
Image object
Loading
Saving
Reading

NOTE:
supports imgs of size max size 256x256
*/

#include <stdlib.h>

/*
 TODO:
 init that returns image?
 free that returns image?
 save image to flash
 get image from flash
 */


// Image structures
struct Image{
	uint8_t size;
	float **data;
}

// allocate memory in image
void initImage(struct Image, uint8_t size){
	Image.size = size;
	Image.data = malloc(size * sizeof(float *));
	for(uint8_t row = 0; row < size; i++){
		Image.data[row] = malloc(size * sizeof(float));
	}
}

// deallocate memory in image
void deinitImage(struct Image){
	for(uint8_t row = 0; row < Image.size){
		free(Image.data[row]);
	}
	free(Image.data);
}

// load entire image into structure
void loadImage(struct Image, float *data_stream){
	for(uint8_t row = 0; row < Image.size){
		for(uint8_t col = 0; col < Image.size){
			Image.data[row][col] = data_stream[row + Image.size * col];
		}
	}
}

// load a single row into img structure
void loadRow(struct Image, float *row_data, uint8_t index){
	for(uint8_t col = 0; col < Image.size; ++column){
		Image.data[index][column] = row_data[column];
	}
}