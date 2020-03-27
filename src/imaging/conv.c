/*
Convolutional operations
*/

#include "image.h"
#include "kernel.h"

void convolve(struct Image input, struct Image output, struct Kernel kernel){
	

	uint8_t irow, icol;
	uint8_t krow, kcol;
	uint8_t half_band = kernel.size / 2;
	int rindex;
	int cindex

	// image iterables
	for(irow  = 0, irow < input.size; ++irow){
		for(  = 0, icol < input.size; ++icol){
	
			// kernel iterables
			for(krow = 0; krow < kernel.size; ++krow){
				for(kcol = 0; kcol < kernel.size; ++kcol){
					
					// image-kernel index mapping ~ must be signed 
					rindex = irow + krow - half_band;
					cindex = icol + kcol - half_band;
					
					// simulate padded borders
					if(rindex < 0){
						rindex = 0;
					}else if(rindex > input.size -1){
						rindex = input.size -1;
					}
					if(cindex < 0){
						cindex = 0;
					}else if(cindex > input.size -1){
						cindex = input.size -1;
					}

					// convolve 
					output.data[irow][icol] += kernel.data[krow][kcol] * input.data[rindex][cindex];
				}
			}
		}
	}
}