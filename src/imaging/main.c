#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conv.h"
#include "fourier.h"

#define MAXCHAR 100000
#define SIZE 128

int main(int argc, char* argv[]){
    printf("Image processing \n");
    FILE *fp;
    char str[MAXCHAR];
    int i = 0;
    int j = 0;
    int k = 0;
    char comma[] = ",";
    float *datastream;
    char *result;
    int sizeImg;
    float rValue;
    
    struct Image *imgInput;
    struct Image *imgOutput;
    struct CImage *cImgInput;
    struct Kernel *knl;
    int limit;
    while((argc > 1) && (argv)){
        switch(argv[1][1]){
            case 'i':
                printf("This is for the input image, open an image in this step.\n");   
                fp = fopen(argv[2], "r");
                if(fp == NULL){
                    printf("can't open file, please provide the full path\n");
                    return 1;
                }
                
                // This contains the whole image in a single string
                struct Image *img = malloc(sizeof(Image));
                initImage(img, SIZE);
                // printf("Yes");
                // read the file
                while (fgets(str, MAXCHAR, fp) != NULL){
                    
                    char *ptr = strtok(str, comma);
                    for(i = 0; i < SIZE; i++){
                        for(j = 0; j < SIZE; j++){
                            img -> data[i][j] = (float) atoi(ptr);
                            ptr = strtok(NULL, comma);
                        }
                    }
                }

                // the int are usually under 256 & the comma and the +1 is for the null terminator
                char *result = malloc(4 * SIZE * SIZE * sizeof(char) + 1);
                
                for(i = 0; i < SIZE; i++){
                    for(j = 0; j < SIZE; j++){
                        char *tmp = malloc(128 * sizeof(char));
                        sprintf(tmp, "%d", (int) img -> data[j][i]), 
                        strcat(result, tmp);
                        strcat(result, comma);
                        free(tmp);
                    }
                }

                result[strlen(result) - 1] = '\0';

                // this is to write the outputs to a file
                fp = fopen("output.txt", "rb+");
                if(fp == NULL) //if file does not exist, create it
                {
                    fp = fopen("output.txt", "wb");
                    fputs(result, fp);
                }

                free(result);
                fclose(fp);

                deinitImage(img);
                break;
            case 'c':
                printf("This is for the Convolution test\n");

                // input arguments required: input image, size of image, sigma of kernel, size of kernel, rdistance
                // example:
                // ./processmake -c /home/heqianw/Documents/git/ECSE444-Filtering/prototyping/clean.txt 128 0.2 5 3.5
                
                fp = fopen(argv[2], "r");
                if(fp == NULL){
                    printf("can't open file, please provide the full path\n");
                    return 1;
                }

                sizeImg = atoi(argv[3]);
                float sigma = atof(argv[4]);
                int sizeKnl = atoi(argv[5]);
                float rdistance = atof(argv[6]);

                printf("Size of image: %d\n", sizeImg);
                printf("Sigma of Kernel: %f\n", sigma);
                printf("Size of Kernel: %d\n", sizeKnl);
                printf("Size of R distance: %f\n", rdistance);

                datastream = malloc(sizeImg * sizeImg * sizeof(float));
                
                // This contains the whole image in a single string
                // struct Image *img = malloc(sizeof(Image));
                // initImage(img, SIZE);
                // read the file
                limit = sizeImg * sizeImg;

                while (fgets(str, MAXCHAR, fp) != NULL){
                    char *ptr = strtok(str, comma);
                    for(i = 0; i < limit; i++){
                        datastream[i] = atof(ptr);
                        ptr = strtok(NULL, comma);
                    }
                }
                // now that the image is in the datastream:
                imgInput = malloc(sizeof(Image));
                initImage(imgInput, sizeImg);
                loadImage(imgInput, datastream);

                knl = malloc(sizeof(Kernel));
                initKernel(knl, sizeKnl);
                       
                generateHammingFilter(knl, rdistance);
                // something is wrong in here
                generateGaussianFilter(knl, sigma);

                imgOutput = malloc(sizeof(Image));
                initImage(imgOutput, sizeImg);
                convolve(imgInput, imgOutput, knl);

                // the int are usually under 256 & the comma and the +1 is for the null terminator
                result = malloc(4 * sizeImg * sizeImg * sizeof(char) + 1);
                for(i = 0; i < sizeImg; i++){
                    for(j = 0; j < sizeImg; j++){
                        char *tmp = malloc(128 * sizeof(char));
                        sprintf(tmp, "%d", (int) imgOutput -> data[j][i]), 
                        strcat(result, tmp);
                        strcat(result, comma);
                        free(tmp);
                    }
                }

                result[strlen(result) - 1] = '\0';

                // this is to write the outputs to a file
                fp = fopen("output.txt", "rb+");
                if(fp == NULL) //if file does not exist, create it
                {
                    fp = fopen("output.txt", "wb");
                    fputs(result, fp);
                }

                free(result);
                fclose(fp);

                deinitImage(imgInput);
                deinitImage(imgOutput);
                deinitKernel(knl);
                break;

            case 'f':
                printf("This is for the Naive fourier Transform\n");
                
                // input arguments required: input image, size of image, sigma of kernel and size of kernel
                // example:
                // ./processmake -f /home/heqianw/Documents/git/ECSE444-Filtering/prototyping/clean.txt 128 3.5
                
                fp = fopen(argv[2], "r");
                if(fp == NULL){
                    printf("can't open file, please provide the full path\n");
                    return 1;
                }

                sizeImg = atoi(argv[3]);
                rValue = atof(argv[4]);

                printf("Size of image: %d\n", sizeImg);
                printf("Size of R value: %f\n", rValue);
                datastream = malloc(sizeImg * sizeImg * sizeof(float));
                

                // This contains the whole image in a single string
                // struct Image *img = malloc(sizeof(Image));
                // initImage(img, SIZE);
                // read the file

                limit = sizeImg * sizeImg;

                while (fgets(str, MAXCHAR, fp) != NULL){
                    char *ptr = strtok(str, comma);
                    for(i = 0; i < limit; i++){
                        datastream[i] = atof(ptr);
                        ptr = strtok(NULL, comma);
                    }
                }

                // now that the image is in the datastream: load it into our struct
                imgInput = malloc(sizeof(Image));
                initImage(imgInput, sizeImg);
                loadImage(imgInput, datastream);

                cImgInput = malloc(sizeof(CImage));
                initComplex(cImgInput, sizeImg);
                loadComplexImage(cImgInput, datastream);

                knl = malloc(sizeof(Kernel));
                initKernel(knl, sizeImg);       
                generateHammingFilter(knl, rValue);

                // compute FFT of the two images into the complex
                fft(imgInput, cImgInput);

                // apply lowpass filter
                lowPass(cImgInput, knl);

                imgOutput = malloc(sizeof(Image));
                initImage(imgOutput, sizeImg);
                
                // inverse fft
                ifft(cImgInput, imgOutput);

                // the int are usually under 256 & the comma and the +1 is for the null terminator
                result = malloc(4 * sizeImg * sizeImg * sizeof(char) + 1);
                for(i = 0; i < sizeImg; i++){
                    for(j = 0; j < sizeImg; j++){
                        char *tmp = malloc(128 * sizeof(char));
                        sprintf(tmp, "%d", (int) imgOutput -> data[j][i]), 
                        strcat(result, tmp);
                        strcat(result, comma);
                        free(tmp);
                    }
                }

                result[strlen(result) - 1] = '\0';

                // this is to write the outputs to a file
                fp = fopen("output.txt", "rb+");
                if(fp == NULL) //if file does not exist, create it
                {
                    fp = fopen("output.txt", "wb");
                    fputs(result, fp);
                }

                free(result);
                fclose(fp);

                deinitImage(imgInput);
                deinitImage(imgOutput);
                deinitKernel(knl);
                break;

            case 't':
                
                printf("This is for the Couley Turkey fourier Transform\n");
                
                // input arguments required: input image, size of image, sigma of kernel and size of kernel
                // example:
                // ./processmake -f /home/heqianw/Documents/git/ECSE444-Filtering/prototyping/clean.txt 128 3.5
                
                fp = fopen(argv[2], "r");
                if(fp == NULL){
                    printf("can't open file, please provide the full path\n");
                    return 1;
                }

                sizeImg = atoi(argv[3]);
                rValue = atof(argv[4]);

                printf("Size of image: %d\n", sizeImg);
                printf("Size of R value: %f\n", rValue);
                datastream = malloc(sizeImg * sizeImg * sizeof(float));
                
                // This contains the whole image in a single string
                // struct Image *img = malloc(sizeof(Image));
                // initImage(img, SIZE);
                // read the file

                limit = sizeImg * sizeImg;

                while (fgets(str, MAXCHAR, fp) != NULL){
                    char *ptr = strtok(str, comma);
                    for(i = 0; i < limit; i++){
                        datastream[i] = atof(ptr);
                        ptr = strtok(NULL, comma);
                    }
                }

                // now that the image is in the datastream: load it into our struct
                cImgInput = malloc(sizeof(CImage));
                initComplex(cImgInput, sizeImg);
                loadComplexImage(cImgInput, datastream);

                knl = malloc(sizeof(Kernel));
                initKernel(knl, sizeImg);       
                generateHammingFilter(knl, rValue);
                printf("At CTFFT\n");
                // compute CTFFT of the image
                ctftt(cImgInput);
                printf("At lowpass\n");
                // apply lowpass filter
                lowPass(cImgInput, knl);

                imgOutput = malloc(sizeof(Image));
                initImage(imgOutput, sizeImg);
                printf("At CTIFFT\n");
                // inverse fft
                ctifft(cImgInput);
                imgOutput -> data = cImgInput -> re;


                // the int are usually under 256 & the comma and the +1 is for the null terminator
                result = malloc(4 * sizeImg * sizeImg * sizeof(char) + 1);
                for(i = 0; i < sizeImg; i++){
                    for(j = 0; j < sizeImg; j++){
                        char *tmp = malloc(128 * sizeof(char));
                        sprintf(tmp, "%d", (int) imgOutput -> data[j][i]), 
                        strcat(result, tmp);
                        strcat(result, comma);
                        free(tmp);
                    }
                }

                result[strlen(result) - 1] = '\0';

                // this is to write the outputs to a file
                fp = fopen("output.txt", "rb+");
                if(fp == NULL) //if file does not exist, create it
                {
                    fp = fopen("output.txt", "wb");
                    fputs(result, fp);
                }

                free(result);
                fclose(fp);

                deinitImage(imgInput);
                deinitImage(imgOutput);
                deinitKernel(knl);
                break;
        }
        argc--;
        argv++;
    }
    return 0;
}
