#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "conv.h"
#include "fourier.h"
#include "utils.h"

#define MAXCHAR 1000000
#define SIZE 128

double gettime(){
    struct timespec ts;
    if(clock_gettime(CLOCK_REALTIME, &ts) < 0)
        perror("Clock-gettime");
    return ts.tv_sec * 1000000 + ts.tv_nsec/1000;
}


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
    unsigned long sizeImg;
    float rValue;
    double start = 0;
    double end = 0;
    
    char *directory;
    char *inputFileName;
    char *fullPath;

    struct Image *imgInput;
    struct Image *imgOutput;
    struct CImage *cImgInput;
    struct Kernel *knl;
    unsigned long limit;

    char *sImageSize = malloc (50 * sizeof(char));
    char *sSigma = malloc (50 * sizeof(char));
    char *sKnlSize = malloc (50 * sizeof(char));
    char *sRDistance = malloc (50 * sizeof(char));
    char *sTimeTaken = malloc (50 * sizeof(char));

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
                fp = fopen("output.txt", "wb");
                fputs(result, fp);
                
                free(result);
                fclose(fp);

                deinitImage(img);
                break;
                
            case 'c':
                printf("This is for the Convolution test\n");

                // input arguments required: input image, size of image, sigma of kernel, size of kernel, rdistance
                // example:
                // ./processmake -c /home/heqianw/Documents/git/ECSE444-Filtering/prototyping/ clean.txt 128 0.2 5
                directory = argv[2];
                inputFileName = argv[3];
                
                fullPath = malloc(256 * sizeof(char));
                strcat(fullPath, directory);
                strcat(fullPath, inputFileName);
                
                fp = fopen(fullPath, "r");
                if(fp == NULL){
                    printf("can't open file, please provide the full path\n");
                    return 1;
                }
                
                sizeImg = atoi(argv[4]);
                float sigma = atof(argv[5]);
                int sizeKnl = atoi(argv[6]);

                printf("Size of image: %ld\n", sizeImg);
                printf("Sigma of Kernel: %f\n", sigma);
                printf("Size of Kernel: %d\n", sizeKnl);

                datastream = malloc(sizeImg * sizeImg * sizeof(float));
                
                // This contains the whole image in a single string
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
                // something is wrong in here
                generateGaussianFilter(knl, sigma);

                imgOutput = malloc(sizeof(Image));
                initImage(imgOutput, sizeImg);
                
                start = gettime();
                convolve(imgInput, imgOutput, knl);
                end = gettime();
                printf("The time taken for convolution was : %f us\n", end - start);

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
                free(fullPath);
                fullPath = malloc (256 * sizeof(char));
                strcat(fullPath, directory);
                strcat(fullPath, "output.txt");

                fp = fopen(fullPath, "wb");
                fputs(result, fp);
                fclose(fp);

                free(fullPath);
                fullPath = malloc (256 * sizeof(char));
                strcat(fullPath, directory);
                strcat(fullPath, "logging.txt");

                fp = fopen(fullPath, "wb");
                sprintf(sImageSize, "Size of image: %ld \n", sizeImg);
                sprintf(sKnlSize, "Size of Kernel: %d \n", sizeKnl);
                sprintf(sSigma, "Sigma Value: %f \n", sigma);
                sprintf(sTimeTaken, "Time taken Convolution: %f us \n", end - start);
                fputs(sImageSize, fp);
                fputs(sKnlSize, fp);
                fputs(sSigma, fp);
                fputs(sTimeTaken, fp);
                fclose(fp);

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
                
                directory = argv[2];
                inputFileName = argv[3];
                
                fullPath = malloc(256 * sizeof(char));
                strcat(fullPath, directory);
                strcat(fullPath, inputFileName);
                
                fp = fopen(fullPath, "r");
                if(fp == NULL){
                    printf("can't open file, please provide the full path\n");
                    return 1;
                }

                sizeImg = atoi(argv[4]);
                rValue = atof(argv[5]);

                printf("Size of image: %ld\n", sizeImg);
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

                imgOutput = malloc(sizeof(Image));
                initImage(imgOutput, sizeImg);

                // apply quickshift to the kernel
                shift(knl -> data, knl -> size);
                
                // compute FFT of the two images into the complex
                start = gettime();
                fft(imgInput, cImgInput);

                // apply lowpass filter
                lowPass(cImgInput, knl);
                
                // inverse fft
                ifft(cImgInput, imgOutput);
                
                end = gettime();
                printf("The time taken for Naive FFT was : %f us\n", end - start);


                // the int are usually under 256 & the comma and the +1 is for the null terminator
                result = malloc(4 * sizeImg * sizeImg * sizeof(char) + 1);
                for(i = 0; i < sizeImg; i++){
                    for(j = 0; j < sizeImg; j++){
                        char *tmp = malloc(128 * sizeof(char));
                        sprintf(tmp, "%d", (int) imgOutput -> data[j][i]); 
                        strcat(result, tmp);
                        strcat(result, comma);
                        free(tmp);
                    }
                }

                result[strlen(result) - 1] = '\0';

                // this is to write the outputs to a file
                free(fullPath);
                fullPath = malloc (256 * sizeof(char));
                strcat(fullPath, directory);
                strcat(fullPath, "output.txt");

                fp = fopen(fullPath, "wb");
                fputs(result, fp);
                fclose(fp);
                
                free(fullPath);
                fullPath = malloc (256 * sizeof(char));
                strcat(fullPath, directory);
                strcat(fullPath, "logging.txt");

                fp = fopen(fullPath, "wb");                
                sprintf(sImageSize, "Size of image: %ld \n", sizeImg);
                sprintf(sRDistance, "R value: %f \n", rValue);
                sprintf(sTimeTaken, "Time taken Naive FFT: %f us \n", end - start);
                fputs(sImageSize, fp);
                fputs(sRDistance, fp);
                fputs(sTimeTaken, fp);
                fclose(fp);

                free(result);

                deinitImage(imgInput);
                deinitImage(imgOutput);
                deinitKernel(knl);
                break;

            case 't':
                
                printf("This is for the Cooley-Tukey fourier Transform\n");
                
                // input arguments required: input image, size of image, sigma of kernel and size of kernel
                // example:
                // ./processmake -f /home/heqianw/Documents/git/ECSE444-Filtering/prototyping/clean.txt 128 3.5
                
                directory = argv[2];
                inputFileName = argv[3];
                
                fullPath = malloc(256 * sizeof(char));
                strcat(fullPath, directory);
                strcat(fullPath, inputFileName);
                
                fp = fopen(fullPath, "r");
                if(fp == NULL){
                    printf("can't open file, please provide the full path\n");
                    return 1;
                }

                sizeImg = atoi(argv[4]);
                rValue = atof(argv[5]);

                printf("Size of image: %ld\n", sizeImg);
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

                // apply quickshift to the kernel
                shift(knl -> data, knl -> size);

                start = gettime();
                // compute CTFFT of the image
                ctftt(cImgInput);

                // apply lowpass filter
                lowPass(cImgInput, knl);

                // inverse fft
                ctifft(cImgInput);
                end = gettime();
                printf("The time taken for Cooley-Tukey FFT was : %f us\n", end - start);

                // the int are usually under 256 & the comma and the +1 is for the null terminator
                free(result);
                result = malloc(5 * sizeImg * sizeImg * sizeof(char) + 1);
                for(i = 0; i < sizeImg; i++){
                    for(j = 0; j < sizeImg; j++){
                        char *tmp = malloc(128 * sizeof(char));
                        sprintf(tmp, "%d", (int) cImgInput -> re[j][i]); 
                        strcat(result, tmp);
                        strcat(result, comma);
                        free(tmp);
                    }
                }
                result[strlen(result) - 1] = '\0';
                // this is to write the outputs to a file
                free(fullPath);
                fullPath = malloc (256 * sizeof(char));
                strcat(fullPath, directory);
                strcat(fullPath, "output.txt");

                fp = fopen(fullPath, "wb");
                fputs(result, fp);
                fclose(fp);
                
                free(fullPath);
                fullPath = malloc (256 * sizeof(char));
                strcat(fullPath, directory);
                strcat(fullPath, "logging.txt");

                fp = fopen(fullPath, "wb");
                sprintf(sImageSize, "Size of image: %ld \n", sizeImg);
                sprintf(sRDistance, "R value: %f \n", rValue);
                sprintf(sTimeTaken, "Time taken Cooley-Tukey FFT: %f us \n", end - start);
                fputs(sImageSize, fp);
                fputs(sRDistance, fp);
                fputs(sTimeTaken, fp);
                fclose(fp);

                free(result);
                break;
        }
        argc--;
        argv++;
    }
    return 0;
}
