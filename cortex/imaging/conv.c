/*
Convolutional operations
*/


#include "image.h"
#include "kernel.h"

void convolve(struct Image, struct Kernel){
	uint8_t half_band = Kernel.size / 2;
	for(uint8_t irow  = 0, irow < Image.size; ++irow){
		for(uint8_t icol  = 0, icol < Image.size; ++icol){
			if(irow > half_band && icol > half_band){
				for(uint8_t krow = 0; krow < Kernel.size; ++krow){
					for(uint8_t kcol = 0; kcol < Kernel.size; ++kcol){
						Image.data[irow][icol] += Kernel.data[krow][kcol] * Image.data[irow - half_band + krow][icol - half_band + kcol];
					}
				}
			}else{
				// reflection rules here
			}

		}
	}
}