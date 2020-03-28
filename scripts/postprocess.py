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
    noisey_txt = utils.loadTxt(os.path.join(args.input, 'noise.txt'))
    input_image = utils.txt2img(input_txt)
    noisey_image = utils.txt2img(noisey_txt)
    
    # TODO
    # Save img
    # compute psnr
    # print PSNR and log as json
    
if __name__ == '__main__':
    main()