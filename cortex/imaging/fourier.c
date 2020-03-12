/*
fft and ifft
complex representations
cooley-tukey
*/

#include <stdlib.h>
#include <math.h>
#include "image.h"
#define PI 3.14159265


// https://www.originlab.com/doc/Origin-Help/FFT2-Algorithm
void fft(struct Image input, struct CImage output){
	uint8_t yW, xW;             // wave components
	uint8_t yS, xS;             // spatial component  
	float modulator;
	float norm = input.size * input.size;
	
	// accumulator
	for(yW = 0; yW < input.size; ++yW){
		for(xW = 0; xW < input.size; ++xW){
			for(yS = 0; yS < input.size; ++yS){
				for(xS = 0; xS < input.size; ++xS){
					modulator = -2.0 * PI * ((1.0 * xW * xS + 1.0 * yW * yS) / input.size);
					output.re[yW][xW] += input[yS][xS] * cos(modulator);
					output.im[yW][xW] += input[yS][xS] * sin(modulator);
				}
			}
		}
	}

	// normalize by avg
	for(yW = 0; yW < input.size; ++yW){
		for(xW = 0; xW < input.size; ++xW){
				output.re[yW][xW] /= norm;
				output.im[yW][xW] /= norm;
		}
	}
}

// https://www.originlab.com/doc/Origin-Help/InverseFFT2-Algorithm
void ifft(struct CImage input, struct Image output){
	uint8_t yW, xW;             // wave components
	uint8_t yS, xS;             // spatial component
	float modulator;
	float norm = input.size * input.size;
		
	// accumulator
	for(yS = 0; yS < input.size; ++yS){
		for(xS = 0; xS < input.size; ++xS){
			for(yW = 0; yW < input.size; ++yW){
				for(xW = 0; xW < input.size; ++xW){
					modulator = 2.0 * PI * ((1.0 * xW * xS + 1.0 * yW * yS) / input.size);
					output.data[yS][xS] += input.re[yW][xW] * cos(modulator) + input.im[yW][xW] * sin(modulator);
				}
			}
		}
	}

	// normalize by avg
	for(yS = 0; yS < input.size; ++yS){
		for(xS = 0; xS < input.size; ++xS){
				output.data[yS][xS] /= norm;
		}
	}
}

float magitude(float a, float b){
	return sqrt(a * a + b * b);
}

void lowpass(struct CImage output, float min){
	return
}
       



