/*
Convolutional operations
*/


#include "image.h"
#include "kernel.h"

void convolve(struct Image, struct Kernel){
	uint8_t half_band = Kernel.size / 2;
	
	// image iterables
	for(uint8_t irow  = 0, irow < Image.size; ++irow){
		for(uint8_t icol  = 0, icol < Image.size; ++icol){
			
			// kernel iterables
			for(uint8_t krow = 0; krow < Kernel.size; ++krow){
				for(uint8_t kcol = 0; kcol < Kernel.size; ++kcol){
					
					// image-kernel index mapping
					uint8_t rindex = irow - half_band + krow;
					uint8_t cindex = irow - half_band + krow
					
					// simulate padded borders
					if(rindex < 0){
						rindex = 0;
					}else if(rindex > Kernel.size -1){
						rindex = Kernel.size -1;
					}
					if(cindex < 0){
						cindex = 0;
					}else if(cindex > Kernel.size -1){
						cindex = Kernel.size -1;
					}

					// convolve 
					Image.data[irow][icol] += Kernel.data[krow][kcol] * Image.data[rindex][cindex];
				}
			}
		}
	}
}