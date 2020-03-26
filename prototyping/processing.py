import numpy as np

def convolve(input, output, kernel):

    half_band = kernel.size // 2

    for irow in range(input.size):
        for icol in range(input.size):
            for krow in range(kernel.size):
                for kcol in range(kernel.size):
                    rindex = irow + krow - half_band
                    cindex = icol + kcol - half_band

                    if(rindex < 0):	
                        rindex = 0
                    if(rindex > input.size -1):
                        rindex = input.size -1
                    if(cindex < 0):
                        cindex = 0
                    if(cindex > input.size -1):
                        cindex = input.size -1

                    output.data[irow][icol] += kernel.data[krow][kcol] * input.data[rindex][cindex]


def fft(input, output):
    PI  = 3.14159265
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

#shift by 1 issue
def ifft(input, output):
    PI  = 3.14159265
    norm = input.size * input.size;

    for y in range(input.size):
        for x in range(input.size):
            for v in range(input.size):
                for u in range(input.size):
                    modulator = 2.0 * PI * ((1.0 * u * x + 1.0 * v * y) / input.size)
                    output.data[input.size - y - 1][input.size - x - 1] += input.re[v][u] * np.cos(modulator) + input.im[v][u] * np.sin(modulator)
                    
def lowpass(cimage, filter):
    for i in range(cimage.size):
        for j in range(cimage.size):
            cimage.re[i][j] *= filter.data[i][j]
            cimage.im[i][j] *= filter.data[i][j]

# TODO
def shift(input):
    return input

def unshift(input):
    return input

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