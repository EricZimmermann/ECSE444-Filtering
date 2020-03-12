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
	uint8_t v, u;             // wave components
	uint8_t y, x;             // spatial component
	float modulator;
	float norm = input.size * input.size;
	
	// accumulator
	for(v = 0; v < input.size; ++v){
		for(u = 0; u < input.size; ++u){
			for(y = 0; y < input.size; ++y){
				for(x = 0; x < input.size; ++x){
					modulator = -2.0 * PI * ((1.0 * u * x + 1.0 * v * y) / input.size);
					output.re[v][u] += input[y][x] * cos(modulator);
					output.im[v][u] += input[y][x] * sin(modulator);
				}
			}
		}
	}

	// normalize by avg
	for(v = 0; v < input.size; ++v){
		for(u = 0; u < input.size; ++u){
				output.re[v][u] /= norm;
				output.im[v][u] /= norm;
		}
	}
}

// https://www.originlab.com/doc/Origin-Help/InverseFFT2-Algorithm
void ifft(struct CImage input, struct Image output){
	uint8_t v, u;             // wave components
	uint8_t y, x;             // spatial component
	float modulator;
	float norm = input.size * input.size;
		
	// accumulator
	for(y = 0; y < input.size; ++y){
		for(x = 0; x < input.size; ++x){
			for(v = 0; v < input.size; ++v){
				for(u = 0; u < input.size; ++u){
					modulator = 2.0 * PI * ((1.0 * u * x + 1.0 * v * y) / input.size);
					output.data[y][x] += input.re[v][u] * cos(modulator) + input.im[v][u] * sin(modulator);
				}
			}
		}
	}
}

float magitude(float a, float b){
	return sqrt(a * a + b * b);
}

void lowpass(struct CImage output, float min){
	return
}
       



