import cv2
import numpy

def loadImage(filepath):
	return cv2.imread(filepath, cv2.IMREAD_GRAYSCALE)

def save(image, filepath):
	image_stream = listify(image)
	with open(filepath, "w") as output:
    	output.write(str(image))

def listify(image):
 	return image.flatten().tolist()

def fuzz(image, ranges, p=0.15):
	rows, cols = image.shape
	for row in range(rows):
		for col in range(cols):
			if(numpy.random.uniform() < p):
				image[row][col] += numpy.random.randint(-ranges, ranges)

def psnr(img_one, img_two):
	return cv2.PSNR(img1, img2)