#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"


#define MAXCHAR 100000
#define SIZE 128

int main(int argc, char* argv[]){
    printf("Image processing \n");
    while((argc > 1) && (argv)){
        switch(argv[1][1]){
            case 'i':
                printf("This is for the input image, open an image in this step.\n");
                FILE *fp;
                char str[MAXCHAR];
                printf("%s", argv[2]);
                printf("\n");
                fp = fopen(argv[2], "r");
                if(fp == NULL){
                    printf("can't open file, please provide the full path\n");
                    return 1;
                }
                int i = 0;
                int j = 0;
                
                // This contains the whole image in a single string
                struct Image *img = malloc(sizeof(Image));
                initImage(img, SIZE);
                // printf("Yes");
                // read the file
                while (fgets(str, MAXCHAR, fp) != NULL){
                    char delim[] = ",";
                    char *ptr = strtok(str, delim);
                    for(i = 0; i < SIZE; i++){
                        for(j = 0; j < SIZE; j++){
                            img -> data[i][j] = (float) atoi(ptr);
                            ptr = strtok(NULL, delim);
                        }
                    }
                }

                // the int are usually under 256 & the comma and the +1 is for the null terminator
                char *result = malloc(4 * SIZE * SIZE * sizeof(char) + 1);
                int k = 0;
                char comma[] = ",";
                for(i = 0; i < SIZE; i++){
                    for(j = 0; j < SIZE; j++){
                        char *tmp = malloc(128 * sizeof(char));
                        sprintf(tmp, "%d", (int) img -> data[i][j]), 
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
            case 's':
                printf("This is for the shift test\n");
                break;
            case 'k':
                printf("This is for the kernel test\n");
                break;
            case 'c':
                printf("This is for the convolution test\n");
                break;
            case 'f':
                printf("This is for the fourier\n");
                break;
        }
        argc--;
        argv++;
    }
    return 0;
}
