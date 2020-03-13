#include <math.h>
#include <stdio.h>
#include "image.h"


void nlm(struct Image input, struct Image output, uint8_t window_band, uint8_t nb_band, float sigma){

	uint8_t ix, iy, wx, wy, nbx, nby; 		// iterables
	float vnbx, vnby;						// neighbours
	float dist;                     		// comparators
	float weight;    
	float value;     
	float sum;       

	// iterate pixels
	for(iy = 0; iy < input.size; ++iy){
		for(ix = 0; ix < input.size; ++ix){

			weight = 0;
			value = 0;
			sum = 0;

			// iterate window 
			for(wy = -window_band; wy <= window_band; ++wy){
				for(wx = -window_band; wx <= window_band; ++wx){

					// validate img boundaries -  stack friendly
					if(ix + wx > input.size || iy + wy > input.size){
						continue;
					}
					if(ix + wx < 0 || iy + wy < 0){
						continue;
					}

					dist = 0;

					//iterate neighbours
					for(nby = -nb_band; nby <= nb_band; ++nby){
						for(nbx = -nb_band; nbx <= nb_band; ++nbx){
										
							// validate img boundaries -  stack friendly
							if (ix + wx + nbx < 0 || ix + wx + nbx >= N){
								continue;
							} 
							if(iy + wy + nby < 0 || iy + wy + nby >= N){
								continue;
							}
							if(ix + nbx < 0 || ix + nbx >= N){
								continue;
							}
							if(iy + nby < 0 || iy + nby >= N){
								continue;
							}

							// compute values
							vnby = input.data[iy + wy + nby][ix + wx + nbx];
							vnbx = input.data[iy + nby][ix + nbx];
							dist += (vnbx - vnby) * (vnbx - vnby);

						}
					}

					weight = expf( -1.0 * (dist / sigma + (wx*wx + wy*wy) / (float)((window_band + 1)*(window_band + 1))));
					sum += weight;
					value += input.data[iy + wy + nby][ix + wx + nbx] * weight;				
				}
			}

			input.data[iy][ix] = value / sum;
		}
	}
}

