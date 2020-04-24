/*
Kernal struct prototype declarations
*/

#include <stdlib.h>
#include <stdio.h>

// filter kernel
typedef struct Kernel{
    unsigned short size;
    float **data;
};

// init and free
struct Kernel initKernel(struct Kernel kernel, unsigned short size);
void deinitKernel(struct Kernel kernel);

// filter population
struct Kernel generateHammingFilter(struct Kernel kernel, float r);
struct Kernel generateGaussianFilter(struct Kernel kernel, float sigma);
