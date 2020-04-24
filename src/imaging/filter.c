/*
Gaussian kernel object
dynamic generation
*/

#include <stdlib.h>
#include <math.h>
#include "filter.h"
#define PI 3.14159265

void initKernel(struct Kernel *kernel, short size){
	kernel->data = malloc(kernel->size * sizeof(float *));
	for(short row = 0; row < kernel->size; ++row){
		kernel->data[row] = malloc(kernel->size * sizeof(float));
	}
}

void deinitKernel(struct Kernel *kernel){
    for(short row = 0; row < kernel->size; ++row){
		free(kernel->data[row]);
	}
	free(kernel->data);
}

void generateHammingFilter(struct Kernel *kernel, float r){
    
    // hamming coeff cache
    float *band = (float *)malloc(kernel->size * sizeof(float));
    
    // generate 1D coeff
    for(short i = 0; i < kernel->size; ++i){
        band[i] = 0.54 - 0.46 * cosf((2* PI * i) / kernel->size);
    }
    
    // generate kernel matrix coeff
    for(short y = 0; y < kernel->size; ++y){
        for(short x = 0; x < kernel->size; ++x){
            kernel->data[y][x] = pow(sqrt(band[y] * band[x]), r);
        }
    }
    
    free(band);     
}

void generateGaussianFilter(struct Kernel *kernel, float sigma){
	
	float sum = 0;
	short half_band = kernel->size >> 1;
    
    for (short i = - half_band; i <= half_band; ++i) {
        for (short j = - half_band; j <= half_band; ++j) {
            kernel->data[i+half_band][j+half_band] = expf(-1*((pow(i, 2) + pow(j, 2)) / ((2 * pow(sigma, 2))))); // unnormalized guassian
            sum += kernel->data[i+half_band][j+half_band]; // normalization pooling
        }
    }
    
    for (short i = - half_band; i <= half_band; ++i) {
        for (short j = -half_band; j < half_band; ++j) {
        	kernel->data[i+half_band][j+half_band] /= sum;
        }
    }
} 
