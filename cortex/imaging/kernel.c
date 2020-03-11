/*
Gaussian kernel object
dynamic generation
*/

#include <stdlib.h>
#include <math.h>

struct Kernel{
	uint8_t size;
	float **data;
}

Kernel initKernel(struct Kernel, uint8_t size){
	Kernel.size = size;
	Kernel.data = malloc(size * sizeof(float *));
	for(uint8_t row = 0; row < size; i++){
		Kernel.data[row] = malloc(size * sizeof(float));
	}
}

void deinitKernel(struct Kernel){
	for(uint8_t row = 0; row < Image.size){
		free(Kernel.data[row]);
	}
	free(Kernel.data);
}

void generateFilter(struct Kernel, sigma){    
	float sum = 0;
	uint8_t half_band = Kernel.size / 2;
    for (uint8_t i = - half_band; i <= half_band; i++) {
        for (uint8_t j = - half_band; j <= half_band; j++) {
            Kernel.data[i+half_band][j+half_band]= exp(-1*((pow(i, 2) + pow(j, 2)) / ((2 * pow(sigma, 2))))); // unnormalized guassian
            sum += Kernel.data[i+half_band][j+half_band] // normalization pooling
        }
    }
    
    for (uint8_t i = - half_band; i <= half_band; i++) {
        for (uint8_t j = -half_band; j < half_band; j++) {
        	Kernel.data[i+half_band][j+half_band] /= sum;
        }
    }
} 