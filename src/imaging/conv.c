/*
Convolutional operations
*/

#include "image.h"
#include "filter.h"

void convolve(struct Image input, struct Image output, struct Kernel kernel){

	unsigned short irow, icol;
	unsigned short krow, kcol;
	unsigned short half_band = kernel.size >> 1;
	int rindex;     // can overflow if unsigned
    int cindex;     // can overflow if unsigned

	// image iterables
    for(irow  = 0; irow < input.size; ++irow){
        for(icol = 0; icol < input.size; ++icol){
	
			// kernel iterables
			for(krow = 0; krow < kernel.size; ++krow){
				for(kcol = 0; kcol < kernel.size; ++kcol){
					
					// image-kernel index mapping ~ must be signed 
					rindex = irow + krow - half_band;
					cindex = icol + kcol - half_band;
					
                    // simulate padded borders via px extention
					if(rindex < 0){
						rindex = 0;
					}
                    if(rindex > input.size - 1){
						rindex = input.size - 1;
					}
					if(cindex < 0){
						cindex = 0;
					}
                    if(cindex > input.size - 1){
						cindex = input.size - 1;
					}

					// convolve 
					output.data[irow][icol] += kernel.data[krow][kcol] * input.data[rindex][cindex];
				}
			}
		}
	}
}
