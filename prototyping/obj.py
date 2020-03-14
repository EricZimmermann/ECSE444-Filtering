import cv2 
import numpy as np


class Image():
    def __init__(self, filepath, size):
        self.size = size
        self.data = cv2.imread(filepath, cv2.IMREAD_GRAYSCALE).astype(dtype=np.float32)
        self.data = cv2.resize(self.data, (size, size))

    def clear(self):
        self.data = np.zeros_like(self.data)

class CImage():
    def __init__(self, filepath, size):
        self.size = size
        self.re = cv2.imread(filepath, cv2.IMREAD_GRAYSCALE).astype(dtype=np.float32)
        self.re = cv2.resize(self.re, (size, size))
        self.im = np.zeros_like(self.re)

    def clear(self):
        self.re = np.zeros_like(self.re)
        self.im = np.zeros_like(self.im)


class Kernel():
    def __init__(self, size):
        self.size = size
        self.data = np.zeros((size,size))

    def generateFilter(self, sigma):
        sum = 0;
        half_band = self.size // 2;

        for i in range(-half_band, half_band + 1):
            for j in range(-half_band, half_band + 1):
                self.data[i+half_band][j+half_band] = np.exp(-1*((i**2 + j**2) / (2 * sigma**2)))
                sum += self.data[i+half_band][j+half_band]
                
        for i in range(-half_band, half_band + 1):
            for j in range(-half_band, half_band + 1):
                self.data[i+half_band][j+half_band] /= sum;