/*
Convolutional operations
*/

#include "image.h"
#include "kernel.h"

void convolve(struct Image input, struct Image output, struct Kernel kernel){
	
	// image iterables
	for(uint8_t irow  = 0, irow < input.size; ++irow){
		for(uint8_t icol  = 0, icol < input.size; ++icol){
			
			// kernel iterables
			for(uint8_t krow = 0; krow < kernel.size; ++krow){
				for(uint8_t kcol = 0; kcol < kernel.size; ++kcol){
					
					// image-kernel index mapping ~ must be signed 
					int rindex = irow + krow - kernel.size / 2;
					int cindex = icol + kcol - kernel.size / 2;
					
					// simulate padded borders
					if(rindex < 0){
						rindex = 0;
					}else if(rindex > kernel.size -1){
						rindex = kernel.size -1;
					}
					if(cindex < 0){
						cindex = 0;
					}else if(cindex > kernel.size -1){
						cindex = kernel.size -1;
					}

					// convolve 
					output.data[irow][icol] += kernel.data[krow][kcol] * input.data[rindex][cindex];
				}
			}
		}
	}
}