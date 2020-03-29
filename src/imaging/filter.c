/*
Gaussian kernel object
dynamic generation
*/

#include <stdlib.h>
#include <math.h>
# define PI 3.14

struct Kernel{
	uint8_t size;
	float **data;
}

void initKernel(struct Kernel kernel, uint8_t size){
	kernel.size = size;
	kernel.data = malloc(size * sizeof(float *));
	for(uint8_t row = 0; row < size; i++){
		kernel.data[row] = malloc(size * sizeof(float));
	}
}

void deinitKernel(struct Kernel kernel){
	for(uint8_t row = 0; row < kernel.size){
		free(kernel.data[row]);
	}
	free(kernel.data);
}

void generateHammingFilter(struct Kernel kernel, r){
    
    // hamming coeff cache
    float *band = (float *)malloc(kernel.size * sizeof(float));
    
    // generate 1D coeff
    for(uint16_t i = 0; i < kernel.size; ++i){
        band[i] = 0.54 - 0.46 * cosf((2* PI * i) / kernel.size)
    }
    
    // generate kernel matrix coeff
    for(uint16_t y = 0; y < kernel.size; ++y){
        for(uint16_t x = 0; x < kernel.size; ++x){
            kernel.data[y][x] = pow(sqrt(band[y] * band[x]), r);
        }
    }
    
    free(band);       
}

void generateGaussianFilter(struct Kernel kernel, sigma){    
	
	float sum = 0;
	uint8_t half_band = kernel.size / 2;
    
    for (uint8_t i = - half_band; i <= half_band; i++) {
        for (uint8_t j = - half_band; j <= half_band; j++) {
            kernel.data[i+half_band][j+half_band] = expf(-1*((pow(i, 2) + pow(j, 2)) / ((2 * pow(sigma, 2))))); // unnormalized guassian
            sum += kernel.data[i+half_band][j+half_band] // normalization pooling
        }
    }
    
    for (uint8_t i = - half_band; i <= half_band; i++) {
        for (uint8_t j = -half_band; j < half_band; j++) {
        	kernel.data[i+half_band][j+half_band] /= sum;
        }
    }
} 