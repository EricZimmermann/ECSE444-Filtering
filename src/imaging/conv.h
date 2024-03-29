#ifndef CONV_H
#define CONV_H


#include "image.h"
#include "filter.h"

void convolve(struct Image *input, struct Image *output, struct Kernel *kernel);

#endif
