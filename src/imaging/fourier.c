/*
fft and ifft
complex representations
cooley-tukey
*/

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "fourier.h"
#include "filter.h"
#include "image.h"
#include "utils.h"
#define PI 3.14159265

// radix-2 bit reversal
// im not sure if this works, we need to find a way to return both re and im. C doesn't work well with passing by reference
// somehow its an operation with & that we are missing here i thin
void bitReverse(float *re, float *im, short size){
    short i = 0; 
    short j = 0;
    short k = 0;
    short bit = 0;
    
    for(i = 0; i < size; ++i){
        if(i < j){
            swap1d(re, i, j);
            swap1d(im, i, j);
        }
        bit = 1 + (i ^ (i + 1));
        k = size / bit;
        j ^= size - k;
    }
}

// custom mod int of log base 2
short customlog2(short size){
    short base = 0;
    while(size != 1){
        size >>= 1;
        ++base;   
    }
    return base;
}


// https://www.originlab.com/doc/Origin-Help/FFT2-Algorithm
// Compute fft using naive dft
void fft(struct Image *input, struct CImage *output){
	
	short v, u;             // wave components
	short y, x;             // spatial component
	float modulator;
	float norm = input -> size * input -> size;
	
	// accumulator
	for(v = 0; v < input->size; ++v){
		for(u = 0; u < input->size; ++u){
			for(y = 0; y < input->size; ++y){
				for(x = 0; x < input->size; ++x){
					modulator = -2.0 * PI * ((1.0 * u * x + 1.0 * v * y) / input->size);
					output -> re[v][u] += input->data[y][x] * cosf(modulator);
					output -> im[v][u] += input->data[y][x] * sinf(modulator);
				}
			}
		}
	}

	// normalize by avg
	for(v = 0; v < input->size; ++v){
		for(u = 0; u < input->size; ++u){
			output->re[v][u] /= norm;
			output->im[v][u] /= norm;
		}
	}
}

// https://www.originlab.com/doc/Origin-Help/InverseFFT2-Algorithm
// Compute ifft using naive dft
void ifft(struct CImage *input, struct Image *output){
	
	short v, u;             // wave components
	short y, x;             // spatial component
	float modulator;
		
	// accumulator
	for(y = 0; y < input->size; ++y){
		for(x = 0; x < input->size; ++x){
			for(v = 0; v < input->size; ++v){
				for(u = 0; u < input->size; ++u){
					modulator = 2.0 * PI * ((1.0 * u * x + 1.0 * v * y) / input->size);
					output->data[y][x] += input->re[v][u] * cosf(modulator) - input->im[v][u] * sinf(modulator);
				}
			}
		}
	}
}

// compute inplace complex to complex ft using radix-2 bit reversal time decimation
// https://en.wikipedia.org/wiki/Cooley%E2%80%93Tukey_FFT_algorithm#Data_reordering,_bit_reversal,_and_in-place_algorithms
void ctft(float *re, float *im, short size, short d){
    // delarations and inits
    short base = customlog2(size);           // radix-2 division points
    short l1 = 0;                       // radix-2 indexer
    short l2 = 0;                       // radix-2 indexer
    float mod, fmod, mod_re, mod_im = 0.0;       // fq modulators
    float re_cache, im_cache = 0.0;              // caches for complex computations
    l2 = 1;                                      // index step
    
    // reverse bits for butterfly radix-2 comps
    bitReverse(re, im, size);
    for(short radix = 0; radix < base; ++radix){
        l1 = l2;
        l2 <<= 1;
        
        fmod = 2 * PI / l2;          // dtheta over interval
        if(d == 1){
            fmod *= -1;              // fft vs ifft select
        }
        
        mod = 0.0;
        for(short j = 0; j < l1; ++j){           // modulate
            mod_re = cosf(mod);
            mod_im = sinf(mod);
            
            for(short k = j; k < size; k += l2){   // compute
                re_cache = mod_re * re[k + l1] - mod_im * im[k + l1];
                im_cache = mod_im * re[k + l1] + mod_re * im[k + l1];
                re[k + l1] = re[k] - re_cache;
                im[k + l1] = im[k] - im_cache;
                re[k] += re_cache;
                im[k] += im_cache;
                // printf("loop bit reverse\n");
            }
            
            mod += fmod;     // next step along fq theta = theta + dtheta
        }
    }
    
    // normalize depending on direction
    if (d == 1){
        for(unsigned short i = 0; i < size; ++i){
            re[i] /= size;
            im[i] /= size;
        }
    }
}

// 2D decomp fft/ifft into row-col processes
void ctftrc(struct CImage *input, short direction){
    
    // iterators
    short x,y = 0;
    
    // init buffers
    float *buffer_re = (float *)malloc(input->size * sizeof(float));
    float *buffer_im = (float *)malloc(input->size * sizeof(float));
    // process fft axis-wise
    for(y = 0; y < input->size; ++y){
        // cache 
        for(x = 0; x < input->size; ++x){
            buffer_re[x] = input->re[y][x]; 
            buffer_im[x] = input->im[y][x];
        }
        // transform
        ctft(buffer_re, buffer_im, input->size, direction);
        
        // replace
        for(x = 0; x < input->size; ++x){
            input->re[y][x] = buffer_re[x]; 
            input->im[y][x] = buffer_im[x];
        }
    }
    int lpl = 0;
    // alternate axis
    for(x = 0; x < input->size; ++x){
        // cache 
        for(y = 0; y < input -> size; ++y){
            buffer_re[y] = input->re[y][x]; 
            buffer_im[y] = input->im[y][x];
        }
        lpl++;
        // transform
        ctft(buffer_re, buffer_im, input -> size, direction);
                

        // replace
        for(y = 0; y < input->size; ++y){
            input->re[y][x] = buffer_re[y]; 
            input->im[y][x] = buffer_im[y];
            
        }
    }
    // free(buffer_re);
    // free(buffer_im);
}


// compute fft using cooly-tukey radix-2 bit reversal optims
// assumes size is 2^n ~ row-col generalized fft
// inplace ~ malloc sqrt(2N) buff space complexity
void ctftt(struct CImage *input){
    ctftrc(input, 1);
}

// compute ifft using cooly-tukey radix-2  bit reversal optims
// asumes size is 2^n ~ row-col generalized fft
// inplace ~ malloc sqrt(2N) buff space complexity
void ctifft(struct CImage *input){
    ctftrc(input, 0);
}

// apply a lps to a complex representation of an image
void lowPass(struct CImage *input, struct Kernel *filter){
    for(short i = 0; i <  input -> size; ++i){
        for(short j = 0; j <  input -> size; ++j){
            input -> re[i][j] *= filter -> data[i][j];
            input -> im[i][j] *= filter -> data[i][j];
        }
    }
}
