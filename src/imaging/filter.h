/*
Kernal struct prototype declarations
*/

#include <stdlib.h>
#include <stdio.h>

// filter kernel
typedef struct Kernel{
    short size;
    float **data;
}Kernel;

// init and free
void initKernel(struct Kernel *kernel, short size);
void deinitKernel(struct Kernel *kernel);

// filter population
void generateHammingFilter(struct Kernel *kernel, float r);
void generateGaussianFilter(struct Kernel *kernel, float sigma);
