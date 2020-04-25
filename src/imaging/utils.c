#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>

// inplace swap 2d element - O(1) space 
void swap2d(float **input, short y_in, short x_in, short y_out, short x_out){
    float temp = input[y_in][x_in];
    input[y_in][x_in] = input[y_out][x_out];
    input[y_out][x_out] = temp;
}

// inplace swap 1d element - O(1) space
void swap1d(float *input, short loc_in, short loc_out){
    float temp = input[loc_in];
    input[loc_in] = input[loc_out];
    input[loc_out] = temp;
}
             
// Shift a spectra to be zero fq centered
// Unshift a spectra to be non-zero shifted centered
// Diagonalized quadrant swap
void shift(float **input, short size){    
    short quad = size >> 1;
    for(short y = 0; y < quad; ++y){
        for(short x = 0; x < quad; ++x){
            swap2d(input, y, x, y + quad, x + quad);
            swap2d(input, y + quad, x, y, x + quad);
        }        
    }
}

