import numpy as np

# Generic 
def convolve(input, output, kernel):

    half_band = kernel.size // 2

    for irow in range(input.size):
        for icol in range(input.size):
            for krow in range(kernel.size):
                for kcol in range(kernel.size):
                    rindex = irow + krow - half_band
                    cindex = icol + kcol - half_band

                    # edge padding via px extension
                    if(rindex < 0):	
                        rindex = 0
                    if(rindex > input.size -1):
                        rindex = input.size -1
                    if(cindex < 0):
                        cindex = 0
                    if(cindex > input.size -1):
                        cindex = input.size -1

                    output.data[irow][icol] += kernel.data[krow][kcol] * input.data[rindex][cindex]

# Naive
def fft(input, output):
    PI  = np.pi
    norm = input.size * input.size

    for v in range(input.size):
        for u in range(input.size):
            for y in range(input.size):
                for x in range(input.size):
                    modulator = -2.0 * PI * ((1.0 * u * x + 1.0 * v * y) / input.size)
                    output.re[v][u] += input.re[y][x] * np.cos(modulator)
                    output.im[v][u] += input.re[y][x] * np.sin(modulator)

    for v in range(input.size):
        for u in range(input.size):
            output.re[v][u] /= norm
            output.im[v][u] /= norm

# Naive
def ifft(input, output):
    PI  = np.pi
    
    for y in range(input.size):
        for x in range(input.size):
            for v in range(input.size):
                for u in range(input.size):
                    modulator = 2.0 * PI * ((1.0 * u * x + 1.0 * v * y) / input.size)
                    output.data[y][x] += input.re[v][u] * np.cos(modulator) - input.im[v][u] * np.sin(modulator)
                
# Cooley-Tukey Implementation
def ct_fft(input):
    _ctrc(input, 1)
    
def ct_ifft(input):
    _ctrc(input, 0)

# row-col fft divisions for divide and conquer
def _ctrc(input, d):
    
    # buffers ~ malloc or prealloc and pass in
    buffer_re = np.zeros(input.size)
    buffer_im = np.zeros(input.size)
   
    # row - col bit reversal dft
    for y in range(input.size):
        for x in range(input.size):
            buffer_re[x] = input.re[y][x]
            buffer_im[x] = input.im[y][x]
        
        _ctft(buffer_re, buffer_im, input.size, d)
        
        for x in range(input.size):
            input.re[y][x] = buffer_re[x]
            input.im[y][x] = buffer_im[x]
    
    for x in range(input.size):
        for y in range(input.size):
            buffer_re[y] = input.re[y][x]
            buffer_im[y] = input.im[y][x]
          
        _ctft(buffer_re, buffer_im, input.size, d)

        for y in range(input.size):
            input.re[y][x] = buffer_re[y]
            input.im[y][x] = buffer_im[y]
    
# 1D ct ft
def _ctft(re, im, size, d):
    
    # bit reverse iterables
    rev_loc = 0
    rev_i = 0
    rev_j = 0
    rev_k = 0  
    
    # temps
    temp_re = 0.0
    temp_im = 0.0
    
    # bit reversal
    reverse_loc = size >> 1;
    for rev_i in range(size-1):
        if (rev_i < rev_j):
            temp_re = re[rev_i]
            temp_im = im[rev_i]
            re[rev_i] = re[rev_j]
            im[rev_i] = im[rev_j]
            re[rev_j] = temp_re
            im[rev_j] = temp_im
        rev_k = reverse_loc;
        while (rev_k <= rev_j):
            rev_j -= rev_k
            rev_k >>= 1

        rev_j += rev_k
     
    l1 = 0             # base power location 1
    l2 = 1             # base power location 2
    base_mod = 0       # freq subdivisor ~ count steps
    mod = 0            # starting point
    re_cache = 0.0     # cache real vals
    im_cache = 0.0     # cache complex vals
    for s in range(int(np.log2(size))):
        l1 = l2
        l2 <<= 1
        base_mod = 2 * np.pi / l2
        if d == 1:
            base_mod *= -1
        mod = 0.0
        for j in range(l1):
            mod_re = np.cos(mod)
            mod_im = np.sin(mod)
            
            for k in range(j, size, l2):
                re_cache = mod_re * re[k + l1] - mod_im * im[k + l1]
                im_cache = mod_im * re[k + l1] + mod_re * im[k + l1]
                re[k + l1] = re[k] - re_cache
                im[k + l1] = im[k] - im_cache
                re[k] += re_cache
                im[k] += im_cache
                
            mod += base_mod
    if d == 1: 
        for i in range(size):
            re[i] /= float(size)
            im[i] /= float(size)
            
                
#LPS via multiplicative fourier                   
def lowpass(cimage, filter):
    for i in range(cimage.size):
        for j in range(cimage.size):
            cimage.re[i][j] *= filter.data[i][j]
            cimage.im[i][j] *= filter.data[i][j]

# Regular and inplace implementations below ~ no safety
# C implementation for inplace shift can be improved via passing by ref
def shift(input, output=None, inplace=True):
    def _swap(data, y_in, x_in, y_out, x_out):
        temp = data[y_in][x_in]
        data[y_in][x_in] = data[y_out][x_out]
        data[y_out][x_out] = temp
        
    quad = input.size // 2
    if inplace:
        for y in range(quad):
            for x in range(quad):
                _swap(input.data, y, x, y+quad, x+quad)
                _swap(input.data, y+quad, x, y, x+quad)
    else:
        for y in range(input.size):
            for x in range(input.size):
                if(x >= quad and y >= quad):
                    output.data[y][x] = input.data[y-quad][x-quad]
                if(x < quad and y < quad):
                    output.data[y][x] = input.data[y+quad][x+quad]
                if(x >= quad and y < quad):
                    output.data[y][x] = input.data[y+quad][x-quad]
                if(x < quad and y >= quad):
                    output.data[y][x] = input.data[y-quad][x+quad]

# Leverage np slicing (not C native) ~ used strictly as a util
def quickShift(image):
    
    shifted = np.zeros(image.shape)
    quad = image.shape[0] // 2
    
    # image subdivision and swap
    shifted[quad:, quad:] = image[:quad, :quad]
    shifted[:quad, :quad] = image[quad:, quad:]
    shifted[:quad, quad:] = image[quad:, :quad]
    shifted[quad:, :quad] = image[:quad, quad:]
   
    return shifted