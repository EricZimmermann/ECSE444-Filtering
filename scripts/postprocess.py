import argparse
import cv
import os
import utils
import numpy as np


parser = argparse.ArgumentParser()
parser.add_argument('--input', type=str, required=True)
parser.add_argument('--output', type=str, required=True)

def main():
    
    args = parser.parse_args()
    input_txt = utils.loadTxt(os.path.join(args.input, 'clean.txt'))
    processed_txt = utils.loadTxt(os.path.join(args.input, 'process.txt'))
    input_image = utils.txt2img(input_txt)
    processed_image = utils.txt2img(processed_txt)
    utils.saveImage(os.path.join(args.output, 'process.jpg'), processed_image)
    psnr = utils.psnr(input_image, processed_image)
    
    #todo: write psnr to file produced my exec
    
if __name__ == '__main__':
    main()
