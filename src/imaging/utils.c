// inplace swap 2d element - O(1) space 
void swap2d(float **input, y_in, x_in, y_out, x_out){
    temp = input[y_in][x_in];
    input[y_in][x_in] = input[y_out][x_out];
    input[y_out][x_out] = temp;
}

// inplace swap 1d element - O(1) space
void swap1d(float *input, loc_in, loc_out){
    temp = input[loc_in];
    input[loc_in] = input[loc_out];
    input[loc_out] = temp;
}
             
// Shift a spectra to be zero fq centered
// Unshift a spectra to be non-zero shifted centered
// Diagonalized quadrant swap
void shift(float **input, int size){    
    uint16_t quad = size >> 1;
    for(uint16_t y = 0; y < quad; ++y){
        for(uint16_t x = 0; x < quad; ++x){
            swap2d(input, y, x, y + quad, x + quad);
            swap2d(input, y + quad, x, y, x + quad);
        }        
    }
}
