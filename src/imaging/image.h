/*
Image struct prototype declarations
*/

#include <stdlib.h>
#include <stdio.h>

struct Image{
	short size;
	float **data;
};

struct CImage{
	short size;
	float **re;
	float **im;
};
