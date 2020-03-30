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
    
    # base
    base = int(np.log2(input.size))
    
    # row - col bit reversal dft
    for y in range(input.size):
        for x in range(input.size):
            buffer_re[x] = input.re[y][x]
            buffer_im[x] = input.im[y][x]
        
        _ctft(buffer_re, buffer_im, base, d)
        
        for x in range(input.size):
            input.re[y][x] = buffer_re[x]
            input.im[y][x] = buffer_im[x]
    
    for x in range(input.size):
        for y in range(input.size):
            buffer_re[y] = input.re[y][x]
            buffer_im[y] = input.im[y][x]
          
        _ctft(buffer_re, buffer_im, base, d)

        for y in range(input.size):
            input.re[y][x] = buffer_re[y]
            input.im[y][x] = buffer_im[y]
    
    
# TODO refactor this section 
# 1D ct ft
def _ctft(re, im, base, d):
    
    # init vars
    size = 0
    
    #iterators
    i = 0
    j = 0
    k = 0
    
    # temps for bit reversals
    temp_re = 0.0
    temp_im = 0.0

    i1 = 0
    i2 = 0
    l = 0
    l1 = 0
    l2 = 0
    c1 = 0.0
    c2 = 0.0

    t1 = 0.0
    t2 = 0.0
    u1 = 0.0
    u2 = 0.0
    z = 0.0
    
    size = 2 ** base

    # bit reversal
    i2 = size >> 1;
    for i in range(size-1):
        if (i < j):
            temp_re = re[i]
            temp_im = im[i]
            re[i] = re[j]
            im[i] = im[j]
            re[j] = temp_re
            im[j] = temp_im
        k = i2;
        while (k <= j):
            j -= k
            k >>= 1

        j += k;

    # fft
    c1 = -1.0
    c2 = 0.0
    l2 = 1
    for l in range(base):
        l1 = l2
        l2 <<= 1
        u1 = 1.0
        u2 = 0.0
        for j in range(l1):
            for i in range(j,size,l2):
                i1 = i + l1
                t1 = u1 * re[i1] - u2 * im[i1]
                t2 = u1 * im[i1] + u2 * re[i1]
                re[i1] = re[i] - t1
                im[i1] = im[i] - t2
                re[i] += t1
                im[i] += t2
                
            z =  u1 * c1 - u2 * c2
            u2 = u1 * c2 + u2 * c1
            u1 = z
        
        # compute radix-2 trig coef using known factorizations
        c2 = np.sqrt((1.0 - c1) / 2.0)
        if d ==1:
            c2 = -c2   # modulation dir flipped
        c1 = np.sqrt((1.0 + c1) / 2.0)

    if d ==1: 
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


##############################################   TEST    ########################################################


# def _ctftu(re, im, N, q):
#     fundamental_modulator = 2*np.pi / N
#     m = N // 2
#     re_cache = [0, 0]
#     im_cache = [0, 0]

#     if N > 1:
#         for p in range(m):
#             # modulation
#             modulator = p * fundamental_modulator
#             re_modulation = np.cos(modulator)
#             im_modulation = -np.sin(modulator)
            
#             # cache
#             re_cache[0] = re[q+p]
#             re_cache[1] = re[q+p+m]
#             im_cache[0] = im[q+p]
#             im_cache[1] = im[q+p+m]
            
#             # complex comps
#             re[q+p] = re_cache[0] + re_cache[1]
#             im[q+p] = im_cache[0] + im_cache[1]
#             re[q+p+m] = re_cache[0] - re_cache[1]
#             im[q+p+m] = im_cache[0] - im_cache[1]
#             re[q+p+m] = re_modulation*re[q+p+m] - im_modulation*im[q+p+m]
#             im[q+p+m] = re_modulation*im[q+p+m] + im_modulation*re[q+p+m]
            
#         # log2 decimation dft
#         _ctftu(re, im, N//2, q)
#         _ctftu(re, im, N//2, q+m)
        
# def bitReverse(re, im, size):
    
#     j = 0
#     i2 = size >> 1;
#     for i in range(size-1):
#         if (i < j):
#             temp_re = re[i]
#             temp_im = im[i]
#             re[i] = re[j]
#             im[i] = im[j]
#             re[j] = temp_re
#             im[j] = temp_im
#         k = i2;
#         while (k <= j):
#             j -= k
#             k >>= 1

#         j += k;

                        
# def _conj(im, size):
#     for idx in range(size):
#         im[idx] *= -1

# def _ctft(re, im, size, d):
#     if d ==0:
#         _conj(im, size)
       
#     _ctftu(re, im, size, 0)
#     bitReverse(re, im, N)
        
#     if d ==0:
#         _conj(im, size)
#         for idx in range(size):
#             re[idx] /= size
#             im[idx] /= size
            
# # row-col fft divisions for divide and conquer
# def _ctrc(input, d):
    
#     # buffers ~ malloc or prealloc and pass in
#     buffer_re = np.zeros(input.size)
#     buffer_im = np.zeros(input.size)
    
#     # row - col bit reversal dft
#     for y in range(input.size):
#         for x in range(input.size):
#             buffer_re[x] = input.re[y][x]
#             buffer_im[x] = input.im[y][x]
        
#         _ctft(buffer_re, buffer_im, input.size, d)
        
#         for x in range(input.size):
#             input.re[y][x] = buffer_re[x]
#             input.im[y][x] = buffer_im[x]
    
#     for x in range(input.size):
#         for y in range(input.size):
#             buffer_re[y] = input.re[y][x]
#             buffer_im[y] = input.im[y][x]
          
#         _ctft(buffer_re, buffer_im, input.size, d)

#         for y in range(input.size):
#             input.re[y][x] = buffer_re[y]
#             input.im[y][x] = buffer_im[y]