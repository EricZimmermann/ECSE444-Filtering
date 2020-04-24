/*
Gaussian kernel object
dynamic generation
*/

#include <stdlib.h>
#include <math.h>
#include "filter.h"
#define PI 3.14159265

struct Kernel initKernel(struct Kernel kernel, unsigned short size){
	kernel.data = malloc(kernel.size * sizeof(float *));
	for(unsigned short row = 0; row < kernel.size; ++row){
		kernel.data[row] = malloc(kernel.size * sizeof(float));
	}
    return kernel;
}

void deinitKernel(struct Kernel kernel){
    for(unsigned short row = 0; row < kernel.size; ++row){
		free(kernel.data[row]);
	}
	free(kernel.data);
}

struct Kernel generateHammingFilter(struct Kernel kernel, float r){
    
    // hamming coeff cache
    float *band = (float *)malloc(kernel.size * sizeof(float));
    
    // generate 1D coeff
    for(unsigned short i = 0; i < kernel.size; ++i){
        band[i] = 0.54 - 0.46 * cosf((2* PI * i) / kernel.size);
    }
    
    // generate kernel matrix coeff
    for(unsigned short y = 0; y < kernel.size; ++y){
        for(unsigned short x = 0; x < kernel.size; ++x){
            kernel.data[y][x] = pow(sqrt(band[y] * band[x]), r);
        }
    }
    
    free(band);
    return kernel;       
}

struct Kernel generateGaussianFilter(struct Kernel kernel, float sigma){
	
	float sum = 0;
	unsigned short half_band = kernel.size >> 1;
    
    for (unsigned short i = - half_band; i <= half_band; ++i) {
        for (unsigned short j = - half_band; j <= half_band; ++j) {
            kernel.data[i+half_band][j+half_band] = expf(-1*((pow(i, 2) + pow(j, 2)) / ((2 * pow(sigma, 2))))); // unnormalized guassian
            sum += kernel.data[i+half_band][j+half_band]; // normalization pooling
        }
    }
    
    for (unsigned short i = - half_band; i <= half_band; ++i) {
        for (unsigned short j = -half_band; j < half_band; ++j) {
        	kernel.data[i+half_band][j+half_band] /= sum;
        }
    }
    return kernel;
} 
