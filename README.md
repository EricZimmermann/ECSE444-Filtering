# ECSE444-Filtering

The following readme has been written in markdown and should therefore be viewed with the proper format for optimal clarity.

## Directory Structure

`├── README.md` <br>
`├── Stochastic_Analysis.ipynb` <br>
`├── prototyping` <br>
`│   ├── obj.py` <br>
`│   ├── processing.py` <br>
`│   ├── test.png` <br>
`│   └── validation.ipynb` <br>
`├── requirements.txt` <br>
`├── scripts` <br>
`│   ├── autotest.py` <br>
`│   ├── postprocess.py` <br>
`│   ├── preprocess.py` <br>
`│   └── utils.py` <br>
`├── src` <br>
`│   └── imaging` <br>
`│       ├── Makefile` <br>
`│       ├── conv.c` <br>
`│       ├── conv.h` <br>
`│       ├── filter.c` <br>
`│       ├── filter.h` <br>
`│       ├── fourier.c` <br>
`│       ├── fourier.h` <br>
`│       ├── image.c` <br>
`│       ├── image.h` <br>
`│       ├── main.c` <br>
`│       ├── processmake` <br>
`│       ├── utils.c` <br>
`│       └── utils.h` <br>
`└── test.png` <br>

## Project Description
The project provides implementations from scratch!!! of classical image denosing algorithms that include: <br>
Image Convolutions - `src/imaging/conv.c` <br>
Fourier Transforms (DFT and FFT) - `src/imaging/fourier.c` <br>
<br>
Image and complex image structures can be found in  `src/imaging/image.c` while filter structures are found in  `src/imaging/filter.c ` <br>

Note: A test image has been provided in root! please use this for simplicity!


## Environment Preparation
Pre and Post processing of images are done using python whose requirements are found in the root directory of the project and are to be installed via:
`pip install -r requirements.txt` <br>

If any issues persist with installations, please `pip install numpy` and `pip install opencv-python`.

Note: `pip install jupyter` if you would like to look at notebooks. To run notebooks, `pip install matplotlib`

## Preprocessing
Done once! Colored images in .jpg or .png are converted from RBG to grayscale, resampled, and flattened into `.txt` files for processing. Preprocess pipeline creates a clean.txt and noise.txt outputs where a noise image is created to validate the algorithms below.

1 .`python scripts/preprocess.py --input <full path to image> --output <directory of .txt creation> --size <target image size 2^n>` <br>
2. clean.txt and noise.txt created in output directory <br>

Optional cmds are: <br>
`--r_noise <range of additive noise ~ default 15>` <br>
`--p_noise <probability of additive noise ~ default 0.5>`  <br>

Note: image size must be of base power 2: {2, 4, 16, 32, 64, 128, 256, 512}


## Steps to run Convolution (requires terminal):
1. `cd src/imaging` (navigate into the directory containing the Makefile)
2. `make processmake` (builds the executable)
3. `./processmake -c <full path to the target directory containing input txt> <input txt filename> <size of img> <sigma of kernel as float> <size of kernel eg: 3, 5, 7>`
4. output.txt contains the resulting image and logging.txt contains the parameters we used (located in same directory as specified)

Note: image size must be identical to that selected in preprocessing <br>
Note: kernel size must be odd and smaller than size of image <br>


## Steps to run Naive FFT (requires terminal):
1. `cd src/imaging` (navigate into the directory containing the Makefile)
2. `make processmake` (builds the executable)
3. `./processmake -f <full path to the target directory containing input txt> <input txt filename> <size of img ideally <=32, eg: 32> <r value as float: eg: 1.0>`
4. output.txt contains the resulting image and logging.txt contains the parameters we used (located in same directory as specified)

Note: sizes of >128 will take too long


## Steps to run Cooley-Tukey FFT (requires terminal):
1. `cd src/imaging` (navigate into the directory containing the Makefile)
2. `make processmake` (builds the executable)
3. `./processmake -t <full path to the target directory containing input txt> <input txt filename> <size of img, eg: 32, 128, 512> <r value as float: eg: 1.0>`
4. output.txt contains the resulting image and logging.txt contains the parameters we used (located in same directory as specified)

## Postprocessing
Compute PSNR metrics and add to logging.txt, create .jpg for output.txt file

1. `python scripts/postprocess.py --input <full path to directory of .txt files>`
2. logging.txt updated
3. output.png created 

## Example
1.  `mkdir experiment` <br>
2.  `python scripts/preprocess.py --input ./test.png --output ./experiment --size 256` <br>
3.   `cd src/imaging ` <br>
4.   `make processmake` <br>
5.  `./processmake -t ../../experiment/ noisy.txt 256 0.3` <br>
6.  `cd ../../` <br>
7. `python scripts/postprocess.py --input ./experiment` <br>

Note: Repeat steps 5 - 7 as needed


## Automated Testing (at your own risk!)
Runs a set of configs to compure batch ffts and persist info. Please edit configs before running or else! Run from root!

`python scripts/autotest.py --input <full path to test img>`

