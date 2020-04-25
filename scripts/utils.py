import cv2
import numpy as np

def loadImage(filepath, size):
    img =  cv2.imread(filepath, cv2.IMREAD_GRAYSCALE)
    return cv2.resize(img, (size, size))

def saveImage(filepath, image):
    cv2.imwrite(filepath, image)
    
def loadTxt(filepath):
    return np.loadtxt(filepath, delimiter=',')

def saveTxt(image, filepath):
    image_stream = listify(image)
    with open(filepath, "w") as output:
        for px in image_stream[:-1]:
            output.write(str(px)+',')
        output.write(str(image_stream[-1]))
        
def txt2img(string):
    size = int(np.sqrt(len(string)))
    return string.reshape((size,size))

def listify(image):
    flat = image.flatten()
    return flat

def fuzz(image, ranges, p=0.15):
    rows, cols = image.shape
    for row in range(rows):
        for col in range(cols):
            if(np.random.uniform() < p):
                image[row][col] += np.random.randint(-ranges, ranges)

def psnr(img_one, img_two):
	return cv2.PSNR(img_one, img_two)

