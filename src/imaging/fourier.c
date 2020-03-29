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
// Compute fft using naive dft
void fft(struct Image input, struct CImage output){
	
	uint8_t v, u;             // wave components
	uint8_t y, x;             // spatial component
	float modulator;
	int norm = input.size * input.size;
	
	// accumulator
	for(v = 0; v < input.size; ++v){
		for(u = 0; u < input.size; ++u){
			for(y = 0; y < input.size; ++y){
				for(x = 0; x < input.size; ++x){
					modulator = -2.0 * PI * ((1.0 * u * x + 1.0 * v * y) / input.size);
					output.re[v][u] += input.re[y][x] * cosf(modulator);
					output.im[v][u] += input.re[y][x] * sinf(modulator);
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
// Compute ifft using naive dft
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
					output.data[y][x] += input.re[v][u] * cosf(modulator) - input.im[v][u] * sinf(modulator);
				}
			}
		}
	}
}

// compute fft using cooly-tukey radix-2 bit reversal optims
// assumes size is 2^n ~ row-col generalized fft
// inplace ~ malloc sqrt(2N) buff space complexity
void ctftt(struct CImage input){
    _ctftrc(struct CImage input, 1);
}

// compute ifft using cooly-tukey radix-2  bit reversal optims
// asumes size is 2^n ~ row-col generalized fft
// inplace ~ malloc sqrt(2N) buff space complexity
void ctifft(struct CImage input){
    _ctftrc(struct CImage input, 0);
}

// 2D decomp fft/ifft into row-col processes
void _ctftrc(_ctftrc(struct CImage input, int direction){
    
    // iterators
    uint16_t x,y = 0;
    
    // init buffers
    float *buffer_re = (float *)malloc(size * sizeof(float));
    float *buffer_im = (float *)malloc(size * sizeof(float));
    
    // get base size
    int base = (int)log2f(input.size);
    
    // process fft axis-wise
    for(y = 0; y < input.size; ++y){
        // cache 
        for(x = 0; x < input.size; ++x){
            buffer_re[x] = input.re[y][x]; 
            buffer_im[x] = input.im[y][x];
        }
        
        // transform
        _ctft(buffer_re, buffer_im, base, direction);
        
        // replace
        for(x = 0; x < input.size; ++x){
            input.re[y][x] = buffer_re[x]; 
            input.im[y][x] = buffer_im[x];
        }
    }
    
    // alternate axis
    for(x = 0; x < input.size; ++x){
        // cache 
        for(y = 0; y < input.size; ++y){
            buffer_re[y] = input.re[y][x]; 
            buffer_im[y] = input.im[y][x];
        }
        
        // transform
        _ctft(buffer_re, buffer_im, base, direction);
        
        // replace
        for(x = 0; x < input.size; ++x){
            input.re[y][x] = buffer_re[y]; 
            input.im[y][x] = buffer_im[y];
        }
    }
    
    free(buffer_re);
    free(buffer_im);
}

// compute inplace complex to complex ft using radix-2 bit reversal decimation
void _ctft(float **re, float **im, int base, int d){
    // TODO following python refactor 
}
   
             
// inplace swap 2d element - O(1) space 
void _swap2d(float **input, y_in, x_in, y_out, x_out){
    temp = input[y_in][x_in];
    input[y_in][x_in] = input[y_out][x_out];
    input[y_out][x_out] = temp;
}

// inplace swap 1d element - O(1) space
void _swap1d(float *input, loc_in, loc_out){
    temp = input[loc_in];
    input[loc_in] = input[loc_out];
    input[loc_out] = temp;
}
             
// Shift a spectra to be zero fq centered
// Unshift a spectra to be non-zero shifted centered
// Diagonalized quadrant swap
void shift(float **input, int size){    
    uint16_t = size / 2;
    for(uint16_t y = 0; y < size / 2; ++y){
        for(uint16_t x = 0; x < quad; ++x){
            _swap2d(input, y, x, y + quad, x + quad);
            _swap2d(input, y + quad, x, y, x + quad);
        }        
    }
}

// apply a lps to a complex representation of an image
void lowPass(struct CImage input, struct Kernel filter){
    for(uint16_t i = 0; i <  input.size; ++i){
        for(uint16_t j = 0; j <  input.size; ++i){
            input.re[i][j] *= filter.data[i][j];
            input.im[i][j] *= filter.data[i][j];
        }
    }
}