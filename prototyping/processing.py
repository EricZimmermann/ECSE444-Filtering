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


def ifft(input, output):
    PI  = 3.14159265
    norm = input.size * input.size;

    for y in range(input.size):
        for x in range(input.size):
            for v in range(input.size):
                for u in range(input.size):
                    modulator = 2.0 * PI * ((1.0 * u * x + 1.0 * v * y) / input.size)
                    output.data[input.size - y - 1][input.size - x - 1] += input.re[v][u] * np.cos(modulator) + input.im[v][u] * np.sin(modulator)
                    
def nlm(input, output, wband, nband, sigma):

    for iy in range(input.size):
        for ix in range(input.size):
            weight = 0;
            value = 0;
            sum = 0;

            for wy in range(-wband, wband+1):
                for wx in range(-wband, wband+1):
                    if(ix + wx >= input.size or iy + wy >= input.size):
                        continue
                    if(ix + wx < 0 or iy + wy < 0):
                        continue;
                    dist = 0;
                    for nby in range(-nband, nband+1):
                        for nbx in range(-nband, nband+1):

                            if (ix + wx + nbx < 0 or ix + wx + nbx >= input.size):
                                continue
                            if(iy + wy + nby < 0 or iy + wy + nby >= input.size):
                                continue
                            if(ix + nbx < 0 or ix + nbx >= input.size):
                                continue
                            if(iy + nby < 0 or iy + nby >= input.size):
                                continue
                        
                            vnby = input.data[iy + wy + nby][ix + wx + nbx]
                            vnbx = input.data[iy + nby][ix + nbx]
                            dist += (vnbx - vnby) * (vnbx - vnby)

                    weight = np.exp( -1.0 * (dist / sigma + (wx*wx + wy*wy) / ((wband + 1)*(wband + 1))));
                    sum += weight
                    value += input.data[iy + wy ][ix + wx ] * weight

            output.data[iy][ix] = value / sum;

