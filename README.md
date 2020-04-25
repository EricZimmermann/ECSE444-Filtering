# ECSE444-Filtering

Instructions on how to run the code

## Steps to run Convolution (requires terminal):
1. `make processmake` (builds the executable)
2. `./processmake -c <full path to the target input.txt file> <size of img, eg: 32, 128> <sigma of kernel as float> <size of kernel eg: 3, 5, 7> <r distance as float: eg: 1.0>`
3. output.txt contains the resulting image and logging.txt contains the parameters we used


## Steps to run Naive FFT (requires terminal):
1. `make processmake` (builds the executable)
2. `./processmake -f <full path to the target input.txt file> <size of img ideally <=32, eg: 32> <r value as float: eg: 1.0>`
3. output.txt contains the resulting image and logging.txt contains the parameters we used


## Steps to run Cooley Turkey FFT (requires terminal):
1. `make processmake` (builds the executable)
2. `./processmake -t <full path to the target input.txt file> <size of img, eg: 32, 128, 512> <r value as float: eg: 1.0>`
3. output.txt contains the resulting image and logging.txt contains the parameters we used
