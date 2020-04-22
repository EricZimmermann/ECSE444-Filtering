import argparse
import os
import utils
from copy import deepcopy

parser = argparse.ArgumentParser()
parser.add_argument('--input', type=str, required=True)
parser.add_argument('--output', type=str, required=True)
parser.add_argument('--size', type=int, required=True)
parser.add_argument('--p-noise', type=float, required=False, default=0.5)
parser.add_argument('--r-noise', type=float, required=False, default=5)

def main():
    
    args = parser.parse_args()
    input_img = utils.loadImage(args.input, args.size)
    noisey_img = deepcopy(input_img)
    utils.fuzz(noisey_img, args.r_noise, args.p_noise)
    txt_img = utils.listify(input_img)
    txt_noise = utils.listify(noisey_img)
    utils.saveTxt(txt_img, os.path.join(args.output, 'clean.txt'))
    utils.saveTxt(txt_noise, os.path.join(args.output, 'noise.txt'))
    
if __name__ == '__main__':
    main()