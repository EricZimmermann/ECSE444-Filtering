/*
Kernal struct prototype declarations
*/

#include <stdlib.h>
#include <stdio.h>

// filter kernel
struct Kernel{
    unsigned short size;
    float **data;
};

// init and free
void initKernel(struct Kernel kernel, unsigned short size);
void deinitKernel(struct Kernel kernel);

// filter population
void generateHammingFilter(struct Kernel kernel, float r);
void generateGaussianFilter(struct Kernel kernel, float sigma);
