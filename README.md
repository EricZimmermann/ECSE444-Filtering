# ECSE444-Filtering

Instructions on how to run the code

## Environment Preparation
Pre and Post processing of images are done using python whose requirements are found in the root directory of the project and are be installed via:
`pip install -r requirements.txt` 


## Preprocessing
Done once! Colored images in .jpg or .png are converted from RBG to grayscale, resmapled, and flattened into `.txt` files for processing. Preprocess pipeline creates a clean.txt and noise.txt outputs where a noise image is created to validate the algorithms below.

1 .`python scripts/preprocess.py --input <full path to image> --output <directory of .txt creation> --size <target image size 2^n>`
2. clean.txt and noise.txt created in output directory

Optional cmds are:
`--r_noise <range of additive noise ~ default 15>`
`--p_noise <probability of additive noise ~ default 0.5>`

Note: image size must be of base power 2: {2, 4, 16, 32, 64, 128, 256, 512}


## Steps to run Convolution (requires terminal):
1. `cd src/imaging` (navigate into the directory containing the Makefile)
2.`make processmake` (builds the executable)
3. `./processmake -c <full path to the target directory containing input txt> <input txt filename> <size of img, eg: 32, 128> <sigma of kernel as float> <size of kernel eg: 3, 5, 7>
4. output.txt contains the resulting image and logging.txt contains the parameters we used (located in same directory as specified)

Note: image size must be identical to that selected in preprocessing
Note: kernel size must be odd and smaller than size of image


## Steps to run Naive FFT (requires terminal):
1. `cd src/imaging` (navigate into the directory containing the Makefile)
2. `make processmake` (builds the executable)
3. `./processmake -f <full path to the target directory containing input txt> <input txt filename> <size of img ideally <=32, eg: 32> <r value as float: eg: 1.0>`
4. output.txt contains the resulting image and logging.txt contains the parameters we used (located in same directory as specified)

Note: sizes of >32 will take too long


## Steps to run Cooley Turkey FFT (requires terminal):
1. `cd src/imaging` (navigate into the directory containing the Makefile)
2. `make processmake` (builds the executable)
3. `./processmake -t <full path to the target directory containing input txt> <input txt filename> <size of img, eg: 32, 128, 512> <r value as float: eg: 1.0>`
4. output.txt contains the resulting image and logging.txt contains the parameters we used (located in same directory as specified)

## Postprocessing
Compute PSNR metrics and add to logging.txt, create .jpg for output.txt file

1. `python scripts/postprocess.py --input <full path to directory of .txt files>`
2. logging.txt updated
3. output.png created 

